#ifndef INSTRUMENT_ORDER_BOOK_HPP
#define INSTRUMENT_ORDER_BOOK_HPP

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <functional>

#include "atomic_list.hpp"
#include "engine.hpp"
#include "io.hpp"

struct BuySellAtomic
{
  uint32_t count;
  uint32_t executionId = 0;
  intmax_t executionTime = 0;
};

struct BuySellEntry
{
  const uint32_t order_id;
  const uint32_t price;
  std::atomic<BuySellAtomic *> parallel;
  intmax_t time_added = 0;
};

class InstrumentOrderBookEntry
{
private:
  AtomicList<BuySellEntry> buy;
  AtomicList<BuySellEntry> sell;
  void genericBuySell(BuySellEntry *order, bool isBuyPhase);
  void genericCancel(uint32_t orderId, bool isBuy);

  // Turnstile to handle all buys or sells first depending which comes first and block other type
  std::mutex mtx;
  std::condition_variable cv;
  int buyThreads = 0;
  int sellThreads = 0;

  void getBuyLock()
  {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]
            { return this->sellThreads == 0; });
    ++buyThreads;
  }

  void releaseBuyLock()
  {
    std::unique_lock<std::mutex> lock(mtx);
    --buyThreads;
    cv.notify_all();
  }

  void getSellLock()
  {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]
            { return this->buyThreads == 0; });
    ++sellThreads;
  }

  void releaseSellLock()
  {
    std::unique_lock<std::mutex> lock(mtx);
    --sellThreads;
    cv.notify_all();
  }

public:
  char instrument[9];

  InstrumentOrderBookEntry(char instrument[9])
  {
    std::memcpy(this->instrument, instrument, sizeof(this->instrument) - 1);
    this->instrument[8] = '\0';
  }

  void printOrders(std::string &output);

  void addBuyOrder(BuySellEntry *order)
  {
    this->getBuyLock();
    this->genericBuySell(order, true);
    this->releaseBuyLock();
  }

  void addSellOrder(BuySellEntry *order)
  {
    this->getSellLock();
    this->genericBuySell(order, false);
    this->releaseSellLock();
  }

  void addCancelOrder(uint32_t orderId, bool isBuy)
  {
    // isBuy ? this->getBuyLock() : this->getSellLock();
    this->genericCancel(orderId, isBuy);
    // isBuy ? this->releaseBuyLock() : this->releaseSellLock();
    ;
  }
};

struct InstrumentOrderBook
{
private:
  AtomicList<InstrumentOrderBookEntry> list;

public:
  InstrumentOrderBookEntry *getInstrument(char instrument[9]);
  void printAtomicList();
};

#endif
