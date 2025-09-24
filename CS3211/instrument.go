package submission

import (
	"assign2/utils"
	"assign2/wg"
	"context"
)

type InstrumentRequest struct {
	order        utils.Input
	outputDoneCh chan struct{}
}

type OrderEntry struct {
	OrderId     uint32
	Price       uint32
	Count       uint32
	TimeAdded   int64
	ExecutionId uint32
}

type UpdateInstrumentRequest struct {
	isUpdatingRestingOrders bool
	isUpdatingRestingBuys   bool
	restingOrderKey         uint32
	restingOrderValue       OrderEntry

	activeOrderOrderType utils.InputType // This field is here in case activeOrder is returned back to queue
	activeOrder          utils.Input

	engineReqDoneCh chan struct{}
}

func copyRestingMap(restingOrders map[uint32]OrderEntry) map[uint32]OrderEntry {
	copiedMap := make(map[uint32]OrderEntry)

	for key, value := range restingOrders {
		copiedMap[key] = value
	}
	return copiedMap
}

func CreateInstrumentPipeline(ctx context.Context, engineWg *wg.WaitGroup) chan InstrumentRequest {
	engineWg.Add(1)

	inputCh := make(chan InstrumentRequest)
	pipelinInCh := make(chan SearchRequest)

	numActiveBuy := 0
	buyOrders := make(map[uint32]OrderEntry)
	buyQueue := make([]InstrumentRequest, 0)

	numActiveSell := 0
	sellOrders := make(map[uint32]OrderEntry)
	sellQueue := make([]InstrumentRequest, 0)

	localOrderIdToType := make(map[uint32]utils.InputType)

	buyTime := 0
	sellTime := 0

	checkBuy := func() {

		if buyTime != 0 && buyTime+10000000 < int(GetCurrentTimestamp()) {
			buyTime = 0
			if numActiveSell > 0 {
				return
			}
		}

		if numActiveSell == 0 && len(buyQueue) > 0 {
			buyTime = int(GetCurrentTimestamp())
			for len(buyQueue) > 0 {
				pipelinInCh <- SearchRequest{
					restingOrders:   copyRestingMap(sellOrders),
					activeOrder:     buyQueue[0].order,
					isBuy:           true,
					engineReqDoneCh: buyQueue[0].outputDoneCh,
				}
				numActiveBuy++
				buyQueue = buyQueue[1:]
			}
		}
	}

	checkSell := func() {

		if sellTime != 0 && sellTime+10000000 < int(GetCurrentTimestamp()) {
			sellTime = 0
			if numActiveBuy > 0 {
				checkBuy()
			}
		}

		if numActiveBuy == 0 && len(sellQueue) > 0 {
			sellTime = int(GetCurrentTimestamp())
			for len(sellQueue) > 0 {
				pipelinInCh <- SearchRequest{
					restingOrders:   copyRestingMap(buyOrders),
					activeOrder:     sellQueue[0].order,
					isBuy:           false,
					engineReqDoneCh: sellQueue[0].outputDoneCh,
				}
				numActiveSell++
				sellQueue = sellQueue[1:]
			}
		}
	}

	checkQueueStatusAndPushPipeline := func() {
		checkBuy()
		checkSell()
	}

	go func() {
		defer engineWg.Done()
		defer close(inputCh)
		defer close(pipelinInCh)

		searchOut := CreateSearcherPipeline(ctx, engineWg, pipelinInCh, 40)
		updateCh := CreateDeconflicterPipeline(ctx, engineWg, searchOut)
		defer close(searchOut)

		for {
			select {
			case req := <-inputCh:
				// fmt.Fprintf(os.Stderr, "Received instrument request:%v\n", req)

				order := req.order

				if order.OrderType != utils.InputCancel {
					localOrderIdToType[order.OrderId] = order.OrderType
				}
				switch order.OrderType {
				case utils.InputBuy:
					buyQueue = append(buyQueue, req)
					checkQueueStatusAndPushPipeline()

				case utils.InputSell:
					sellQueue = append(sellQueue, req)
					checkQueueStatusAndPushPipeline()

				case utils.InputCancel:
					// Submit straight to deconflicter
					orderType := localOrderIdToType[order.OrderId]
					searchOut <- DeconflictRequest{
						activeOrder:     order,
						isBuy:           orderType != utils.InputBuy,
						selectedOrderId: order.OrderId,
						isValid:         true,
						engineReqDoneCh: req.outputDoneCh,
					}
				}

			case update := <-updateCh:
				// fmt.Fprintf(os.Stderr, "Received update request:%v\n", update)

				if update.isUpdatingRestingOrders {
					if update.isUpdatingRestingBuys {
						if update.restingOrderValue != (OrderEntry{}) {
							buyOrders[update.restingOrderKey] = update.restingOrderValue
						} else {
							delete(buyOrders, update.restingOrderKey)
						}
					} else {
						if update.restingOrderValue != (OrderEntry{}) {
							sellOrders[update.restingOrderKey] = update.restingOrderValue
						} else {
							delete(sellOrders, update.restingOrderKey)
						}
					}
				}

				// Return the order back to queue
				if update.activeOrder != (utils.Input{}) {
					// fmt.Fprintf(os.Stderr, "Returning order back to queue: %v\n", update.activeOrder)
					updateReq := InstrumentRequest{
						order:        update.activeOrder,
						outputDoneCh: update.engineReqDoneCh,
					}

					if update.activeOrder.OrderType == utils.InputBuy {
						buyQueue = append(buyQueue, updateReq)
					} else if update.activeOrder.OrderType == utils.InputSell {
						sellQueue = append(sellQueue, updateReq)
					}
				}

				if update.activeOrderOrderType == utils.InputBuy {
					numActiveBuy--
				} else if update.activeOrderOrderType == utils.InputSell {
					numActiveSell--
				}

				checkQueueStatusAndPushPipeline()
			case <-ctx.Done():
				return
			}
		}
	}()

	return inputCh
}
