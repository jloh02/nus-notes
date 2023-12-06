## Question 1
### (i) Compute det(A)

$$
\begin{align}
\text{det}(A) &= a_{11}\text{ det}(A_{11}) - a_{12}\text{ det}(A_{12}) + a_{13}\text{ det}(A_{13}) \\
&= (1*((-7*(s-4)) -(3*6))) - (-2*((4*(s-4)) - (3*-2))) +  (1*((4*6) - (-7*-2))) \\
&= (-7s + 10)  + (8s-20) + 10\\
&= s
\end{align}
$$

### (ii) Determine adj(A)

$$
\begin{align}
\text{adj}(A) &= \begin{pmatrix}
(-7*(s-4)) -(3*6)) & -((-2*(s-4)) -(1*6)) & (-2*3) - (-7*1)\\ 
-((4*(s-4)) - (3*-2)) & (1*(s-4)) - (1*-2) & -((1*3) - (1*4)) \\ 
(4*6) - (-7*-2) & -((1*6) - (-2*-2)) & (1*-7) - (-2*4) \\
\end{pmatrix} \\\\
&= \begin{pmatrix}
10-7s & 2s-2 & 1\\ 
10-4s & s-2 & 1 \\ 
10 & -2 & 1 \\
\end{pmatrix}
\end{align}
$$

### (iii) Determine the value(s) of s such that A is an invertible matrix

$s \neq 0$. A square matrix is invertible only when $det(A) \neq 0$. Since $det(A) = s$, $s\neq 0$

### (iv) Find the inverse matrix $A^{−1}$

$$
\begin{align}
&\left(
\begin{matrix}
1 & -2 & 1 \\ 4 & -7 & 3\\-2 & 6 & s-4
\end{matrix}
\right|
\left.
\begin{matrix}
1 & 0 & 0 \\ 0 & 1 & 0\\0 & 0 & 1
\end{matrix}
\right)
\rightarrow 
\left(
\begin{matrix}
1 & 0 & 0 \\ 0 & 1 & 0\\0 & 0 & 1
\end{matrix}
\right|
\left.
\begin{matrix}
\frac{10-7s}{s} & \frac{2(s-1)}{s} & \frac{1}{s} \\ 
\frac{2(5-2s)}{s} & \frac{s-2}{s} & \frac{1}{s}\\
\frac{10}{s} & -\frac{2}{s} & \frac{1}{s}
\end{matrix}
\right)\\\\
&
A^{-1} = \begin{pmatrix}
\frac{10-7s}{s} & \frac{2(s-1)}{s} & \frac{1}{s} \\ 
\frac{2(5-2s)}{s} & \frac{s-2}{s} & \frac{1}{s}\\
\frac{10}{s} & -\frac{2}{s} & \frac{1}{s}
\end{pmatrix}
\end{align}
$$
<div style="page-break-after: always;"></div>

### (v) Solve the Linear System

$$
\begin{align}
&\begin{aligned}
&\begin{pmatrix}
1 &-2& 1\\4 & -7 & 3\\-2 & 6 & -3
\end{pmatrix}
\begin{pmatrix}
x\\y\\z
\end{pmatrix}
=
\begin{pmatrix}
3\\-2\\1
\end{pmatrix}
\\\\
&\text{Let } s-4 = -3\\
&s = 1\\
&\text{From (i) and (ii),}\\
\end{aligned}\\
&\begin{aligned}
&\text{When }s=1,s-4=3,A = \begin{pmatrix}
1 &-2& 1\\4 & -7 & 3\\-2 & 6 & s-4
\end{pmatrix} = \begin{pmatrix}
1 &-2& 1\\4 & -7 & 3\\-2 & 6 & -3
\end{pmatrix}\\\\
&\text{Let }A = \begin{pmatrix}
1 &-2& 1\\4 & -7 & 3\\-2 & 6 & -3
\end{pmatrix}, x=
\begin{pmatrix}
x\\y\\z
\end{pmatrix}, b=
\begin{pmatrix}
3\\-2\\1
\end{pmatrix}\\\\
&\begin{aligned}
Ax &= b\\
A^{-1}Ax &=A^{-1}b\\
Ix &= A^{-1}b\\
x &= A^{-1}b\\
&=\begin{pmatrix}
\frac{10-7s}{s} & \frac{2(s-1)}{s} & \frac{1}{s} \\ 
\frac{2(5-2s)}{s} & \frac{s-2}{s} & \frac{1}{s}\\
\frac{10}{s} & -\frac{2}{s} & \frac{1}{s}
\end{pmatrix} \begin{pmatrix}
3\\-2\\1
\end{pmatrix} &\text{(From (iv))}\\
&=\begin{pmatrix}
3 & 0 & 1 \\ 
6 & -1 & 1\\
10 & -2 & 1
\end{pmatrix} \begin{pmatrix}
3\\-2\\1
\end{pmatrix}\\
&=\begin{pmatrix}
10\\21\\35
\end{pmatrix}
\end{aligned}\\\\
&\therefore x = 10, y=21, z=35
\end{aligned}
\end{align}
$$
<div style="page-break-after: always;"></div>

## Question 2

### (i) Which vectors belong to V

To find the linear combinations of $v_i$ which form $w_j$, we create an augmented matrix. The resultant coefficients would then represent the coefficients of $v_i$ to form each $w_j$. Inconsistent systems would indicate that $w_j$ cannot be written as a linear combination of $v_i$ and that $w_j$ does not belong to V.

$$
\begin{align}
&\left(\quad
\begin{matrix}
|&|&|\\
v_1 & v_2 & v_3\\
|&|&|\\
\end{matrix}
\quad \right|\left.
\begin{matrix}|\\w_1\\|\end{matrix}
\right|\left.
\begin{matrix}|\\w_2\\|\end{matrix}
\right|\left.
\begin{matrix}|\\w_3\\|\end{matrix}
\right|\left.
\begin{matrix}|\\w_4\\|\end{matrix}
\right)
\rightarrow
\left(\quad
\begin{matrix}
1&0&0\\
0&1&0\\
0&0&1\\
0&0&0\\
\end{matrix}
\quad \right|\left.
\begin{matrix}17/7\\-2/7\\-4/7\\0\end{matrix}
\right|\left.
\begin{matrix}0\\0\\0\\1\end{matrix}
\right|\left.
\begin{matrix}-15/7\\-4/7\\6/7\\0\end{matrix}
\right|\left.
\begin{matrix}631/140\\11/140\\-26/35\\1/20\end{matrix}
\right)\\\\
&\text{Observing the last row, notice that } w_2 \text { and } w_4 \text{ have non-zero last rows, while v columns are zero}\\\\
&\therefore w_1 \text{ and } w_3 \text{ are in V}
\end{align}
$$

### (ii) Write $w_j$ as a linear combination of $v_1,v_2,v_3$

$$
\begin{align}
w_1 &= \frac{17}{7}v_1 - \frac{2}{7} v_2 - \frac{4}{7} v_3\\
w_2 &= -\frac{15}{7}v_1 - \frac{4}{7} v_2 + \frac{6}{7} v_3\\
\end{align}
$$
<div style="page-break-after: always;"></div>

## Question 3

### (iii) Solve the linear system

$$
\begin{align}
&\left(
\begin{matrix}
1&1&0&-1\\
1&2&3&-15\\
2&5&2&-9
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)
\xrightarrow{R_2 - R_1}
\left(
\begin{matrix}
1&1&0&-1\\
0&1&3&-14\\
2&5&2&-9
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)
\xrightarrow{R_3 - 2R_1}
\left(
\begin{matrix}
1&1&0&-1\\
0&1&3&-14\\
0&3&2&-7
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)\\\\
&\qquad\xrightarrow{R_3 - 3R_2}
\left(
\begin{matrix}
1&1&0&-1\\
0&1&3&-14\\
0&0&-7&35
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)
\xrightarrow{-\frac{1}{7}R_3}
\left(
\begin{matrix}
1&1&0&-1\\
0&1&3&-14\\
0&0&1&-5
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)\\\\
&\qquad\xrightarrow{R_2-3R_3}
\left(
\begin{matrix}
1&1&0&-1\\
0&1&0&1\\
0&0&1&-5
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)
\xrightarrow{R_1-R_2}
\left(
\begin{matrix}
1&0&0&-2\\
0&1&0&1\\
0&0&1&-5
\end{matrix}
\right|
\left.
\begin{matrix}
0\\0\\0
\end{matrix}
\right)\\\\
&x_4 = s, x_1 = 2s, x_2 = -s, x_3 = 5s, s\in \mathbb{R}\\\\
\end{align}
$$


### (iv) Write V as a solution of a homogenous linear system

$$
\begin{align}
&\text{By definition of homogenous linear equation, }Ax=0\text{, where }A\text{ is an }m \times n \text{ matrix }\\
&\text{and 0 is the zero vector in }\mathbb{R}^m \\
&A\text{ must be a m}\times\text{4 matrix for }Ax\text{ to be a zero vector}\\\\
&\text{Let }A = \begin{pmatrix}- & a_1 & -\\- & a_2 & -\\- & \vdots & -\end{pmatrix}\\\\
&A\ x = 0 \\
&a_i\ x = 0\\\\
&\text{Since V is the vector space in question 2, } \\
&v_1 \cdot x = v_2 \cdot x = v_3 \cdot x = 0 \\
&\text{which is equivalent to the linear system in 3(iii)}\\
&\therefore a_i = (\text{Solutions to linear system in 3(iii)})^T = \begin{pmatrix}2 \\ -1 \\ 5 \\ 1\end{pmatrix}^T = \begin{pmatrix}2 & -1 & 5 & 1\end{pmatrix} \\\\
&A = \begin{pmatrix}2 & -1 & 5 & 1\end{pmatrix}\\
&V = \left\{x\in \mathbb{R}^4 :\begin{pmatrix}2 & -1 & 5 & 1\end{pmatrix}\ x = 0\right\}
\end{align}
$$