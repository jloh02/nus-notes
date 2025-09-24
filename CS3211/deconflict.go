package submission

import (
	"assign2/utils"
	"assign2/wg"
	"context"
)

type DeconflictRequest struct {
	activeOrder utils.Input
	isBuy       bool

	// isValid is true if the activeOrder can be matched with a restingOrder
	selectedOrderId uint32
	isValid         bool

	engineReqDoneCh chan struct{}
}

func processDeconflictRequest(req DeconflictRequest, restingBuys, restingSells map[uint32]OrderEntry, outCh chan UpdateInstrumentRequest) { //, orderCompleteCh chan bool) {
	orderCompleted := !req.isValid
	if req.isValid {
		var selectedRestingOrder OrderEntry
		var found bool
		if req.isBuy {
			selectedRestingOrder, found = restingSells[req.selectedOrderId]
		} else {
			selectedRestingOrder, found = restingBuys[req.selectedOrderId]
		}

		update := UpdateInstrumentRequest{activeOrderOrderType: req.activeOrder.OrderType}
		usableRestingOrder := found && selectedRestingOrder.Count > 0

		// Cancel handling
		if req.activeOrder.OrderType == utils.InputCancel {
			if usableRestingOrder {
				update.isUpdatingRestingOrders = true
				update.isUpdatingRestingBuys = !req.isBuy
				update.restingOrderKey = req.selectedOrderId
				update.activeOrder.OrderType = utils.InputCancel

				if req.isBuy {
					delete(restingSells, req.selectedOrderId)
				} else {
					delete(restingBuys, req.selectedOrderId)
				}
			}
			utils.OutputOrderDeleted(req.activeOrder, usableRestingOrder, GetCurrentTimestamp())
			outCh <- update
			close(req.engineReqDoneCh)
			return
		}

		if usableRestingOrder {
			transactionCount := min(req.activeOrder.Count, selectedRestingOrder.Count)
			selectedRestingOrder.ExecutionId++
			utils.OutputOrderExecuted(selectedRestingOrder.OrderId, req.activeOrder.OrderId, selectedRestingOrder.ExecutionId, selectedRestingOrder.Price, transactionCount, GetCurrentTimestamp())
			req.activeOrder.Count -= transactionCount
			selectedRestingOrder.Count -= transactionCount

			update.isUpdatingRestingOrders = true
			update.isUpdatingRestingBuys = !req.isBuy

			if req.isBuy {
				if selectedRestingOrder.Count == 0 {
					delete(restingSells, selectedRestingOrder.OrderId)
				} else {
					restingSells[req.selectedOrderId] = selectedRestingOrder
					update.restingOrderValue = selectedRestingOrder
				}

				update.restingOrderKey = req.selectedOrderId
			} else {
				if selectedRestingOrder.Count == 0 {
					delete(restingBuys, selectedRestingOrder.OrderId)
				} else {
					restingBuys[req.selectedOrderId] = selectedRestingOrder
					update.restingOrderValue = selectedRestingOrder
				}

				update.restingOrderKey = req.selectedOrderId
			}
		}

		// Continue execution, send it back first
		if req.activeOrder.Count > 0 {
			update.activeOrder = req.activeOrder
			update.engineReqDoneCh = req.engineReqDoneCh
		} else {
			orderCompleted = true
		}
		outCh <- update
	} else { // No resting order found, add to resting
		timestamp := GetCurrentTimestamp()
		utils.OutputOrderAdded(req.activeOrder, timestamp)
		if req.isBuy {
			restingBuys[req.activeOrder.OrderId] = OrderEntry{
				OrderId:     req.activeOrder.OrderId,
				Price:       req.activeOrder.Price,
				Count:       req.activeOrder.Count,
				TimeAdded:   timestamp,
				ExecutionId: 0,
			}
			outCh <- UpdateInstrumentRequest{
				isUpdatingRestingOrders: true,
				isUpdatingRestingBuys:   true,
				restingOrderKey:         req.activeOrder.OrderId,
				restingOrderValue:       restingBuys[req.activeOrder.OrderId],
				activeOrderOrderType:    req.activeOrder.OrderType,
			}
		} else {
			restingSells[req.activeOrder.OrderId] = OrderEntry{
				OrderId:     req.activeOrder.OrderId,
				Price:       req.activeOrder.Price,
				Count:       req.activeOrder.Count,
				TimeAdded:   timestamp,
				ExecutionId: 0,
			}
			outCh <- UpdateInstrumentRequest{
				isUpdatingRestingOrders: true,
				isUpdatingRestingBuys:   false,
				restingOrderKey:         req.activeOrder.OrderId,
				restingOrderValue:       restingSells[req.activeOrder.OrderId],
				activeOrderOrderType:    req.activeOrder.OrderType,
			}
		}
	}

	if orderCompleted {
		close(req.engineReqDoneCh) // Release for next order
	}

	//orderCompleteCh <- true
}

func CreateDeconflicterPipeline(
	ctx context.Context,
	wg *wg.WaitGroup,
	reqCh <-chan DeconflictRequest,
) chan UpdateInstrumentRequest { //, chan bool) {
	restingBuys := make(map[uint32]OrderEntry)
	restingSells := make(map[uint32]OrderEntry)

	outCh := make(chan UpdateInstrumentRequest)
	//orderCompleteCh := make(chan bool)

	wg.Add(1)
	go func() {
		defer wg.Done()
		defer close(outCh)
		//defer close(orderCompleteCh)

		for {
			select {
			case req := <-reqCh:
				// fmt.Fprintf(os.Stderr, "Received deconflict request:%v\n", req)
				processDeconflictRequest(req, restingBuys, restingSells, outCh) //, orderCompleteCh)
			case <-ctx.Done():
				return
			}
		}
	}()

	return outCh //, orderCompleteCh
}
