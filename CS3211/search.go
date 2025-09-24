package submission

import (
	"assign2/utils"
	"assign2/wg"
	"context"
)

type SearchRequest struct {
	restingOrders map[uint32]OrderEntry
	activeOrder   utils.Input
	isBuy         bool

	engineReqDoneCh chan struct{}
}

func isValidPriceCompare(activePrice uint32, listPrice uint32, isBuy bool) bool {
	if isBuy {
		return activePrice >= listPrice
	} else {
		return activePrice <= listPrice
	}
}

func isBetterOrder(order1, order2 *OrderEntry, isBuy bool) bool {
	if order1.Price == order2.Price {
		return order1.TimeAdded < order2.TimeAdded
	}

	if isBuy {
		return order1.Price < order2.Price
	} else {
		return order1.Price > order2.Price
	}
}

func search(request SearchRequest) DeconflictRequest {
	var bestOrder *OrderEntry
	var bestOrderId uint32

	// fmt.Fprintf(os.Stderr, "Received search request:%v\n", request)

	for _, currentOrder := range request.restingOrders {
		if isValidPriceCompare(request.activeOrder.Price, currentOrder.Price, request.isBuy) {
			if bestOrder == nil || isBetterOrder(&currentOrder, bestOrder, request.isBuy) {
				bestOrder = &currentOrder
				bestOrderId = currentOrder.OrderId
			}
		}
	}

	return DeconflictRequest{
		activeOrder:     request.activeOrder,
		isBuy:           request.isBuy,
		selectedOrderId: bestOrderId,
		isValid:         bestOrder != nil,
		engineReqDoneCh: request.engineReqDoneCh,
	}
}

func CreateSearcherPipeline(
	ctx context.Context,
	wg *wg.WaitGroup,
	reqCh <-chan SearchRequest,
	instances int,
) chan DeconflictRequest {
	wg.Add(instances)
	outCh := make(chan DeconflictRequest, instances)

	for i := 0; i < instances; i++ {
		go func() {
			defer wg.Done()

			for {
				select {
				case req := <-reqCh:
					outCh <- search(req)
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	return outCh
}
