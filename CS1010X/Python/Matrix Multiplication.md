## ChatGPT Explanation

Let's say we have two matrices, A and B, that we want to multiply:

Matrix A has dimensions m x n (m rows and n columns). Matrix B has dimensions n x p (n rows and p columns).

To multiply these matrices, follow these steps:

1. Check if the number of columns in matrix A is equal to the number of rows in matrix B. If they are not equal, matrix multiplication is not possible.
    
2. Create a new matrix, let's call it C, with dimensions m x p (m rows and p columns). This will be the resulting matrix.
    
3. For each element in matrix C, do the following:
    
    - Take the dot product of the corresponding row in matrix A and the corresponding column in matrix B. This means you multiply each element in the row of matrix A by the corresponding element in the column of matrix B, and then sum up the products.
        
    - Place the result in the corresponding position in matrix C.
        
4. Repeat step 3 for each element in matrix C, filling in the entire matrix.
    

That's it! Matrix C will be the result of the matrix multiplication of A and B.

Note: Matrix multiplication is not commutative, meaning that A * B might not be equal to B * A. Make sure to multiply matrices in the correct order based on their dimensions.

## Sample Python Code

```python
def matrix_multiply(A, B):
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])

    # Check if matrix multiplication is possible
    if cols_A != rows_B:
        print("Matrix multiplication is not possible.")
        return None

    # Create a new matrix to store the result
    C = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Perform matrix multiplication
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                C[i][j] += A[i][k] * B[k][j] #IMPORTANT: This is +=; Instead can store C[i][j] once after each loop of k

    return C

# Example usage
matrix_A = [[1, 2, 3],
            [4, 5, 6]]

matrix_B = [[7, 8],
            [9, 10],
            [11, 12]]

result = matrix_multiply(matrix_A, matrix_B)
```