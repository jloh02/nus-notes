## Matrix Transposition
Pseudo "In Place"
```python
def transpose_matrix(matrix):
    rows = len(matrix)
    cols = len(matrix[0])

    # Create a new matrix with transposed dimensions
    transposed_matrix = [[0 for _ in range(rows)] for _ in range(cols)]

    # Transpose the matrix by swapping elements
    for i in range(rows):
        for j in range(cols):
            transposed_matrix[j][i] = matrix[i][j]

    # Update the original matrix with transposed values
    matrix.clear()
    matrix.extend(transposed_matrix)
```

    
## In-Place Transposition
To perform an in-place transpose of a matrix in Python, you can use the built-in zip() function along with list comprehension. Here's an example:

```python
def transpose(matrix):
    matrix[:] = [list(row) for row in zip(*matrix)]

# Example usage
original_matrix = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

transpose(original_matrix)

# Verify the transposed matrix
for row in original_matrix:
    print(row)
```
In this example, the transpose() function takes a matrix as input and modifies it in-place. It uses list comprehension along with zip(*matrix) to transpose the matrix. The zip() function takes the elements from each row of the matrix as separate arguments, and the * operator unpacks those arguments as individual row elements. Finally, list() is used to convert the transposed rows back into lists, and matrix[:] is used to update the original matrix in-place.


## Not In-place

```python
def transpose(matrix):
    n = len(matrix)
    m = len(matrix[0])

    transposed_matrix = [[0] * n for _ in range(m)]

    for i in range(n):
        for j in range(m):
            transposed_matrix[j][i] = matrix[i][j]

    matrix[:] = transposed_matrix

# Example usage
original_matrix = [
    [1, 2, 3],
    [4, 5, 6]
]

transpose(original_matrix)

# Verify the transposed matrix
for row in original_matrix:
    print(row)
```