# Tower of Hanoi

```python
def tower_of_hanoi(n, source, auxiliary, destination):
    if n == 1:
        print(f"Move disk 1 from {source} to {destination}")
        return

    tower_of_hanoi(n-1, source, destination, auxiliary)
    print(f"Move disk {n} from {source} to {destination}")
    tower_of_hanoi(n-1, auxiliary, source, destination)

# Example usage
n = 3  # Number of disks
tower_of_hanoi(n, 'A', 'B', 'C')
```

# N Towers of Hanoi

```python
def tower_of_hanoi(n, source, auxiliary, destination, memo):
    if n == 1:
        print(f"Move disk 1 from {source} to {destination}")
        return

    if (n, source, auxiliary, destination) in memo:
        return memo[(n, source, auxiliary, destination)]

    tower_of_hanoi(n-1, source, destination, auxiliary, memo)
    print(f"Move disk {n} from {source} to {destination}")
    tower_of_hanoi(n-1, auxiliary, source, destination, memo)

    memo[(n, source, auxiliary, destination)] = True

def solve_n_towers_of_hanoi(n, sources, auxiliaries, destinations):
    memo = {}
    for i in range(n):
        print(f"Solving Tower of Hanoi {i+1}:")
        tower_of_hanoi(len(sources[i]), sources[i], auxiliaries[i], destinations[i], memo)
        print()  # Add an empty line between towers

# Example usage
n = 3  # Number of towers
sources = [['A', 'B', 'C'], ['D', 'E'], ['F', 'G', 'H']]
auxiliaries = [['X', 'Y'], [], ['I', 'J']]
destinations = [['P', 'Q', 'R'], ['K', 'L'], ['M', 'N', 'O']]

solve_n_towers_of_hanoi(n, sources, auxiliaries, destinations)
```

# Knapsack 

```python
def knapsack_dp(weights, values, capacity):
    n = len(weights)
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        for j in range(1, capacity + 1):
            if weights[i - 1] <= j:
                dp[i][j] = max(values[i - 1] + dp[i - 1][j - weights[i - 1]], dp[i - 1][j])
            else:
                dp[i][j] = dp[i - 1][j]

    return dp[n][capacity]
```

# Travelling Salesman Problem

```python
def tsp_dp(distances):
    n = len(distances)
    dp = [[float('inf')] * n for _ in range(2**n)]

    dp[1][0] = 0

    for mask in range(1, 2**n):
        for j in range(1, n):
            if (mask >> j) & 1:
                for k in range(n):
                    if k != j and (mask >> k) & 1:
                        dp[mask][j] = min(dp[mask][j], distances[j][k] + dp[mask ^ (1 << j)][k])

    return min(dp[2**n - 1][j] + distances[j][0] for j in range(1, n))
```

# Longest Common Subsequence 

```python
def lcs_dp(text1, text2):
    m, n = len(text1), len(text2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    return dp[m][n]
```

# Matrix Chain Multiplication

```python
def matrix_chain_mult_dp(dimensions):
    n = len(dimensions)
    dp = [[float('inf')] * n for _ in range(n)]

    for i in range(n):
        dp[i][i] = 0

    for l in range(2, n + 1):
        for i in range(n - l + 1):
            j = i + l - 1
            for k in range(i, j):
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + dimensions[i][0] * dimensions[k][1] * dimensions[j][1])

    return dp[0][n - 1]
```

# Subset Sum

```python
def subset_sum_dp(nums, target):
    n = len(nums)
    dp = [[False] * (target + 1) for _ in range(n + 1)]

    for i in range(n + 1):
        dp[i][0] = True

    for i in range(1, n + 1):
        for j in range(1, target + 1):
            if nums[i - 1] <= j:
                dp[i][j] = dp[i - 1][j] or dp[i - 1][j - nums[i - 1]]
            else:
                dp[i][j] = dp[i - 1][j]

    return dp[n][target]
```

# Maximum Subarray Sum

```python
def max_subarray_sum_dp(nums):
    n = len(nums)
    dp = [0] * n
    dp[0] = nums[0]
    max_sum = nums[0]

    for i in range(1, n):
        dp[i] = max(nums[i], dp[i - 1] + nums[i])
        max_sum = max(max_sum, dp[i])

    return max_sum
```

# Coin Change Problem

```python
def coin_change_dp(coins, amount):
    dp = [float('inf')] * (amount + 1)
    dp[0] = 0

    for coin in coins:
        for i in range(coin, amount + 1):
            dp[i] = min(dp[i], dp[i - coin] + 1)

    return dp[amount] if dp[amount] != float('inf') else -1
```

# Fibonacci Sequence

```python
def fibonacci_dp(n):
    if n <= 1:
        return n

    dp = [0] * (n + 1)
    dp[1] = 1

    for i in range(2, n + 1):
        dp[i] = dp[i - 1] + dp[i - 2]

    return dp[n]
```

# Maximum Weighted Independent Set

```python
def max_weighted_independent_set_dp(weights):
    n = len(weights)
    dp = [0] * (n + 1)

    dp[1] = weights[0]

    for i in range(2, n + 1):
        dp[i] = max(weights[i - 1] + dp[i - 2], dp[i - 1])

    return dp[n]
```



