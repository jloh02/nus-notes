Also Big O notation

## Definitions
Theta Notation:
$$f(n) = Θ(g(n)) ⇔ ∃k1, k2, n0 . k1 · g(n) ≤ f(n) ≤ k2 · g(n), for n > n0$$
Big O Notation:
$$f(n) = O(g(n)) ⇔ ∃k, n0 . f(n) ≤ k · g(n), for n > n0$$
To disprove $f(n) = O(g(n))$ or to prove $f(n) = Θ(g(n))$, pick $k_1$, $k_2$ and $n_0$ and pick a value of $n$ which does not satisfy $k1 · g(n) ≤ f(n) ≤ k2 · g(n)$

## General Space Complexity
- In iterative: Size of array used
- In recursive: Number of function calls

## Increasing Order of Inefficiency
- O(1)
- O(log n)
- O(n)
- O(n log n)
- O(n^2)
- O(n^3)
- O(2^n)