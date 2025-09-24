package submission

import "C"
import (
	"assign2/utils"
	"assign2/wg"
	"context"
	"fmt"
	"io"
	"net"
	"os"
	"time"
)

type Engine struct {
	wg     *wg.WaitGroup
	router *Router
}

func (e *Engine) Init(ctx context.Context, wg *wg.WaitGroup) {
	e.wg = wg
	e.router = CreateRouter(ctx, wg)
}

func (e *Engine) Shutdown(ctx context.Context) {
	e.wg.Wait()
}

func (e *Engine) Accept(ctx context.Context, conn net.Conn) {
	e.wg.Add(2)

	go func() {
		defer e.wg.Done()
		<-ctx.Done()
		conn.Close()
	}()

	// This goroutine handles the connection.
	go func() {
		defer e.wg.Done()
		e.handleConn(conn)
	}()
}

func (e *Engine) handleConn(conn net.Conn) {
	defer conn.Close()

	localOrderStore := make(map[uint32]utils.Input)
	for {
		in, err := utils.ReadInput(conn)
		if err != nil {
			if err != io.EOF {
				_, _ = fmt.Fprintf(os.Stderr, "Error reading input: %v\n", err)
			}
			return
		}

		// fmt.Fprintf(os.Stderr, "Got order: %c %v x %v @ %v ID: %v\n", in.OrderType, in.Instrument, in.Count, in.Price, in.OrderId)
		doneCh := make(chan struct{})

		// We need to restore the details of the original order (Specifically the order type and instrument)
		if in.OrderType == utils.InputCancel {
			in = localOrderStore[in.OrderId]
			in.OrderType = utils.InputCancel
		} else {
			localOrderStore[in.OrderId] = utils.Input{
				OrderType:  in.OrderType,
				Instrument: in.Instrument,
				OrderId:    in.OrderId,
			}
		}

		e.router.handleOrder(in, doneCh)
		<-doneCh
	}
}

func GetCurrentTimestamp() int64 {
	return time.Now().UnixNano()
}
