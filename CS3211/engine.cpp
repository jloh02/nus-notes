#include <iostream>
#include <thread>
#include <unordered_map>

#include "io.hpp"
#include "engine.hpp"

InstrumentOrderBook orderBook;

void Engine::accept(ClientConnection connection)
{
  auto thread = std::thread(&Engine::connection_thread, this, std::move(connection));
  thread.detach();
}

void Engine::connection_thread(ClientConnection connection)
{
  std::unordered_map<uint32_t, std::pair<std::string, bool>> mapOrder;

  while (true)
  {
    ClientCommand input{};
    switch (connection.readInput(input))
    {
    case ReadResult::Error:
      SyncCerr{} << "Error reading input" << std::endl;
    case ReadResult::EndOfFile:
      return;
    case ReadResult::Success:
      break;
    }

    // Functions for printing output actions in the prescribed format are
    // provided in the Output class:
    switch (input.type)
    {
    case input_cancel:
    {
      // SyncCerr{} << "Got cancel: ID: " << input.order_id << std::endl;

      std::pair<std::string, bool> cancelOrder = mapOrder.find(input.order_id)->second;

      char instrumentCancel[9];
      strcpy(instrumentCancel, cancelOrder.first.c_str());
      orderBook.getInstrument(instrumentCancel)->addCancelOrder(input.order_id, cancelOrder.second);
      // orderBook.printAtomicList();
      break;
    }

    case input_buy:
    {
      // SyncCerr{}
      //     << "Got buy order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
      //     << input.price << " ID: " << input.order_id << std::endl;

      BuySellAtomic *entrySub = new BuySellAtomic{input.count};
      BuySellEntry *entry = new BuySellEntry{input.order_id, input.price, entrySub};
      orderBook.getInstrument(input.instrument)->addBuyOrder(entry);
      mapOrder[input.order_id] = std::make_pair(input.instrument, true);
      // orderBook.printAtomicList();
      break;
    }

    case input_sell:
    {
      // SyncCerr{}
      //     << "Got sell order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
      //     << input.price << " ID: " << input.order_id << std::endl;

      BuySellAtomic *entrySub = new BuySellAtomic{input.count};
      BuySellEntry *entry = new BuySellEntry{input.order_id, input.price, entrySub};
      orderBook.getInstrument(input.instrument)->addSellOrder(entry);
      mapOrder[input.order_id] = std::make_pair(input.instrument, false);
      // orderBook.printAtomicList();
      break;
    }

    default:
    {
      //! Should never happen

      // SyncCerr{}
      //     << "Got order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
      //     << input.price << " ID: " << input.order_id << std::endl;

      // Remember to take timestamp at the appropriate time, or compute
      // an appropriate timestamp!
      auto output_time = getCurrentTimestamp();
      Output::OrderAdded(input.order_id, input.instrument, input.price, input.count, input.type == input_sell,
                         output_time);
      break;
    }
    }
  }
}
