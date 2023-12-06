## Question 1
### (i) Write down the augmented matrix of the above linear system

$$
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		3 & 4 & 2 \\
		2 & 3 & -1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b \\ 
		1
	\end{matrix}
	\right. 
\right)
$$
### (ii)(iii)(iv)

$$ 
\begin{align}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		3 & 4 & 2 \\
		2 & 3 & -1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b \\ 
		1
	\end{matrix}
	\right. 
\right)
&\xrightarrow{R_2 - 3R_1}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 3a-7 \\
		2 & 3 & -1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6 \\ 
		1
	\end{matrix}
	\right. 
\right) 
\\ \\
&\xrightarrow{R_3 - 2R_1}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 3a-7 \\
		0 & 1 & 2a-7
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6 \\ 
		-3
	\end{matrix}
	\right. 
\right)
\\ \\
&\xrightarrow{R_3 - R_2}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 3a-7 \\
		0 & 0 & -a
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6 \\ 
		3-b
	\end{matrix}
	\right. 
\right)
\\ \\
&\text{Case 1: } a = 0
\\ \\
&\xrightarrow{}
\left(
	\begin{matrix}
		1 & 1 & 3 \\
		0 & 1 & -7 \\
		0 & 0 & 0
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6 \\ 
		3-b
	\end{matrix}
	\right. 
\right)
\\ \\
&\qquad \quad \begin{aligned}
	&\text{Subcase 1a: } b = 3 
	\\ \\
	&\xrightarrow{}
	\left(
		\begin{matrix}
			1 & 1 & 3 \\
			0 & 1 & -7 \\
			0 & 0 & 0
		\end{matrix} 
		\left| 
		\begin{matrix}
			2 \\ 
			-3 \\ 
			0
		\end{matrix}
		\right. 
	\right)
	\xrightarrow{R_1-R_2}
	\left(
		\begin{matrix}
			1 & 0 & 10 \\
			0 & 1 & -7 \\
			0 & 0 & 0
		\end{matrix} 
		\left| 
		\begin{matrix}
			5 \\ 
			-3 \\ 
			0
		\end{matrix}
		\right. 
	\right) 
	\\ \\ 
	&z=s,\ y = -3 + 7s,\ x = 5 - 10s,\ s \in \mathbb{R} 
	\\
	&\therefore \text{Infinite solutions with free variable } s 
	\\ \\
	&\text{Subcase 1b: } b \neq 3
	\\ \\
	&\xrightarrow{}
	\left(
		\begin{matrix}
			1 & 1 & 3 \\
			0 & 1 & -7 \\
			0 & 0 & 0
		\end{matrix} 
		\left| 
		\begin{matrix}
			2 \\ 
			b-6 \\ 
			3-b
		\end{matrix}
		\right. 
	\right)
	\\ \\
	&\text{Since }b\neq 3,\ 3-b \neq 0 \\ 
	&\therefore \text{No solution}
	\\ \\
\end{aligned}
\\ \\
&\text{Case 2: } a \neq 0
\\ \\
&\xrightarrow{-\frac{1}{a}R_3}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 3a-7 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6 \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\xrightarrow{R_2+(7-3a)R_3}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 3a-7 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		b-6-(3a-7)*\frac{b-3}{a} \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\\ \\ 
&\xrightarrow{}
\left(
	\begin{matrix}
		1 & 1 & 3-a \\
		0 & 1 & 0 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 \\ 
		-2b + \frac{7b}{a} - \frac{21}{a} + 3 \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\xrightarrow{R_1+(a-3)R_3}
\left(
	\begin{matrix}
		1 & 1 & 0 \\
		0 & 1 & 0 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		2 + (a-3)(\frac{b}{a}-\frac{3}{a}) \\ 
		-2b + \frac{7b}{a} - \frac{21}{a} + 3 \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\\ \\
&\xrightarrow{}
\left(
	\begin{matrix}
		1 & 1 & 0 \\
		0 & 1 & 0 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		-1 + \frac{9}{a} + b - \frac{3b}{a} \\ 
		-2b + \frac{7b}{a} - \frac{21}{a} + 3 \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\xrightarrow{R_1-R_2}
\left(
	\begin{matrix}
		1 & 0 & 0 \\
		0 & 1 & 0 \\
		0 & 0 & 1
	\end{matrix} 
	\left| 
	\begin{matrix}
		-4 + \frac{30}{a} + 3b - \frac{10b}{a} \\ 
		-2b + \frac{7b}{a} - \frac{21}{a} + 3 \\ 
		\frac{b-3}{a}
	\end{matrix}
	\right. 
\right)
\\ \\
&\therefore x = -4 + \frac{30}{a} + 3b - \frac{10b}{a},\ y = -2b + \frac{7b}{a} - \frac{21}{a} + 3,\ z = \frac{b-3}{a}
\end{align}
$$

### (ii) Determine the values of a and b in which the above system has no solution
$$
a = 0,b \neq 3
$$
### (iii) Determine the values of a and b in which the above system has infinitely many  solutions, and find the general solution
$$
\begin{align}
a&=0,b=3\\
z&=s,\ y = -3 + 7s,\ x = 5 - 10s,\ s \in \mathbb{R}
\end{align}
$$
### (iv) Determine the values of a and b in which the above system has a unique solution.  What is the value of z for this unique solution?
$$
\begin{align}
a &\neq 0, b\in \mathbb{R}\\
z &= \frac{b-3}{a}
\end{align}
$$

## Question 2
### (i) Compute $B$
$$
\begin{align}
&\left(
	\begin{matrix}
		1 & 0 & 2 \\
		2 & -1 & 3 \\
		4 & 1 & 6
	\end{matrix}
\right)
\xrightarrow{R_2-2R_1}
\left(
	\begin{matrix}
		1 & 0 & 2 \\
		0 & -1 & -1 \\
		4 & 1 & 6
	\end{matrix}
\right)
\xrightarrow{R_3-4R_1}
\left(
	\begin{matrix}
		1 & 0 & 2 \\
		0 & -1 & -1 \\
		0 & 1 & -2
	\end{matrix}
\right)
\\ \\
&\xrightarrow{-R_2}
\left(
	\begin{matrix}
		1 & 0 & 2 \\
		0 & 1 & 1 \\
		0 & 1 & -2
	\end{matrix}
\right)
\xrightarrow{R_3-R_2}
\left(
	\begin{matrix}
		1 & 0 & 2 \\
		0 & 1 & 1 \\
		0 & 0 & -3
	\end{matrix}
\right)
\xrightarrow{-\frac{1}{3}R_3}
\left(
	\begin{matrix}
		1 & 0 & 2 \\
		0 & 1 & 1 \\
		0 & 0 & 1
	\end{matrix}
\right) = B
\end{align}
$$

### (ii) Compute RREF from $B$, $R$
$$
\begin{align}
\begin{pmatrix}
	1 & 0 & 2 \\
	0 & 1 & 1 \\
	0 & 0 & 1
\end{pmatrix}
\xrightarrow{R_2-R_3}
\begin{pmatrix}
	1 & 0 & 2 \\
	0 & 1 & 0 \\
	0 & 0 & 1
\end{pmatrix}
\xrightarrow{R_1-2R_3}
\begin{pmatrix}
	1 & 0 & 0 \\
	0 & 1 & 0 \\
	0 & 0 & 1
\end{pmatrix}
\end{align}
$$

### (iii) Using (i) and (ii), write R as a product of elementary matrices, ie $R = E_kE_{k-1}...E_1A$

$$
\begin{align}
	R &= E_kE_{k-1}...E_1A \\
	&= E_kE_{k-1}...E_1
	\begin{pmatrix}
	1 & 0 & 2 \\ 2 & -1 & 3 \\ 4 & 1 & 6
	\end{pmatrix} \\
	&=
	\begin{pmatrix}
	1 & 0 & -2 \\ 0 & 1 & 0 \\ 0 & 0 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ 0 & 1 & -1 \\ 0 & 0 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ 0 & 1 & 0 \\ 0 & 0 & -1/3
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ 0 & 1 & 0 \\ 0 & -1 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ 0 & -1 & 0 \\ 0 & 0 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ 0 & 1 & 0 \\ -4 & 0 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 0 \\ -2 & 1 & 0 \\ 0 & 0 & 1
	\end{pmatrix}
	\begin{pmatrix}
	1 & 0 & 2 \\ 2 & -1 & 3 \\ 4 & 1 & 6
	\end{pmatrix}
\end{align}
$$

### (iv) Using (ii) or otherwise, explain why A is an invertible matrix

From (ii), since the RREF of $A=I_3$, where $I_3$ is a 3x3 identity matrix, $A \sim I_3$ . Because $A$ is row equivalent to $I_3$, there exists a 3x3 matrix $C$, where $CA = I$, represented by $E_kE_{k-1}...E_1$ . $C$ is the inverse matrix $A^{-1}$. Thus, $A$ is an invertible matrix
### (v) Compute the inverse matrix $A^{-1}$

$$
\begin{align}
&\text{Since }R = I_3 = A^{-1}A \text{, using the same row operations, } E_kE_{k-1}...E_1\text{, on } I_3 \text{ results in } A^{-1}  \\ \\
&\begin{pmatrix}
1 & 0 & 0 \\ 0 & 1 & 0 \\ 0 & 0 & 1
\end{pmatrix}
\xrightarrow{R_2-2R_1}
\begin{pmatrix}
1 & 0 & 0 \\ -2 & 1 & 0 \\ 0 & 0 & 1
\end{pmatrix}
\xrightarrow{R_3-4R_1}
\begin{pmatrix}
1 & 0 & 0 \\ -2 & 1 & 0 \\ -4 & 0 & 1
\end{pmatrix}
\xrightarrow{-R_2}
\begin{pmatrix}
1 & 0 & 0 \\ 2 & -1 & 0 \\ -4 & 0 & 1
\end{pmatrix}
\\
&\xrightarrow{R_3-R_2}
\begin{pmatrix}
1 & 0 & 0 \\ 2 & -1 & 0 \\ -6 & 1 & 1
\end{pmatrix}
\xrightarrow{-\frac{1}{3}R_3}
\begin{pmatrix}
1 & 0 & 0 \\ 2 & -1 & 0 \\ 2 & -\frac{1}{3} & -\frac{1}{3}
\end{pmatrix}
\xrightarrow{R_2-R_3}
\begin{pmatrix}
1 & 0 & 0 \\ 0 & -\frac{2}{3} & \frac{1}{3} \\ 2 & -\frac{1}{3} & -\frac{1}{3}
\end{pmatrix}
\xrightarrow{R_1-2R_3}
\begin{pmatrix}
-3 & \frac{2}{3} & \frac{2}{3} \\ 0 & -\frac{2}{3} & \frac{1}{3} \\ 2 & -\frac{1}{3} & -\frac{1}{3}
\end{pmatrix}
\\ \\
&A^{-1} = 
\begin{pmatrix}
-3 & \frac{2}{3} & \frac{2}{3} \\ 0 & -\frac{2}{3} & \frac{1}{3} \\ 2 & -\frac{1}{3} & -\frac{1}{3}
\end{pmatrix}
\end{align}
$$
<div style="page-break-after: always;"></div>

## Question 3

$$
\begin{align}
\text{Since}&\text{ we are only interested in finding }C_{21}\text{, ignore all other values and calculate for }c_{31},c_{32},c_{41},c_{42}
\\ \\
C &= XY = AB =
\begin{pmatrix}
c_{11} & c_{12} & c_{13} & c_{14} & c_{15} & c_{16}\\
c_{21} & c_{22} & c_{23} & c_{24} & c_{25} & c_{26}\\
c_{31} & c_{32} & c_{33} & c_{34} & c_{35} & c_{36}\\
c_{41} & c_{42} & c_{43} & c_{44} & c_{45} & c_{46}\\
c_{51} & c_{52} & c_{53} & c_{54} & c_{55} & c_{56}\\
c_{61} & c_{62} & c_{63} & c_{64} & c_{65} & c_{66}\\
\end{pmatrix}
\\
&=
\begin{pmatrix}
a_{11} & a_{12} & a_{13} & a_{14} & a_{15} & a_{16}\\
a_{21} & a_{22} & a_{23} & a_{24} & a_{25} & a_{26}\\
a_{31} & a_{32} & a_{33} & a_{34} & a_{35} & a_{36}\\
a_{41} & a_{42} & a_{43} & a_{44} & a_{45} & a_{46}\\
a_{51} & a_{52} & a_{53} & a_{54} & a_{55} & a_{56}\\
a_{61} & a_{62} & a_{63} & a_{64} & a_{65} & a_{66}\\
\end{pmatrix}
\begin{pmatrix}
b_{11} & b_{12} & b_{13} & b_{14} & b_{15} & b_{16}\\
b_{21} & b_{22} & b_{23} & b_{24} & b_{25} & b_{26}\\
b_{31} & b_{32} & b_{33} & b_{34} & b_{35} & b_{36}\\
b_{41} & b_{42} & b_{43} & b_{44} & b_{45} & b_{46}\\
b_{51} & b_{52} & b_{53} & b_{54} & b_{55} & b_{56}\\
b_{61} & b_{62} & b_{63} & b_{64} & b_{65} & b_{66}\\
\end{pmatrix}
\\
&= 
\begin{pmatrix}
*&*&*&*&*&*\\
*&*&*&*&*&*\\
a_{31}b_{11} + a_{32}b_{21} + a_{33}b_{31} + a_{34}b_{41} + a_{35}b_{51} + a_{36}b_{61} & 
a_{31}b_{12} + a_{32}b_{22} + a_{33}b_{32} + a_{34}b_{42} + a_{35}b_{52} + a_{36}b_{62}
&*&*&*&*\\
a_{41}b_{11} + a_{42}b_{21} + a_{43}b_{31} + a_{44}b_{41} + a_{45}b_{51} + a_{46}b_{61} &
a_{41}b_{12} + a_{42}b_{22} + a_{43}b_{32} + a_{44}b_{42} + a_{45}b_{52} + a_{46}b_{62} &*&*&*&*\\
*&*&*&*&*&*\\
*&*&*&*&*&*\\
\end{pmatrix}
\\ \\
C_{21} &= 
\begin{pmatrix}a_{31}b_{11} + a_{32}b_{21} + a_{33}b_{31} + a_{34}b_{41} + a_{35}b_{51} + a_{36}b_{61} & 
a_{31}b_{12} + a_{32}b_{22} + a_{33}b_{32} + a_{34}b_{42} + a_{35}b_{52} + a_{36}b_{62}
\\
a_{41}b_{11} + a_{42}b_{21} + a_{43}b_{31} + a_{44}b_{41} + a_{45}b_{51} + a_{46}b_{61} &
a_{41}b_{12} + a_{42}b_{22} + a_{43}b_{32} + a_{44}b_{42} + a_{45}b_{52} + a_{46}b_{62} \\
\end{pmatrix}
\\ \\
&=
\begin{pmatrix}
a_{31}b_{11} + a_{32}b_{21}  & 
a_{31}b_{12} + a_{32}b_{22} \\
a_{41}b_{11} + a_{42}b_{21} &
a_{41}b_{12} + a_{42}b_{22} \\
\end{pmatrix}
+
\begin{pmatrix}
a_{33}b_{31} + a_{34}b_{41}  & 
a_{33}b_{32} + a_{34}b_{42} \\
a_{43}b_{31} + a_{44}b_{41} &
a_{43}b_{32} + a_{44}b_{42} \\
\end{pmatrix}
+
\begin{pmatrix}
a_{35}b_{51} + a_{36}b_{61}  & 
a_{35}b_{52} + a_{36}b_{62} \\
a_{45}b_{51} + a_{46}b_{61} &
a_{45}b_{52} + a_{46}b_{62} \\
\end{pmatrix}
\\ \\
&=
\begin{pmatrix}
	| & | \\
	\begin{pmatrix}
	a_{31}&a_{32}\\
	a_{41}&a_{42}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{11}\\b_{21}
	\end{pmatrix} &
	\begin{pmatrix}
	a_{31}&a_{32}\\
	a_{41}&a_{42}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{12}\\b_{22}
	\end{pmatrix} \\
	| & | \\
\end{pmatrix}
\\
\quad&+
\begin{pmatrix}
	| & | \\
	\begin{pmatrix}
	a_{33}&a_{34}\\
	a_{43}&a_{44}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{31}\\b_{41}
	\end{pmatrix} &
	\begin{pmatrix}
	a_{33}&a_{34}\\
	a_{43}&a_{44}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{32}\\b_{42}
	\end{pmatrix} \\
	| & | \\
\end{pmatrix}
\\
\quad&+
\begin{pmatrix}
	| & | \\
	\begin{pmatrix}
	a_{35}&a_{36}\\
	a_{45}&a_{46}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{51}\\b_{61}
	\end{pmatrix} &
	\begin{pmatrix}
	a_{35}&a_{36}\\
	a_{45}&a_{46}
	\end{pmatrix} 
	\begin{pmatrix}
	b_{52}\\b_{62}
	\end{pmatrix} \\
	| & | \\
\end{pmatrix}
\\ \\
&=
\begin{pmatrix}
	a_{31}&a_{32}\\
	a_{41}&a_{42}
\end{pmatrix} 
\begin{pmatrix}
	b_{11}&b_{12}\\
	b_{21}&b_{22}
\end{pmatrix} 	
+
\begin{pmatrix}
	a_{33}&a_{34}\\
	a_{43}&a_{44}
	\end{pmatrix} 
\begin{pmatrix}
	b_{31}&b_{32}\\
	b_{41}&b_{42}
\end{pmatrix} 
+
\begin{pmatrix}
	a_{35}&a_{36}\\
	a_{45}&a_{46}
	\end{pmatrix}
\begin{pmatrix}
	b_{51}&b_{52}\\
	b_{61}&b_{62}
\end{pmatrix} 		
\\ \\
&=
A_{21}B_{11} + A_{22}B_{21} + A_{23}B_{31}
\end{align}
$$
