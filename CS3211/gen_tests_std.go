package main

import (
	"fmt"
	"math/rand"
	"strings"
)

const (
	NUM_ORDERS                 = 30000 * 500
	NUM_THREADS                = 40
	PROBABILITY_NEW_CONNECTION = 0.1
	PROBABILITY_ORDER          = 0.9
	PROBABILITY_WAIT           = 0.05
	PROBABILITY_CANCEL         = 0.05
)

var INSTRUMENTS []string

func generateInstruments(totalInstruments int) {
	chosen := make(map[string]bool)
	i := 0
	for i < totalInstruments {
		symbol := randomString(4)
		if !chosen[symbol] {
			chosen[symbol] = true
			INSTRUMENTS = append(INSTRUMENTS, symbol)
			i++
		}
	}
}

func randomString(length int) string {
	letters := "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	var sb strings.Builder
	for i := 0; i < length; i++ {
		sb.WriteByte(letters[rand.Intn(len(letters))])
	}
	return sb.String()
}

func generateTestcase() {
	numThreads := NUM_THREADS
	fmt.Println(numThreads)
	fmt.Println("0 o")

	threadWaitingConnect := make([]int, 0, numThreads-1)
	threadConnected := []int{0}

	for i := 1; i < numThreads; i++ {
		threadWaitingConnect = append(threadWaitingConnect, i)
	}

	seq := 0
	orders := make([][2]int, 0)

	for i := 0; i < NUM_ORDERS; i++ {
		if len(threadWaitingConnect) > 0 && rand.Float64() < PROBABILITY_NEW_CONNECTION {
			idx := rand.Intn(len(threadWaitingConnect))
			threadToConnect := threadWaitingConnect[idx]
			threadConnected = append(threadConnected, threadToConnect)
			threadWaitingConnect = append(threadWaitingConnect[:idx], threadWaitingConnect[idx+1:]...)
			fmt.Printf("%d o\n", threadToConnect)
		}

		action := weightedRandom([]string{"order", "wait", "cancel"}, []float64{PROBABILITY_ORDER, PROBABILITY_WAIT, PROBABILITY_CANCEL})

		switch action {
		case "order":
			threadID := threadConnected[rand.Intn(len(threadConnected))]
			orderID := seq
			symbol := INSTRUMENTS[rand.Intn(len(INSTRUMENTS))]
			price := (rand.Intn(51) + 50) * 10
			count := rand.Intn(5000) + 1
			orderType := "B"
			if rand.Intn(2) == 1 {
				orderType = "S"
			}
			fmt.Printf("%d %s %d %s %d %d\n", threadID, orderType, orderID, symbol, price, count)
			orders = append(orders, [2]int{threadID, orderID})
			seq++
		case "wait":
			if len(orders) > 0 {
				idx := rand.Intn(len(orders))
				fmt.Printf("%d w %d\n", orders[idx][0], orders[idx][1])
				orders = append(orders[:idx], orders[idx+1:]...)
			}
		case "cancel":
			if len(orders) > 0 {
				idx := rand.Intn(len(orders))
				fmt.Printf("%d C %d\n", orders[idx][0], orders[idx][1])
				orders = append(orders[:idx], orders[idx+1:]...)
			}
		}
	}

	fmt.Println(".")
	fmt.Printf("0-%d x\n", numThreads-1)
}

func weightedRandom(choices []string, weights []float64) string {
	sum := 0.0
	for _, weight := range weights {
		sum += weight
	}

	r := rand.Float64() * sum
	sum = 0.0
	for i, weight := range weights {
		sum += weight
		if r < sum {
			return choices[i]
		}
	}
	return choices[len(choices)-1]
}

func main() {
	generateInstruments(500)
	generateTestcase()
}
