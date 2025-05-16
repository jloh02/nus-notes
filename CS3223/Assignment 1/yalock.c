/*-------------------------------------------------------------------------
 *
 * freelist.c
 *        routines for managing the buffer pool's replacement strategy.
 *
 *
 * Portions Copyright (c) 1996-2024, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *        src/backend/storage/buffer/freelist.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "pgstat.h"
#include "port/atomics.h"
#include "storage/buf_internals.h"
#include "storage/bufmgr.h"
#include "storage/proc.h"

#define INT_ACCESS_ONCE(var) ((int)(*((volatile int *)&(var))))

/* (cs3223) Queue node storing internal_buf_id and refBit */
typedef struct
{
  int internal_buf_id;
  int refbit;
} QueueElement;

/*
 * The shared freelist control information.
 */
typedef struct
{
  /* Spinlock: protects the values below */
  slock_t buffer_strategy_lock;

  /*
   * Clock sweep hand: index of next buffer to consider grabbing. Note that
   * this isn't a concrete buffer - we only ever increase the value. So, to
   * get an actual buffer, it needs to be used modulo NBuffers.
   */
  pg_atomic_uint32 nextVictimBuffer;

  int firstFreeBuffer; /* Head of list of unused buffers */
  int lastFreeBuffer;  /* Tail of list of unused buffers */

  /*
   * NOTE: lastFreeBuffer is undefined when firstFreeBuffer is -1 (that is,
   * when the list is empty)
   */

  /*
   * Statistics.  These counters should be wide enough that they can't
   * overflow during a single bgwriter cycle.
   */
  uint32 completePasses;            /* Complete cycles of the clock sweep */
  pg_atomic_uint32 numBufferAllocs; /* Buffers allocated since last reset */

  /*
   * Bgworker process to be notified upon activity or -1 if none. See
   * StrategyNotifyBgWriter.
   */
  int bgwprocno;

  /* (cs3223) YACLOCK buffer queue*/
  slock_t queue_lock;
  int count; // Tail/number of elements
  int next;
  QueueElement *queue; // No need to maintain head as it is always 0
} BufferStrategyControl;

/* Pointers to shared state */
static BufferStrategyControl *StrategyControl = NULL;

/*
 * Private (non-shared) state for managing a ring of shared buffers to re-use.
 * This is currently the only kind of BufferAccessStrategy object, but someday
 * we might have more kinds.
 */
typedef struct BufferAccessStrategyData
{
  /* Overall strategy type */
  BufferAccessStrategyType btype;
  /* Number of elements in buffers[] array */
  int nbuffers;

  /*
   * Index of the "current" slot in the ring, ie, the one most recently
   * returned by GetBufferFromRing.
   */
  int current;

  /*
   * Array of buffer numbers.  InvalidBuffer (that is, zero) indicates we
   * have not yet selected a buffer for this ring slot.  For allocation
   * simplicity this is palloc'd together with the fixed fields of the
   * struct.
   */
  Buffer buffers[FLEXIBLE_ARRAY_MEMBER];
} BufferAccessStrategyData;

void StrategyAccessBuffer(int buf_id, int event_num); /* cs3223 */

/* Prototypes for internal functions */
static BufferDesc *GetBufferFromRing(BufferAccessStrategy strategy,
                                     uint32 *buf_state);
static void AddBufferToRing(BufferAccessStrategy strategy,
                            BufferDesc *buf);

/*
 * ClockSweepTick - Helper routine for StrategyGetBuffer()
 *
 * Move the clock hand one buffer ahead of its current position and return the
 * id of the buffer now under the hand.
 */
static inline uint32
ClockSweepTick(void)
{
  uint32 victim;
  uint32 originalVictim;

  uint32 expected;
  uint32 wrapped;
  bool success;

  /*
   * Atomically move hand ahead one buffer - if there's several processes
   * doing this, this can lead to buffers being returned slightly out of
   * apparent order.
   */

  victim = pg_atomic_fetch_add_u32(&StrategyControl->nextVictimBuffer, 1);

  if (victim >= NBuffers)
  {
    originalVictim = victim;

    /* always wrap what we look up in BufferDescriptors */
    victim = victim % NBuffers;

    /*
     * If we're the one that just caused a wraparound, force
     * completePasses to be incremented while holding the spinlock. We
     * need the spinlock so StrategySyncStart() can return a consistent
     * value consisting of nextVictimBuffer and completePasses.
     */
    if (victim == 0)
    {
      success = false;

      expected = originalVictim + 1;

      while (!success)
      {
        /*
         * Acquire the spinlock while increasing completePasses. That
         * allows other readers to read nextVictimBuffer and
         * completePasses in a consistent manner which is required for
         * StrategySyncStart().  In theory delaying the increment
         * could lead to an overflow of nextVictimBuffers, but that's
         * highly unlikely and wouldn't be particularly harmful.
         */
        SpinLockAcquire(&StrategyControl->buffer_strategy_lock);

        wrapped = expected % NBuffers;

        success = pg_atomic_compare_exchange_u32(&StrategyControl->nextVictimBuffer,
                                                 &expected, wrapped);
        if (success)
          StrategyControl->completePasses++;
        SpinLockRelease(&StrategyControl->buffer_strategy_lock);
      }
    }
  }
  return victim;
}

/*
 * have_free_buffer -- a lockless check to see if there is a free buffer in
 *                                         buffer pool.
 *
 * If the result is true that will become stale once free buffers are moved out
 * by other operations, so the caller who strictly want to use a free buffer
 * should not call this.
 */
bool have_free_buffer(void)
{
  if (StrategyControl->firstFreeBuffer >= 0)
    return true;
  else
    return false;
}

/*
 * StrategyGetBuffer
 *
 *      Called by the bufmgr to get the next candidate buffer to use in
 *      BufferAlloc(). The only hard requirement BufferAlloc() has is that
 *      the selected buffer must not currently be pinned by anyone.
 *
 *      strategy is a BufferAccessStrategy object, or NULL for default strategy.
 *
 *      To ensure that no one else can pin the buffer before we do, we must
 *      return the buffer with the buffer header spinlock still held.
 */
BufferDesc *
StrategyGetBuffer(BufferAccessStrategy strategy, uint32 *buf_state, bool *from_ring)
{
  BufferDesc *buf;
  int bgwprocno;
  int trycounter;
  uint32 local_buf_state; /* to avoid repeated (de-)referencing */
  int idx;
  int i;
  QueueElement queueElem;

  // elog(LOG, "StrategyGetBuffer (%d)", strategy == NULL);

  *from_ring = false;

  /*
   * If given a strategy object, see whether it can select a buffer. We
   * assume strategy objects don't need buffer_strategy_lock.
   */
  if (strategy != NULL)
  {
    buf = GetBufferFromRing(strategy, buf_state);

    if (buf != NULL)
    {
      *from_ring = true;
      return buf;
    }
  }

  bgwprocno = INT_ACCESS_ONCE(StrategyControl->bgwprocno);
  if (bgwprocno != -1)
  {
    StrategyControl->bgwprocno = -1;
    SetLatch(&ProcGlobal->allProcs[bgwprocno].procLatch);
  }

  pg_atomic_fetch_add_u32(&StrategyControl->numBufferAllocs, 1);

  /*
   * First check, without acquiring the lock, whether there's buffers in the
   * freelist. Since we otherwise don't require the spinlock in every
   * StrategyGetBuffer() invocation, it'd be sad to acquire it here -
   * uselessly in most cases. That obviously leaves a race where a buffer is
   * put on the freelist but we don't see the store yet - but that's pretty
   * harmless, it'll just get used during the next buffer acquisition.
   *
   * If there's buffers on the freelist, acquire the spinlock to pop one
   * buffer of the freelist. Then check whether that buffer is usable and
   * repeat if not.
   *
   * Note that the freeNext fields are considered to be protected by the
   * buffer_strategy_lock not the individual buffer spinlocks, so it's OK to
   * manipulate them without holding the spinlock.
   */
  // elog(LOG, "StrategyGetBuffer - Stage 2");

  if (StrategyControl->firstFreeBuffer >= 0)
  {
    while (true)
    {
      SpinLockAcquire(&StrategyControl->buffer_strategy_lock);

      if (StrategyControl->firstFreeBuffer < 0)
      {
        SpinLockRelease(&StrategyControl->buffer_strategy_lock);
        break;
      }

      buf = GetBufferDescriptor(StrategyControl->firstFreeBuffer);
      Assert(buf->freeNext != FREENEXT_NOT_IN_LIST);

      StrategyControl->firstFreeBuffer = buf->freeNext;
      buf->freeNext = FREENEXT_NOT_IN_LIST;

      SpinLockRelease(&StrategyControl->buffer_strategy_lock);

      local_buf_state = LockBufHdr(buf);

      if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && BUF_STATE_GET_USAGECOUNT(local_buf_state) == 0)
      {
        /* (cs3223) valid buffer found, return! */
        if (strategy != NULL)
          AddBufferToRing(strategy, buf);
        *buf_state = local_buf_state;

        /* cs3223 CASE 2 */
        StrategyAccessBuffer(buf->buf_id, 2);

        return buf;
      }

      UnlockBufHdr(buf, local_buf_state);
    }
  }

  // elog(LOG, "StrategyGetBuffer - Stage 3");

  /* (cs3223) freelist is empty, run YACLOCK sweep algorithm. */
  SpinLockAcquire(&StrategyControl->queue_lock);
  trycounter = NBuffers;

  /* (cs3223) check next, and set next to head of queue if null */
  if (StrategyControl->next == -1)
  {
    StrategyControl->next = 0;
  }

  while (true)
  {
    /* (cs3223) retrieve current buffer in queue */
    // elog(LOG, "Stage 3 - Try %d (Out of %d)", StrategyControl->next, StrategyControl->count);

    idx = StrategyControl->next;
    buf = GetBufferDescriptor(StrategyControl->queue[idx].internal_buf_id);
    local_buf_state = LockBufHdr(buf);

    /* (cs3223) check if buffer is pinned */
    if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0)
    {
      // elog(LOG, "Stage 3 - Ref %d", StrategyControl->queue[idx].refbit);

      // Still being used, just decrement and try again
      //  if (BUF_STATE_GET_USAGECOUNT(local_buf_state) != 0)
      //  {
      //    local_buf_state -= BUF_USAGECOUNT_ONE;

      //   trycounter = NBuffers;
      // }

      if (StrategyControl->queue[idx].refbit == 1) /* (cs3223) check refBit of retrieved buffer */
      {
        // elog(LOG, "Stage 3a");
        /* (cs3223) refBit = 1, set to 0 and increment next */
        StrategyControl->queue[idx].refbit = 0;

        trycounter = NBuffers;
      }
      else
      {
        // elog(LOG, "Stage 3b");
        /* (cs3223) valid buffer found, move it to the end of the queue.
         * next should be pointing to the next buffer to check.
         *
         * TODO: set buffer useagecount to 0? and dont UnlockBufHdr?
         *       (bc that was what was done in the original code)
         */

        if (strategy != NULL)
          AddBufferToRing(strategy, buf);
        *buf_state = local_buf_state;

        /* (cs3223) move buffer to end of queue */
        queueElem = StrategyControl->queue[idx];
        for (i = idx; i < StrategyControl->count - 1; i++)
        {
          StrategyControl->queue[i].internal_buf_id = StrategyControl->queue[i + 1].internal_buf_id;
          StrategyControl->queue[i].refbit = StrategyControl->queue[i + 1].refbit;
        }
        StrategyControl->queue[StrategyControl->count - 1] = queueElem;

        // (cs3223) wrap around next if last element because the victim is last element
        if (idx == StrategyControl->count - 1)
        {
          StrategyControl->next = 0;
        }

        // elog(LOG, "Stage 3b - Moved buffer %d to end of queue", buf->buf_id);

        // This call only releases the lock and does nothing else
        StrategyAccessBuffer(buf->buf_id, 3); /* cs3223 CASE 3 */

        return buf;
      }
    }
    else if (--trycounter == 0)
    {
      /* (cs3223 ) no unpinned buffers available */
      UnlockBufHdr(buf, local_buf_state);
      SpinLockRelease(&StrategyControl->queue_lock);
      // elog(ERROR, "CASE 3 no unpinned buffers available");
    }

    /* (cs3223) buffer is pinned, unlock buffer and increment next */
    UnlockBufHdr(buf, local_buf_state);
    StrategyControl->next = (StrategyControl->next + 1) % StrategyControl->count;
  }
}

/*
 * StrategyFreeBuffer: put a buffer on the freelist
 */
void StrategyFreeBuffer(BufferDesc *buf)
{
  // elog(LOG, "StrategyFreeBuffer");
  SpinLockAcquire(&StrategyControl->buffer_strategy_lock);
  StrategyAccessBuffer(buf->buf_id, 4); /* cs3223 CASE 4 */

  /*
   * It is possible that we are told to put something in the freelist that
   * is already in it; don't screw up the list if so.
   */
  if (buf->freeNext == FREENEXT_NOT_IN_LIST)
  {
    buf->freeNext = StrategyControl->firstFreeBuffer;
    if (buf->freeNext < 0)
      StrategyControl->lastFreeBuffer = buf->buf_id;
    StrategyControl->firstFreeBuffer = buf->buf_id;
  }

  SpinLockRelease(&StrategyControl->buffer_strategy_lock);
}

/*
 * StrategySyncStart -- tell BufferSync where to start syncing
 *
 * The result is the buffer index of the best buffer to sync first.
 * BufferSync() will proceed circularly around the buffer array from there.
 *
 * In addition, we return the completed-pass count (which is effectively
 * the higher-order bits of nextVictimBuffer) and the count of recent buffer
 * allocs if non-NULL pointers are passed.  The alloc count is reset after
 * being read.
 */
int StrategySyncStart(uint32 *complete_passes, uint32 *num_buf_alloc)
{
  uint32 nextVictimBuffer;
  int result;

  SpinLockAcquire(&StrategyControl->buffer_strategy_lock);
  nextVictimBuffer = pg_atomic_read_u32(&StrategyControl->nextVictimBuffer);
  result = nextVictimBuffer % NBuffers;

  if (complete_passes)
  {
    *complete_passes = StrategyControl->completePasses;

    /*
     * Additionally add the number of wraparounds that happened before
     * completePasses could be incremented. C.f. ClockSweepTick().
     */
    *complete_passes += nextVictimBuffer / NBuffers;
  }

  if (num_buf_alloc)
  {
    *num_buf_alloc = pg_atomic_exchange_u32(&StrategyControl->numBufferAllocs, 0);
  }
  SpinLockRelease(&StrategyControl->buffer_strategy_lock);
  return result;
}

/*
 * StrategyNotifyBgWriter -- set or clear allocation notification latch
 *
 * If bgwprocno isn't -1, the next invocation of StrategyGetBuffer will
 * set that latch.  Pass -1 to clear the pending notification before it
 * happens.  This feature is used by the bgwriter process to wake itself up
 * from hibernation, and is not meant for anybody else to use.
 */
void StrategyNotifyBgWriter(int bgwprocno)
{
  /*
   * We acquire buffer_strategy_lock just to ensure that the store appears
   * atomic to StrategyGetBuffer.  The bgwriter should call this rather
   * infrequently, so there's no performance penalty from being safe.
   */
  SpinLockAcquire(&StrategyControl->buffer_strategy_lock);
  StrategyControl->bgwprocno = bgwprocno;
  SpinLockRelease(&StrategyControl->buffer_strategy_lock);
}

/*
 * StrategyShmemSize
 *
 * estimate the size of shared memory used by the freelist-related structures.
 *
 * Note: for somewhat historical reasons, the buffer lookup hashtable size
 * is also determined here.
 */
Size StrategyShmemSize(void)
{
  Size size = 0;

  /* size of lookup hash table ... see comment in StrategyInitialize */
  size = add_size(size, BufTableShmemSize(NBuffers + NUM_BUFFER_PARTITIONS));

  /* size of the shared replacement strategy control block */
  size = add_size(size, MAXALIGN(sizeof(BufferStrategyControl)));

  return size;
}

/*
 * StrategyInitialize -- initialize the buffer cache replacement
 *              strategy.
 *
 * Assumes: All of the buffers are already built into a linked list.
 *              Only called by postmaster and only during initialization.
 */
void StrategyInitialize(bool init)
{
  bool found;

  /*
   * Initialize the shared buffer lookup hashtable.
   *
   * Since we can't tolerate running out of lookup table entries, we must be
   * sure to specify an adequate table size here.  The maximum steady-state
   * usage is of course NBuffers entries, but BufferAlloc() tries to insert
   * a new entry before deleting the old.  In principle this could be
   * happening in each partition concurrently, so we could need as many as
   * NBuffers + NUM_BUFFER_PARTITIONS entries.
   */
  InitBufTable(NBuffers + NUM_BUFFER_PARTITIONS);

  /*
   * Get or create the shared strategy control block
   */
  StrategyControl = (BufferStrategyControl *)
      ShmemInitStruct("Buffer Strategy Status",
                      sizeof(BufferStrategyControl),
                      &found);

  if (!found)
  {
    // elog(LOG, "Buffer Strategy Status initializing...");

    /*
     * Only done once, usually in postmaster
     */
    Assert(init);

    SpinLockInit(&StrategyControl->buffer_strategy_lock);
    SpinLockInit(&StrategyControl->queue_lock);

    /*
     * Grab the whole linked list of free buffers for our strategy. We
     * assume it was previously set up by InitBufferPool().
     */
    StrategyControl->firstFreeBuffer = 0;
    StrategyControl->lastFreeBuffer = NBuffers - 1;

    /* Initialize the clock sweep pointer */
    pg_atomic_init_u32(&StrategyControl->nextVictimBuffer, 0);

    /* Clear statistics */
    StrategyControl->completePasses = 0;
    pg_atomic_init_u32(&StrategyControl->numBufferAllocs, 0);

    /* No pending notification */
    StrategyControl->bgwprocno = -1;

    /* (cs3223) initializing queue and next pointer */
    StrategyControl->count = 0;
    StrategyControl->next = -1;

    StrategyControl->queue = (QueueElement *)ShmemAlloc(sizeof(QueueElement) * (NBuffers));
    if (!StrategyControl->queue)
    {
      // elog(ERROR, "ShmemAlloc failed: Unable to allocate buffer queue.");
      return;
    }

    for (int i = 0; i < NBuffers; i++)
    {
      StrategyControl->queue[i].internal_buf_id = InvalidBuffer;
      StrategyControl->queue[i].refbit = 0;
    }

    // elog(LOG, "Buffer Strategy Status initialized");
  }
  else
    Assert(!init);
}

/* ----------------------------------------------------------------
 *                              Backend-private buffer ring management
 * ----------------------------------------------------------------
 */

/*
 * GetAccessStrategy -- create a BufferAccessStrategy object
 *
 * The object is allocated in the current memory context.
 */
BufferAccessStrategy
GetAccessStrategy(BufferAccessStrategyType btype)
{
  int ring_size_kb;

  /*
   * Select ring size to use.  See buffer/README for rationales.
   *
   * Note: if you change the ring size for BAS_BULKREAD, see also
   * SYNC_SCAN_REPORT_INTERVAL in access/heap/syncscan.c.
   */
  switch (btype)
  {
  case BAS_NORMAL:
    /* if someone asks for NORMAL, just give 'em a "default" object */
    return NULL;

  case BAS_BULKREAD:
    ring_size_kb = 256;
    break;
  case BAS_BULKWRITE:
    ring_size_kb = 16 * 1024;
    break;
  case BAS_VACUUM:
    ring_size_kb = 2048;
    break;

  default:
    elog(ERROR, "unrecognized buffer access strategy: %d", (int)btype);
    return NULL; /* keep compiler quiet */
  }

  return GetAccessStrategyWithSize(btype, ring_size_kb);
}

/*
 * GetAccessStrategyWithSize -- create a BufferAccessStrategy object with a
 *              number of buffers equivalent to the passed in size.
 *
 * If the given ring size is 0, no BufferAccessStrategy will be created and
 * the function will return NULL.  ring_size_kb must not be negative.
 */
BufferAccessStrategy
GetAccessStrategyWithSize(BufferAccessStrategyType btype, int ring_size_kb)
{
  int ring_buffers;
  BufferAccessStrategy strategy;

  Assert(ring_size_kb >= 0);

  /* Figure out how many buffers ring_size_kb is */
  ring_buffers = ring_size_kb / (BLCKSZ / 1024);

  /* 0 means unlimited, so no BufferAccessStrategy required */
  if (ring_buffers == 0)
    return NULL;

  /* Cap to 1/8th of shared_buffers */
  ring_buffers = Min(NBuffers / 8, ring_buffers);

  /* NBuffers should never be less than 16, so this shouldn't happen */
  Assert(ring_buffers > 0);

  /* Allocate the object and initialize all elements to zeroes */
  strategy = (BufferAccessStrategy)
      palloc0(offsetof(BufferAccessStrategyData, buffers) +
              ring_buffers * sizeof(Buffer));

  /* Set fields that don't start out zero */
  strategy->btype = btype;
  strategy->nbuffers = ring_buffers;

  return strategy;
}

/*
 * GetAccessStrategyBufferCount -- an accessor for the number of buffers in
 *              the ring
 *
 * Returns 0 on NULL input to match behavior of GetAccessStrategyWithSize()
 * returning NULL with 0 size.
 */
int GetAccessStrategyBufferCount(BufferAccessStrategy strategy)
{
  if (strategy == NULL)
    return 0;

  return strategy->nbuffers;
}

/*
 * GetAccessStrategyPinLimit -- get cap of number of buffers that should be pinned
 *
 * When pinning extra buffers to look ahead, users of a ring-based strategy are
 * in danger of pinning too much of the ring at once while performing look-ahead.
 * For some strategies, that means "escaping" from the ring, and in others it
 * means forcing dirty data to disk very frequently with associated WAL
 * flushing.  Since external code has no insight into any of that, allow
 * individual strategy types to expose a clamp that should be applied when
 * deciding on a maximum number of buffers to pin at once.
 *
 * Callers should combine this number with other relevant limits and take the
 * minimum.
 */
int GetAccessStrategyPinLimit(BufferAccessStrategy strategy)
{
  if (strategy == NULL)
    return NBuffers;

  switch (strategy->btype)
  {
  case BAS_BULKREAD:

    /*
     * Since BAS_BULKREAD uses StrategyRejectBuffer(), dirty buffers
     * shouldn't be a problem and the caller is free to pin up to the
     * entire ring at once.
     */
    return strategy->nbuffers;

  default:

    /*
     * Tell caller not to pin more than half the buffers in the ring.
     * This is a trade-off between look ahead distance and deferring
     * writeback and associated WAL traffic.
     */
    return strategy->nbuffers / 2;
  }
}

/*
 * FreeAccessStrategy -- release a BufferAccessStrategy object
 *
 * A simple pfree would do at the moment, but we would prefer that callers
 * don't assume that much about the representation of BufferAccessStrategy.
 */
void FreeAccessStrategy(BufferAccessStrategy strategy)
{
  /* don't crash if called on a "default" strategy */
  if (strategy != NULL)
    pfree(strategy);
}

/*
 * GetBufferFromRing -- returns a buffer from the ring, or NULL if the
 *              ring is empty / not usable.
 *
 * The bufhdr spin lock is held on the returned buffer.
 */
static BufferDesc *
GetBufferFromRing(BufferAccessStrategy strategy, uint32 *buf_state)
{
  BufferDesc *buf;
  Buffer bufnum;
  uint32 local_buf_state; /* to avoid repeated (de-)referencing */

  /* Advance to next ring slot */
  if (++strategy->current >= strategy->nbuffers)
    strategy->current = 0;

  /*
   * If the slot hasn't been filled yet, tell the caller to allocate a new
   * buffer with the normal allocation strategy.  He will then fill this
   * slot by calling AddBufferToRing with the new buffer.
   */
  bufnum = strategy->buffers[strategy->current];
  if (bufnum == InvalidBuffer)
    return NULL;

  /*
   * If the buffer is pinned we cannot use it under any circumstances.
   *
   * If usage_count is 0 or 1 then the buffer is fair game (we expect 1,
   * since our own previous usage of the ring element would have left it
   * there, but it might've been decremented by clock sweep since then). A
   * higher usage_count indicates someone else has touched the buffer, so we
   * shouldn't re-use it.
   */
  buf = GetBufferDescriptor(bufnum - 1);
  local_buf_state = LockBufHdr(buf);
  if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && BUF_STATE_GET_USAGECOUNT(local_buf_state) <= 1)
  {
    *buf_state = local_buf_state;
    return buf;
  }
  UnlockBufHdr(buf, local_buf_state);

  /*
   * Tell caller to allocate a new buffer with the normal allocation
   * strategy.  He'll then replace this ring element via AddBufferToRing.
   */
  return NULL;
}

/*
 * AddBufferToRing -- add a buffer to the buffer ring
 *
 * Caller must hold the buffer header spinlock on the buffer.  Since this
 * is called with the spinlock held, it had better be quite cheap.
 */
static void
AddBufferToRing(BufferAccessStrategy strategy, BufferDesc *buf)
{
  // elog(LOG, "ADD TO RING");
  strategy->buffers[strategy->current] = BufferDescriptorGetBuffer(buf);
}

/*
 * Utility function returning the IOContext of a given BufferAccessStrategy's
 * strategy ring.
 */
IOContext
IOContextForStrategy(BufferAccessStrategy strategy)
{
  if (!strategy)
    return IOCONTEXT_NORMAL;

  switch (strategy->btype)
  {
  case BAS_NORMAL:

    /*
     * Currently, GetAccessStrategy() returns NULL for
     * BufferAccessStrategyType BAS_NORMAL, so this case is
     * unreachable.
     */
    pg_unreachable();
    return IOCONTEXT_NORMAL;
  case BAS_BULKREAD:
    return IOCONTEXT_BULKREAD;
  case BAS_BULKWRITE:
    return IOCONTEXT_BULKWRITE;
  case BAS_VACUUM:
    return IOCONTEXT_VACUUM;
  }

  elog(ERROR, "unrecognized BufferAccessStrategyType: %d", strategy->btype);
  pg_unreachable();
}

/*
 * StrategyRejectBuffer -- consider rejecting a dirty buffer
 *
 * When a nondefault strategy is used, the buffer manager calls this function
 * when it turns out that the buffer selected by StrategyGetBuffer needs to
 * be written out and doing so would require flushing WAL too.  This gives us
 * a chance to choose a different victim.
 *
 * Returns true if buffer manager should ask for a new victim, and false
 * if this buffer should be written and re-used.
 */
bool StrategyRejectBuffer(BufferAccessStrategy strategy, BufferDesc *buf, bool from_ring)
{
  /* We only do this in bulkread mode */
  if (strategy->btype != BAS_BULKREAD)
    return false;

  /* Don't muck with behavior of normal buffer-replacement strategy */
  if (!from_ring ||
      strategy->buffers[strategy->current] != BufferDescriptorGetBuffer(buf))
    return false;

  /*
   * Remove the dirty buffer from the ring; necessary to prevent infinite
   * loop if all ring members are dirty.
   */
  strategy->buffers[strategy->current] = InvalidBuffer;

  return true;
}

void StrategyAccessBuffer(int buf_id, int event_num)
{
  int i;
  // BufferDesc *buf;
  // uint32 buf_state;

  switch (event_num)
  {
  case 1:
    SpinLockAcquire(&StrategyControl->buffer_strategy_lock);
    SpinLockAcquire(&StrategyControl->queue_lock);
    for (i = 0; i < StrategyControl->count; i++)
    {
      if (StrategyControl->queue[i].internal_buf_id == buf_id)
      {
        StrategyControl->queue[i].refbit = 1;
        break;
      }
    }
    SpinLockRelease(&StrategyControl->queue_lock);
    SpinLockRelease(&StrategyControl->buffer_strategy_lock);
    // elog(LOG, "CASE 1 done");
    break;

  case 2:
    SpinLockAcquire(&StrategyControl->queue_lock);
    StrategyControl->queue[StrategyControl->count].internal_buf_id = buf_id;
    StrategyControl->queue[StrategyControl->count].refbit = 0;
    StrategyControl->count++;
    SpinLockRelease(&StrategyControl->queue_lock);
    // elog(LOG, "CASE 2 done");
    break;

  case 3:
    SpinLockRelease(&StrategyControl->queue_lock);
    // elog(LOG, "CASE 3 done");
    break;

  case 4:
    SpinLockAcquire(&StrategyControl->queue_lock);
    i = StrategyControl->next;
    for (; i < StrategyControl->count - 1; i++)
    {
      StrategyControl->queue[i] = StrategyControl->queue[i + 1];
    }

    StrategyControl->queue[StrategyControl->count - 1].internal_buf_id = InvalidBuffer;
    StrategyControl->queue[StrategyControl->count - 1].refbit = 0;

    StrategyControl->count--;

    if (StrategyControl->count == 0)
    {
      StrategyControl->next = -1;
    }

    SpinLockRelease(&StrategyControl->queue_lock);
    // elog(LOG, "CASE 4 done");
    break;

  default:
    // elog(ERROR, "unrecognized event_num: %d", event_num);
    break;
  }

  // Debug print all buffer states
  // SpinLockAcquire(&StrategyControl->queue_lock);
  // elog(LOG, "Queue: ");
  // for (i = 0; i < StrategyControl->count; i++)
  // {
  // buf = GetBufferDescriptor(StrategyControl->queue[i].internal_buf_id);
  // buf_state = pg_atomic_read_u32(&buf->state);

  // elog(LOG, "%d - internal_buf_id: %d, refbit: %d, refcount: %d", i, StrategyControl->queue[i].internal_buf_id, StrategyControl->queue[i].refbit, BUF_STATE_GET_REFCOUNT(buf_state));
  // }
  // SpinLockRelease(&StrategyControl->queue_lock);

  // elog(LOG, "---CASE %d: %d", event_num, buf_id);
}