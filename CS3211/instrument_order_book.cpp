#include "instrument_order_book.hpp"

void InstrumentOrderBookEntry::printOrders(std::string &output)
{
  std::function<bool(AtomicList<BuySellEntry>::Node *)> traverseInstrument = [&output](AtomicList<BuySellEntry>::Node *curr)
  {
    output += "ID:" + std::to_string(curr->data->order_id) + " Price:" + std::to_string(curr->data->price) + " Count:" + std::to_string(curr->data->parallel.load(std::memory_order_acquire)->count) + " | ";
    return false;
  };

  output += "(Sell Orders) ";
  this->sell.traverse(traverseInstrument);
  output += "(Buy Orders) ";
  this->buy.traverse(traverseInstrument);
}

void InstrumentOrderBook::printAtomicList()
{
  std::string output = "";

  std::function<bool(AtomicList<InstrumentOrderBookEntry>::Node *)> traverseBook = [&output](AtomicList<InstrumentOrderBookEntry>::Node *curr)
  {
    output += "Instrument " + std::string(curr->data->instrument) + ": ";
    curr->data->printOrders(output);
    output += "\n\n";
    return false;
  };

  this->list.traverse(traverseBook);
  SyncCerr{} << "\nList:\n"
             << output << std::endl;
}

InstrumentOrderBookEntry *InstrumentOrderBook::getInstrument(char instrument[9])
{
  std::function<bool(AtomicList<InstrumentOrderBookEntry>::Node *)> checkInstrumentReturn = [instrument](AtomicList<InstrumentOrderBookEntry>::Node *curr)
  {
    if (curr == nullptr)
    {
      // SyncCerr{} << "NULL HEAD " << std::endl;
      return false;
    }

    // SyncCerr{} << "Instrument " << curr << " " << curr->data->instrument << std::endl;
    return strcmp(curr->data->instrument, instrument) == 0;
  };

  InstrumentOrderBookEntry *newEntry = new InstrumentOrderBookEntry(instrument);
  InstrumentOrderBookEntry *actualEntry = this->list.getOrInsert(newEntry, checkInstrumentReturn);
  // SyncCerr{} << "Actual Entry" << actualEntry << std::endl;
  return actualEntry;
}

bool isBetterOrder(BuySellEntry *a, BuySellEntry *b, bool isBuyPhase) // is a better than b? isBuyPhase=true when buy orders are being compared against sell list
{
  if (a->price != b->price)
  {
    return (a->price > b->price) ^ isBuyPhase; // When isbuyPhase=1, a is better when a.price < b.price
  }
  return a->time_added < b->time_added;
}

void InstrumentOrderBookEntry::genericBuySell(BuySellEntry *order, bool isBuyPhase)
{
  AtomicList<BuySellEntry> *listToSearch = isBuyPhase ? &(this->sell) : &(this->buy);
  bool found = true;

  // Repeatedly find a desirable match
  while (order->parallel.load(std::memory_order_relaxed)->count > 0 && found)
  {
    BuySellEntry *bestMatch = nullptr;
    found = false;
    std::function<bool(AtomicList<BuySellEntry>::Node *)> findBestOrder = [&bestMatch, order, &found, isBuyPhase](AtomicList<BuySellEntry>::Node *curr)
    {
      // SyncCerr{} << "Order " << curr << " " << curr->data->order_id << std::endl;

      // Ensures buy price >= sell price
      bool isUsablePrice = isBuyPhase ? (order->price >= curr->data->price) : (curr->data->price >= order->price);

      if ((curr->data->parallel.load(std::memory_order_acquire))->count > 0 && isUsablePrice)
      {
        if (bestMatch == nullptr || isBetterOrder(curr->data, bestMatch, isBuyPhase))
        {
          bestMatch = curr->data;
          found = true;
        }
      }
      return false;
    };
    listToSearch->traverse(findBestOrder);

    BuySellAtomic *expectedBestMatch;
    while (found && (expectedBestMatch = bestMatch->parallel.load(std::memory_order_acquire))->count > 0)
    {
      BuySellAtomic *currEntry = order->parallel.load(std::memory_order_relaxed);
      uint32_t expectedCountToUse = std::min(expectedBestMatch->count, currEntry->count);
      uint32_t remainder = expectedBestMatch->count - expectedCountToUse;

      BuySellAtomic *swapEntry = new BuySellAtomic{remainder, expectedBestMatch->executionId + 1, getCurrentTimestamp()};

      if (bestMatch->parallel.compare_exchange_weak(expectedBestMatch, swapEntry, std::memory_order_acq_rel, std::memory_order_relaxed))
      {
        Output::OrderExecuted(bestMatch->order_id, order->order_id, swapEntry->executionId, bestMatch->price, expectedCountToUse, swapEntry->executionTime);

        BuySellAtomic *activeCurr = order->parallel.load(std::memory_order_relaxed);

        order->parallel.store(new BuySellAtomic{activeCurr->count - expectedCountToUse, activeCurr->executionId, 0}, std::memory_order_relaxed);

        // order->executionId++;
        break;
      }
    }
  }

  BuySellAtomic *currOrderAtomic = order->parallel.load(std::memory_order_relaxed);
  if (currOrderAtomic->count > 0)
  {
    (isBuyPhase ? this->buy : this->sell).insert(order);
    // SyncCerr{} << "Inserted " << order << " " << order->order_id << " " << order->count << " " << order->price << std::endl;
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderAdded(order->order_id, this->instrument, order->price, currOrderAtomic->count, !isBuyPhase, output_time);
    order->parallel.store(new BuySellAtomic{currOrderAtomic->count, currOrderAtomic->executionId, output_time}, std::memory_order_release);
    order->time_added = output_time;
  }
}

void InstrumentOrderBookEntry::genericCancel(uint32_t orderId, bool isBuy)
{
  AtomicList<BuySellEntry> *listToSearch = isBuy ? &(this->buy) : &(this->sell);
  BuySellEntry *cancelEntry = nullptr;
  std::function<bool(AtomicList<BuySellEntry>::Node *)> findOrder = [orderId, &cancelEntry](AtomicList<BuySellEntry>::Node *curr)
  {
    // SyncCerr{} << "i " << curr << " " << curr->next << " " << curr->data->order_id << " " << curr->data->price << " " << curr->data->count << std::endl;

    if (orderId == curr->data->order_id)
    {
      cancelEntry = curr->data;
      return true;
    }
    return false;
  };

  bool found = listToSearch->traverse(findOrder);

  if (!found)
  {
    // SyncCerr{} << "ERROR: Invalid order not found and rejected " << orderId << std::endl;
    intmax_t output_time = getCurrentTimestamp();
    Output::OrderDeleted(orderId, false, output_time);
    return;
  }

  BuySellAtomic *expectedEntry;
  while ((expectedEntry = cancelEntry->parallel.load(std::memory_order_acquire))->count > 0)
  {
    if (cancelEntry->parallel.compare_exchange_weak(expectedEntry, (new BuySellAtomic{0, 0, 0}), std::memory_order_acq_rel, std::memory_order_relaxed))
    {
      intmax_t output_time = getCurrentTimestamp();
      Output::OrderDeleted(orderId, true, output_time);
      return;
    }
  }

  intmax_t output_time = getCurrentTimestamp();
  Output::OrderDeleted(orderId, false, output_time);
}
