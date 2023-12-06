> Name: Jonathan Loh Junhao
> Tutorial Group: T17A

### Question 1

#### (a) $f^{-1}(\{0\})$
$$
\begin{align}
&\text{When } f(x) = 0, 3 \not\mid (x-2)\\
&\text{When } x=2, f(2) = (x-2)/3 = 0\\
&f^{-1}(\{0\}) = \{ x \in \mathbb{Z} \mid 3 \not\mid (x-2) \lor x=2\}
\end{align}
$$

#### (b) Does 𝑓 have a left inverse? If yes, find it. If not, prove that it does not have one.

$$
\begin{align}
&\text{1. Consider if }f\text{ is injective, } \forall x_1,x_2 \in X (f(x_1) = f(x_2) \implies x_1 = x_2) & \text{(By definition of injective function)}\\
&\qquad\begin{aligned}
&\text{1.1 Consider }x_1 = 3, x_2 = 4\\
&\text{1.2 }f(x_1) = 0, f(x_2) = 0\\
&\text{1.3 }f(x_1) = f(x_2), x_1 \neq x_2\\
&\text{1.4 }\therefore f \text{ is not injective}
\end{aligned}\\
&\text{2. By Fact 1, } f\text{ is not injective} \implies f\text{ does not have a left inverse}\\
\end{align}
$$

#### (c) Does 𝑓 have a right inverse? If yes, find it. If not, prove that it does not have one.
$$
\text{Right inverse function }h(x) = 3x+2, x \in \mathbb{Z}
$$
### Question 2
#### (a)
$$
\begin{align}
&\text{Let }C = \{0\} \text{ and }g(x) = 0\\
&g \text{ is not injective because }g(1) = g(-1) = 0, 1\neq -1\\
&g \circ f \text{ is injective because there does not exist an }x\text{ where }f(x) = -1\\
&\quad\implies \text{all elements in codomain have at most 1 arrow}
\end{align}
$$
#### (b)
$$
\begin{align}
&\text{There is no such function } g \text{ for all }f\\
&\text{Let }A=B \text{ and } f(x) = x, x\in A\\
&B = f(A) = A\\
&g(f(A)) = g(B) = g(A)\\
&f\text{ is injective, but } g\circ f = g \text{ is not injective}
\end{align}
$$

<div style="page-break-after: always;"></div>

### Question 3
$$
\begin{align}
&\text{1. Let } F2(n)=(F(n),F(n+1)) \text{ be the proposition }\forall n\in \mathbb{N}\\
&\text{2. Basis Step: To prove }F2(0)\text{ is true}\\
&\qquad\begin{aligned}
&\text{2.1 }F2(0) = (0,1) = (F(0),F(1)) &\text{(By definition of F2 and F)}\\
&\text{2.2 }F2(0) \text{ is true}\\
\end{aligned}\\
&\text{3. To prove }F2(k) \text{ is true}\implies F2(k+1) \text{ is true for some }n=k\\
&\qquad\begin{aligned}
&\text{3.1 Assume F2(k) is true, }F2(k) = H(F2(k-1)) = (F(k), F(k+1)) &\text{(Inductive Hypothesis)} \\
&\text{3.2 }F2(k+1) = H(F2((k+1)-1)) &\text{(By definition of F2)}\\
&\text{3.3 }\qquad\qquad\ = H(F2(k)) &\text{(By basic algebra)}\\
&\text{3.4 }\qquad\qquad\ = H((F(k), F(k+1))) &\text{(By inductive hypothesis)}\\
&\text{3.5 }\qquad\qquad\ = (F(k+1), F(k) + F(k+1)) &\text{(By definition of H)}\\
&\text{3.6 }\qquad\qquad\ = (F(k+1), F((k+1) + 1)) &\text{(By definition of F)}\\
&\text{3.7 }\therefore F2(k) \text{ is true}\implies F2(k+1) \text{ is true}\\
\end{aligned}\\
&\text{4. Since }F2(0)\text{ is true, and }  (F2(k) \text{ is true}\implies F2(k+1)\ \forall k\in\mathbb{N}_{>0}), \text{by mathematical induction},\\
&\quad F2(n) \text{ is true } \forall n \in \mathbb{N} 
\end{align}
$$

### Question 4
#### (a)
$^{24}C_{4} = 10626$
#### (b)
$^{23}C_{3} + ^{22}C_{3} + ^{21}C_{3} + ^{20}C_{3} + ^{19}C_{3} + ^{18}C_{3} = 7566$
#### (c)
For 0-5 red/green balloons, 
$^{22}C_{2} + 2\times^{21}C_{2} + 3\times ^{20}C_{2} + 4\times ^{19}C_{2} + 5\times ^{18}C_{2} + 6\times ^{17}C_{2} = 3486$

For 6-10 red/green balloons,
$5 \times ^{16}C_{2} + 4 \times ^{15}C_{2} + 3 \times ^{14}C_{2} + 2 \times ^{13}C_{2} + ^{12}C_{2} = 1515$

$3486 + 1515 = 5001$

<div style="page-break-after: always;"></div>

### Question 5
#### (a)
By Theorem 9.2.1 Multiplication Rule, $3^4 = 81$
#### (b)
By Theorem 9.2.1 Multiplication Rule, $3 * 2^3 = 24$
#### (c)
$$
\begin{align}
&\ \quad |B| - |A \cap B|\\
&= |A| + |B| - |A \cap B| - |A| &\text{(By basic algebra)}\\
&= |A \cup B| - |A| &\text{(By Theorem 9.3.3 Inclusion/Exclusion Rule)}\\
&= |(A \cup B) \setminus A| &\text{(By Theorem 9.3.2 Difference Rule)}\\
&= |(A \cup B) \cap \bar{A}| &\text{(By Set Difference Law)}\\
&= |(\bar{A} \cap A) \cup (\bar{A} \cap B)| &\text{(By Distributive Law)}\\
&= |\emptyset \cup (\bar{A} \cap B)| &\text{(By Complement Law)}\\
&= |\bar{A} \cap B| &\text{(By Identity Law)}\\
\end{align}
$$
#### (d)
Let $A$ be the set of length 4 strings that have consecutive symbols.
Let $B$ be the set of length 4 strings that contain either "cab" or "bac".

$|A \cap B| = |\{\text{``bbac"},\text{``bacc"},\text{``ccab"},\text{``cabb"}\}| = 4$

Number of length 4 strings that contain neither "cab" nor "bac", or do not have consecutive symbols:
$|\overline{A} \cup \overline{B}|$
$= |\overline{A \cap B}|$   (By De Morgan's Law)
$= |\text{All length 4 strings}| - |A\cap B|$ (By definition of cardinality)
$= 81 - 4$ (By Q5a)
$= 77$

<div style="page-break-after: always;"></div>

### Question 6
#### (a)
$$
\frac{12}{51} \times \frac{11}{50} = \frac{132}{2550} = \frac{22}{425}
$$
#### (b)
Let each pigeonhole be a group of 3 consecutive numbers. The most number of pigeonholes can be achieved using such set $\{\{1,2,3\},\{4,5,6\}...\{1000\}\}$ or $\{\{1\},\{2,3,4\}...\{998,999,1000\}\}$. 

After filling each pigeonhole to a maximum of 2 pigeons and the singleton set fully with 1,
$\lfloor1000/3\rfloor * 2 + 1 = 333 * 2 + 1 = 667$ pigeons

Therefore $667+1=668$ pigeons are required

$k = 668$
#### (c)

Define $mul(n)$ as the set of positive integers not exceeding 2000 that are multiples of $n$
$$
\begin{align}
(&mul(3)) \cap ((mul(4)) \cup (mul(5)))\\
&=((mul(3)) \cap (mul(4))) \cup ((mul(3)) \cap (mul(5))) &\text{(By distributive law)}\\
&=(mul(3),4)) \cup (mul(lcm(3,5))) &\text{(By given fact)}\\
&=(mul(12)) \cup (mul(15)) &\text{(By lcm definition)}\\\\
&|(mul(12)) \cup (mul(15))|\\
&=|(mul(12))| + |(mul(15))| - |(mul(12)) \cap (mul(15))| &\text{(By Theorem 9.3.3 Inclusion/Exclusion Rule)}\\
&=|(mul(12))| + |(mul(15))| - |(mul(lcm(12,15))| &\text{(By given fact)}\\
&=|(mul(12))| + |(mul(15))| - |(mul(60))| &\text{(By lcm definition)}\\
\end{align}
$$

Positive integers that are multiples of 12 not exceeding 2000: $|(mul(12))| = \lfloor 2000/12 \rfloor = 166$ 
Positive integers that are multiples of 15 not exceeding 2000: $|(mul(15))| = \lfloor 2000/15 \rfloor = 133$ 
Positive integers that are multiples of 60 not exceeding 2000: $|(mul(60))| = \lfloor 2000/60 \rfloor = 33$ 

Number of positive integers not exceeding 2000 are multiples of 3 as well as multiples of either 4 or 5: 
$|(mul(12))| + |(mul(15))| - |(mul(60))| = 166+133-33 = 266$