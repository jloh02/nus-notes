## Row Operations
Assuming matrix `A`, syntax is `A(row,col)`, but you can use `:` to represent all rows/cols. Sometimes `*` appears because of unrepresentable floats. Just print using `A(row,col)` - it is usually close to 0
- Swapping `A([2,4],:) = A([4,2],:)`
- Scaling `A(2,:) = 5*A(2,:)`
- Adding `A(2,:) = A(2,:) + 3*A(1,:)`
- Appending columns `[ A x ]`
- Appending rows `[ A; x ]`
### Example
```matlab
format rat
A = [3 2 -4 3; 2 3 3 15; 5 -3 1 14]
```

```matlab
rref(A)
```
## RREF Properties
- Leading non-zero entry must be 1
- Each leading 1 is the only nonzero entry in its column
## Invertibility of Matrices
- If determinant is nonzero
```matlab
det(A)
```

```matlab
adjoint(A)
inv(A)
eye(n) %for identity
```
## LU Decomposition
```matlab
[L, U] = lu(A)
```
## Equations
```
syms a b c d x
solve(a*x^3 + b*x^2 + c*x + d == 1, [a,b,c,d])
solve(det(A) == 0, a) % if a is a free variable in the matrix
```

## Solving linear equations
### mldivide, \
`x = A\B` solves the system of linear equations $Ax = B$. The matrices $A$ and $B$ must have the same number of rows. MATLAB displays a warning message if A is badly scaled or nearly singular, but performs the calculation regardless.
- If $A$ is a scalar, then `A\B` is equivalent to `A.\B`.
- If $A$ is a square n-by-n matrix and B is a matrix with n rows, then `x = A\B` is a solution to the equation $Ax = B$, if it exists.
- If $A$ is a rectangular m-by-n matrix with `m ~= n`, and $B$ is a matrix with m rows, then `A\B` returns a least-squares solution to the system of equations $Ax= B$.

### Solution Sets
When the `rref` of a matrix gives you some infinite solution set, you can easily find the parameterized solution set with `null(sym(Matrix))`.

## QR Decomposition
`[Q R] = qr(A)`: gives full QR factorization
`[Q R] = qr(A, "econ")`: gives the QR we are familiar with in 1522
This is equivalent to 
```matlab
[Q R] = qr(A);
Q = Q(:,1:n)
R = R(1:n,:)
```

## Characteristic Polynomials
`charpoly(Matrix)`: gives the roots of the polynomial
To obtain in factorized form
```matlab
syms x
factor(charpoly(Matrix, x))
```