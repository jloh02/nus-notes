## Question 1
### (i)
$$
\begin{align}
&B = A^TA = \begin{pmatrix}2&1&1&0\\0&-2&-1&3\end{pmatrix}\begin{pmatrix}2&0\\1&-2\\1&-1\\0&3\end{pmatrix} = \begin{pmatrix}6&-3\\-3&14\end{pmatrix}\\
&det(xI_2-B) = x^2 - 20x + 75 = (x-5)(x-15)\\
&\text{Eigenvalues of B }\lambda = 5,15
\end{align}
$$

### (ii)
$$
\begin{align}
\lambda=5,\quad &\lambda I_2-B = 5I_2-B = \begin{pmatrix}-1&3\\3&-9\end{pmatrix} = \begin{pmatrix}1&-3\\0&0\end{pmatrix}\\
\frac{\begin{pmatrix}3\\1\end{pmatrix}}{\left|\left|\begin{pmatrix}3\\1\end{pmatrix}\right|\right|} &= \frac{1}{\sqrt{10}}\begin{pmatrix}3\\1\end{pmatrix}\\
\lambda=15,\quad &\lambda I_2-B = 15I_2-B = \begin{pmatrix}9&3\\3&1\end{pmatrix} = \begin{pmatrix}1&\frac{1}{3}\\0&0\end{pmatrix}\\
\frac{\begin{pmatrix}-1\\3\end{pmatrix}}{\left|\left|\begin{pmatrix}-1\\3\end{pmatrix}\right|\right|} &= \frac{1}{\sqrt{10}}\begin{pmatrix}-1\\3\end{pmatrix}\\\\
\text{Unit eigenvectors of B} &= \frac{1}{\sqrt{10}}\begin{pmatrix}3\\1\end{pmatrix},\quad\frac{1}{\sqrt{10}}\begin{pmatrix}-1\\3\end{pmatrix}
\end{align}
$$


### (iii)
$$
\begin{align}
B &= 
\begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}
\begin{pmatrix}5&0\\0&15\end{pmatrix}
\begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}^{-1}\\\\
&=
\begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}
\begin{pmatrix}5&0\\0&15\end{pmatrix}
\begin{pmatrix}\frac{3}{\sqrt{10}}&\frac{1}{\sqrt{10}}\\-\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}\\\\
&=
\begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}
\begin{pmatrix}5&0\\0&15\end{pmatrix}
\begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}^\top\\\\
E &= \begin{pmatrix}\frac{3}{\sqrt{10}}&-\frac{1}{\sqrt{10}}\\\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\end{pmatrix}, \quad D = \begin{pmatrix}5&0\\0&15\end{pmatrix}
\end{align}
$$

### (iv)

$$
\begin{align}
A &= U\Sigma V^\top\\
\sigma_1 &= \sqrt{15}, \sigma_2 = \sqrt{5}
\\\\
v_1 &= \begin{pmatrix}-\frac{1}{\sqrt{10}}\\\frac{3}{\sqrt{10}}\end{pmatrix}, v_2 = \begin{pmatrix}\frac{3}{\sqrt{10}}\\\frac{1}{\sqrt{10}}\end{pmatrix}\\
V &= \begin{pmatrix}-\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\\\frac{3}{\sqrt{10}}&\frac{1}{\sqrt{10}}\end{pmatrix}
\end{align}
$$

<div style="page-break-after: always;"></div>

$$
\begin{align}
u_1 &= \frac{1}{\sigma_1} A v_1 = \frac{1}{\sqrt{15}} A \begin{pmatrix}-\frac{1}{\sqrt{10}}\\\frac{3}{\sqrt{10}}\end{pmatrix} = \begin{pmatrix}-\frac{2}{5\sqrt{6}}\\-\frac{7}{5\sqrt{6}}\\-\frac{4}{5\sqrt{6}}\\\frac{9}{5\sqrt{6}}\end{pmatrix}\\
u_2 &= \frac{1}{\sigma_2} A v_2 = \frac{1}{\sqrt{5}} A \begin{pmatrix}\frac{3}{\sqrt{10}}\\\frac{1}{\sqrt{10}}\end{pmatrix} = \begin{pmatrix}\frac{6}{5\sqrt{2}}\\\frac{1}{5\sqrt{2}}\\\frac{2}{5\sqrt{2}}\\\frac{3}{5\sqrt{2}}\end{pmatrix}
\\\\
&\begin{pmatrix}-&u_1&-\\-&u_2&-\end{pmatrix}\xrightarrow{rref}\begin{pmatrix}1&0&1/4&3/4\\0&1&1/2&-3/2\end{pmatrix}\\\\\\
u_3 &= \frac{\begin{pmatrix}-1\\-2\\4\\0\end{pmatrix}}{\left|\left|\begin{pmatrix}-1\\-2\\4\\0\end{pmatrix}\right|\right|}\\
&\begin{pmatrix}-1\\4\\-2\\2\end{pmatrix} - \frac{\begin{pmatrix}-1\\4\\-2\\2\end{pmatrix}\cdot\begin{pmatrix}-1\\-2\\4\\0\end{pmatrix}}{\left|\left|\begin{pmatrix}-1\\-2\\4\\0\end{pmatrix}\right|\right|^2}\begin{pmatrix}-1\\-2\\4\\0\end{pmatrix} = \frac{2}{7}\begin{pmatrix}-6\\9\\3\\7\end{pmatrix} \\
u_4 &= \frac{\begin{pmatrix}-6\\9\\3\\7\end{pmatrix}}{\left|\left|\begin{pmatrix}-6\\9\\3\\7\end{pmatrix}\right|\right|} = \begin{pmatrix}-\frac{6}{5\sqrt{7}}\\\frac{9}{5\sqrt{7}}\\\frac{3}{5\sqrt{7}}\\\frac{\sqrt{7}}{5}\end{pmatrix}\\\\
U &= \begin{pmatrix}
-\frac{2}{5\sqrt{6}}&\frac{6}{5\sqrt{2}}&-\frac{1}{\sqrt{21}}&-\frac{6}{5\sqrt{7}}\\
-\frac{7}{5\sqrt{6}}&\frac{1}{5\sqrt{2}}&-\frac{2}{\sqrt{21}}&\frac{9}{5\sqrt{7}}\\
-\frac{4}{5\sqrt{6}}&\frac{2}{5\sqrt{2}}&\frac{4}{\sqrt{21}}&\frac{3}{5\sqrt{7}}\\
\frac{9}{5\sqrt{6}}&\frac{3}{5\sqrt{2}}&0&\frac{\sqrt{7}}{5}
\end{pmatrix}
\\\\
\Sigma &= \begin{pmatrix}\sqrt{15}&0\\0&\sqrt{5}\\0&0\\0&0\end{pmatrix}
\\\\
A &= \begin{pmatrix}
-\frac{2}{5\sqrt{6}}&\frac{6}{5\sqrt{2}}&-\frac{1}{\sqrt{21}}&-\frac{6}{5\sqrt{7}}\\
-\frac{7}{5\sqrt{6}}&\frac{1}{5\sqrt{2}}&-\frac{2}{\sqrt{21}}&\frac{9}{5\sqrt{7}}\\
-\frac{4}{5\sqrt{6}}&\frac{2}{5\sqrt{2}}&\frac{4}{\sqrt{21}}&\frac{3}{5\sqrt{7}}\\
\frac{9}{5\sqrt{6}}&\frac{3}{5\sqrt{2}}&0&\frac{\sqrt{7}}{5}
\end{pmatrix}\begin{pmatrix}\sqrt{15}&0\\0&\sqrt{5}\\0&0\\0&0\end{pmatrix}\begin{pmatrix}-\frac{1}{\sqrt{10}}&\frac{3}{\sqrt{10}}\\\frac{3}{\sqrt{10}}&\frac{1}{\sqrt{10}}\end{pmatrix}
\end{align}
$$


<div style="page-break-after: always;"></div>

## Question 2

### (i)
$$
char(A) = det(xI_3-A) = x^3-63x+162
$$
### (ii)
$char(A) = x^3-63x+162 = (x-3)(x-6)(x+9)$
$\text{Eigenvalues of }A\quad \lambda = 3,6,-9$
### (iii)
$$
\begin{align}
\lambda=3,\quad &\lambda I_3-A = 3I_3-A = \begin{pmatrix}1&1&-5\\1&1&-5\\-5&-5&7\end{pmatrix} = \begin{pmatrix}1&1&0\\0&0&1\\0&0&0\end{pmatrix}\\
E_3 &= \text{span}\left\{\begin{pmatrix}-1\\1\\0\end{pmatrix}\right\}\\\\
\lambda=6,\quad &\lambda I_3-A = 6I_3-A = \begin{pmatrix}4&1&-5\\1&4&-5\\-5&-5&10\end{pmatrix} = \begin{pmatrix}1&0&-1\\0&1&-1\\0&0&0\end{pmatrix}\\
E_6 &= \text{span}\left\{\begin{pmatrix}1\\1\\1\end{pmatrix}\right\}\\\\
\lambda=-9,\quad &\lambda I_3-A = -9I_3-A = \begin{pmatrix}-11&1&-5\\1&-11&-5\\-5&-5&-5\end{pmatrix} = \begin{pmatrix}1&0&0.5\\0&1&0.5\\0&0&0\end{pmatrix}\\
E_{-9} &= \text{span}\left\{\begin{pmatrix}-1\\-1\\2\end{pmatrix}\right\}\\\\
\end{align}
$$

## Question 3
### (iv)
$$
\begin{align}
\frac{\begin{pmatrix}-1\\1\\0\end{pmatrix}}{\left|\left|
\begin{pmatrix}-1\\1\\0\end{pmatrix}\right|\right|} &= \begin{pmatrix}-\frac{1}{\sqrt{2}}\\\frac{1}{\sqrt{2}}\\0\end{pmatrix}\\\\
\frac{\begin{pmatrix}1\\1\\1\end{pmatrix}}{\left|\left|
\begin{pmatrix}1\\1\\1\end{pmatrix}\right|\right|} &= \begin{pmatrix}\frac{1}{\sqrt{3}}\\\frac{1}{\sqrt{3}}\\\frac{1}{\sqrt{3}}\end{pmatrix}\\\\
\frac{\begin{pmatrix}-1\\-1\\2\end{pmatrix}}{\left|\left|
\begin{pmatrix}-1\\-1\\2\end{pmatrix}\right|\right|} &= \begin{pmatrix}-\frac{1}{\sqrt{6}}\\-\frac{1}{\sqrt{6}}\\\frac{\sqrt{2}}{\sqrt{3}}\end{pmatrix}\\
\\
\text{Since }&P\text{ is a orthonormal matrix with normalized columns, }A=PDP^{-1} = PDP^\top
\\
A &= 
\begin{pmatrix}
-\frac{1}{\sqrt{2}}&\frac{1}{\sqrt{3}}&-\frac{1}{\sqrt{6}}\\
\frac{1}{\sqrt{2}}&\frac{1}{\sqrt{3}}&-\frac{1}{\sqrt{6}}\\
0&\frac{1}{\sqrt{3}}&\frac{\sqrt{2}}{\sqrt{3}}
\end{pmatrix}
\begin{pmatrix}
3&0&0\\
0&6&0\\
0&0&-9
\end{pmatrix}
\begin{pmatrix}
-\frac{1}{\sqrt{2}}&\frac{1}{\sqrt{3}}&-\frac{1}{\sqrt{6}}\\
\frac{1}{\sqrt{2}}&\frac{1}{\sqrt{3}}&-\frac{1}{\sqrt{6}}\\
0&\frac{1}{\sqrt{3}}&\frac{\sqrt{2}}{\sqrt{3}}
\end{pmatrix}^{-1}\\
\end{align}
$$

### (v)

Yes. Since $0$ is not an eigenvalue of $A$, and $A$ is a square matrix, $A$ is invertible.

