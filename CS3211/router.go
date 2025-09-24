package submission

import (
	"assign2/utils"
	"assign2/wg"
	"context"
)

type Router struct {
	readCh            chan RouterRequest
	instrumentEntries map[string]chan InstrumentRequest
	ctx               context.Context
	wg                *wg.WaitGroup
}

type RouterRequest struct {
	order        utils.Input
	outputDoneCh chan struct{}
}

func CreateRouter(ctx context.Context, engineWg *wg.WaitGroup) *Router {
	router := Router{
		readCh:            make(chan RouterRequest),
		instrumentEntries: make(map[string]chan InstrumentRequest),
		ctx:               ctx,
		wg:                engineWg,
	}
	go router.manageRouter()
	return &router
}

func (router *Router) manageRouter() {
	router.wg.Add(1)
	defer router.wg.Done()
	defer close(router.readCh)
	for {
		select {
		case request := <-router.readCh:
			entry, found := router.instrumentEntries[request.order.Instrument]
			if found {
				entry <- InstrumentRequest(request)
			} else {
				instrument := CreateInstrumentPipeline(router.ctx, router.wg)
				router.instrumentEntries[request.order.Instrument] = instrument
				instrument <- InstrumentRequest(request)
			}
			// fmt.Fprintf(os.Stderr, "Router: %v\n", router.instrumentEntries)
		case <-router.ctx.Done():
			return
		}
	}
}

func (router *Router) handleOrder(order utils.Input, doneCh chan struct{}) {
	router.readCh <- RouterRequest{order: order, outputDoneCh: doneCh}
}
