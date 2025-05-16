import random
import os

NUM_TESTCASES = 10
NUM_ORDERS = 1000

NUM_THREADS = 40

PROBABILITY_NEW_CONNECTION = 0.1
PROBABILITY_ORDER = 0.9
PROBABILITY_WAIT = 0.05
PROBABILITY_CANCEL = 0.05


def generate_testcase():
    num_threads = NUM_THREADS
    orders = []

    # Start building the testcase input
    testcase = [f"{num_threads}\n"]

    testcase.append(f"0 o\n")

    thread_waiting_connect = list(range(1, num_threads))
    thread_connected = [0]

    seq = 0
    # Generate random actions (buy/sell/wait/cancel)
    for _ in range(NUM_ORDERS):
        if (
            len(thread_waiting_connect) > 0
            and random.random() < PROBABILITY_NEW_CONNECTION
        ):
            thread_to_connect = random.choice(thread_waiting_connect)
            thread_connected.append(thread_to_connect)
            thread_waiting_connect.remove(thread_to_connect)
            testcase.append(f"{thread_to_connect} o\n")

        action = random.choices(
            ["order", "wait", "cancel"],
            weights=[PROBABILITY_ORDER, PROBABILITY_WAIT, PROBABILITY_CANCEL],
        )[0]

        if action == "order":
            thread_id = random.choice(thread_connected)
            order_id = seq
            symbol = random.choice(["GOOG", "AAPL", "MSFT", "AMZN"])
            price = random.randint(50, 100) * 10  # Prices between 500 and 1000
            count = random.randint(1, 5000)
            order_type = random.choice(["B", "S"])  # Buy or Sell
            testcase.append(
                f"{thread_id} {order_type} {order_id} {symbol} {price} {count}\n"
            )
            seq += 1
            orders.append((thread_id, order_id))
        elif (
            action == "wait" and orders
        ):  # Ensure there's at least one order to wait on
            thread_id, order_id = random.choice(orders)
            testcase.append(f"{thread_id} w {order_id}\n")
            orders.remove((thread_id, order_id))
        elif (
            action == "cancel" and orders
        ):  # Ensure there's at least one order to cancel
            cancel_thread, order_id = random.choice(orders)
            testcase.append(f"{cancel_thread} C {order_id}\n")
            orders.remove((cancel_thread, order_id))

    # Sync all threads before cancellations
    testcase.append(".\n")

    testcase.append(f"0-{num_threads-1} x\n")

    return "".join(testcase)


def main():
    os.makedirs("tests", exist_ok=True)
    for i in range(NUM_TESTCASES):
        testcase = generate_testcase()
        with open(f"tests/testcase{i}.in", "w", newline="\n") as f:
            f.write(testcase)
        print("Generated", f"tests/testcase{i}.in")


if __name__ == "__main__":
    main()
