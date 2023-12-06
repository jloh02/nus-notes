> Name: Jonathan Loh Junhao
> Tutorial Group: T17A

## Question 1

## (a)
### (i) Use Theorem 2.1.1 to simplify 𝑠 as much as possible, with your answer still in DNF. To avoid showing too many steps, you may skip steps involving the associative law.

$$
\begin{align}
s &\equiv (p \land q \land \sim r) \lor (p \land \sim q \land \sim r) \lor (\sim p \land q \land \sim r) \\
&\equiv (p \land (q \land \sim r)) \lor (p \land (\sim q \land \sim r)) \lor (\sim p \land q \land \sim r) &\text{(By associative law)} \\
&\equiv (p \land ((q \land \sim r) \lor (\sim q \land \sim r))) \lor (\sim p \land q \land \sim r) & \text{(By distributive law)} \\
&\equiv (p \land ((\sim r \land q) \lor (\sim r \land \sim q))) \lor (\sim p \land q \land \sim r) & \text{(By commutative law twice)} \\
&\equiv (p \land (\sim r \land(q  \lor \sim q)) \lor (\sim p \land q \land \sim r) &\text{(By distributive law)} \\
&\equiv (p \land (\sim r \land true) \lor (\sim p \land q \land \sim r) &\text{(By negation law)} \\
&\equiv (p \land \sim r) \lor (\sim p \land q \land \sim r) &\text{(By identity law)} \\
&\equiv (\sim r \land p) \lor (\sim r \land \sim p \land q) &\text{(By commutative law twice)} \\
&\equiv \sim r \land(p \lor (\sim p \land q)) &\text{(By distributive law)} \\
&\equiv \sim r \land((p \lor \sim p) \land (p \lor q)) &\text{(By distributive law)} \\
&\equiv \sim r \land(true \land (p \lor q)) &\text{(By negation law)} \\
&\equiv \sim r \land((p \lor q) \land true) &\text{(By commutative law)} \\
&\equiv \sim r \land(p \lor q) &\text{(By identity law)} \\
\end{align}
$$

### (ii) From your answer in part (i), express 𝑠 using only ~ and ∨
$$
\begin{align}
&\text{From 1(i),} \\
&s\ \equiv \sim r \land (p \lor q)\\
&\quad \equiv \sim r \land \sim\sim(p \lor q) &\text{(By double negative law)}\\
&\quad \equiv \sim (r \lor \sim(p \lor q)) &\text{(By De Morgan's law)}\\
\end{align}
$$

### (iii) nor can be defined as follows: nor(𝑥 ,𝑦 ) $\equiv$ ~(𝑥 ∨ 𝑦 ) to represent every propositional formula. Express negation ~𝑎, disjunction 𝑎 ∨ 𝑏, and conjunction 𝑎 ∧ 𝑏 in terms of nor. Working is not required. 
$$
\begin{align}
&\text{Negation:}&&\sim a= nor(a,a)\\
&\text{Disjunction:}&&a \lor b = nor(nor(a,b),nor(a,b)) \\
&\text{Conjunction:}&&a \land b = nor(nor(a,a),nor(b,b))
\end{align}
$$

<div style="page-break-after: always;"></div>

## (b) Prove that this argument is valid or give a counter-example otherwise. State whether the argument is valid or invalid, and whether it is sound or unsound.

The argument is <ins>valid</ins>

$$
\begin{align}
&\text{1. The argument is valid if and only if the P(a)}\rightarrow\text{Q(a), where P(a) is the premise and Q(a) the conclusion} \\ 
&\text{2. Evaluate the premise P(a), given H and T are true,} \\
&\quad \begin{aligned}
	2.1 &\quad((P \lor H) \rightarrow \sim T) \land H \land T) \\
	2.2 &\equiv (((P \lor true) \rightarrow \sim true) \land true \land true) \\ 
	2.3 &\equiv ((P \lor true) \rightarrow \sim true) &\text{(By identity law twice)} \\
	2.4 &\equiv (true \rightarrow \sim true)  &\text{(By universal bound law)} \\
	2.5 &\equiv \sim true \lor \sim true  &\text{(By implication law)} \\
	2.6 &\equiv \sim true  &\text{(By indempotent law)} \\
	2.7 &\equiv false  &\text{(By negation of true)} \\
\end{aligned}\\
&\text{3. Since }\sim P(a)\text{, P(a) is vacuously true}\\
&\text{4. The argument is valid}
\end{align}
$$

The argument is <ins>unsound</ins> as it is not valid with the premises

<div style="page-break-after: always;"></div>

## Question 2
### (a) Write the transitivity rule of inference for prerequisite
$$
\forall X,Y,Z\in C, (Preq(X,Y) \land Preq(Y,Z) \rightarrow Preq(X,Z))
$$
### (b) If a student has taken course 𝑌 and course 𝑋 is a prerequisite of 𝑌, then the student must have taken 𝑋. Express this rule as a quantified statement.
$$
\forall ((s\in N), (X,Y \in C)), (Preq(X,Y) \land Taken(s,Y) \rightarrow Taken(s,X))
$$
### (c) Translate the above argument into logic statements and show whether the argument is valid or not.
$$
\begin{align}
&\text{Let a be Aiken, where } a\in N \text{. And P,Q,R,S be the relevant courses, where }P,Q,R,S\in C\\\\
&\begin{aligned}
&Preq(P,R) \\
&Preq(P,S) \\ 
&Preq(Q,R) \\ 
&Preq(Q,S) \\
&Taken(a,R) \\
&\sim Taken(a,S) \\ \\
\hline \\
&Taken(a,P) \land Taken(a,Q)
\end{aligned}
\end{align}
$$


$$
\begin{align}
& 1.\ \text{Prove that } (Preq(P,R) \land Preq(Q,R) \land Taken(a,R) \land Preq(P,S)  \land Preq(Q,S) \land \sim Taken(a,S)) \\
&\quad\ \rightarrow (Taken(a,P) \land Taken(a,Q)) \\
& 2.\ \text{Consider }Preq(P,R) \land Preq(Q,R) \land taken(a,R)\\
&\quad\begin{aligned}
	&2.1 \quad\ Preq(P,R) \land Preq(Q,R) \land Taken(a,R)\\
	&\quad\ \equiv Preq(P,R) \land Preq(Q,R) \land Taken(a,R) \land Taken(a,R) &\text{(By idempotent laws)}\\
	&2.2 \equiv Preq(P,R) \land Taken(a,R) \land Preq(Q,R) \land Taken(a,R) &\text{(By commutative laws)}\\
	&2.3 \equiv Preq(P,R) \land Taken(a,R) \land (Preq(Q,R) \land Taken(a,R)) &\text{(By associative laws)}\\
	&2.4 \equiv Taken(a,P) \land Taken(a,Q) &\text{(By rule in (b) twice)}\\
\end{aligned}\\
& 3.\ \text{From above}, Preq(P,R) \land Preq(Q,R) \land Taken(a,R) \land Preq(P,S)  \land Preq(Q,S) \land \sim Taken(a,S) \\
&\quad\ \equiv Taken(a,P) \land Taken(a,Q) \land Preq(P,S)  \land Preq(Q,S) \land \sim Taken(a,S) \\
& 4.\therefore \text{The initial argument is valid, }Taken(a,P) \land Taken(a,Q)
\end{align}
$$


<div style="page-break-after: always;"></div>

## Question 3
### (a)
False. When x=1, y=5, z=9, 
|x-y| = 4, |y-z| = 4 
Since |x-y|=|y-z|=4, the initial statement is false
### (b)
True. Since the minimum value in B is 7 and maximum value in C is 6, any value chosen from C is less than B. Therefore, $\forall y \in B, \forall z \in C, (y > z)$ . This makes (𝑥 < 𝑦) ∧ (𝑦 < 𝑧) false. Since the premise is false, the statement is vacuously true.
### (c)
True. 
$$
\begin{align}
&1.\ \text{If we choose y = 5, }\forall z\in C,|y-z|=5 \\
&2.\ \text{Proof by cases } \forall x\in A,y = 5,|y-z|=5 \\
&3.\ \text{Case 1: }x=2 \\
&\qquad \text{3.1 When }|x-y| = 3 < 5 \\
&4.\ \text{Case 2: }x=4 \\
&\qquad \text{4.1 When }|x-y| = 1 < 5 \\
&5.\ \text{Case 3: }x=6 \\
&\qquad \text{5.1 When }|x-y| = 1 < 5 \\
&6.\ \text{Case 4: }x=8 \\
&\qquad \text{6.1 When }|x-y| = 3 < 5 \\
&7.\ \therefore \forall x\in A, \exists y \in B, \forall z \in C, (|x-y| < |y-z|) \\
\end{align}
$$

<div style="page-break-after: always;"></div>

## Question 4
### (a)
#### (i) 𝐴 ∪ 𝒫(𝐴) = 𝒫(𝐴)
$A = \emptyset$
#### (ii) 𝐴 ∪ 𝒫(𝐴) ≠ 𝒫(𝐴)
$A = \{5\}$
#### (iii) 𝐴 ∩ 𝒫(𝐴) = ∅
$A = \emptyset$
#### (iv) 𝐴 ∩ 𝒫(𝐴) ≠ ∅
$A = \{\emptyset\}$

### (b) Prove or disprove the following statements on two non-empty sets 𝐴 and 𝐵
#### (i) ∅ ⊆ (𝒫(𝐴) − 𝒫(𝐵))
$$
\begin{align}
&1.\ \emptyset \subseteq C, \text{where C is any set}\\
&2.\ \mathcal{P}(A) - \mathcal{P}(B) \text{ is a set, more specifically the difference of 2 power sets} \\
&3.\ \text{Since }\mathcal{P}(A) - \mathcal{P}(B) \text{ is a set, the initial statement is true, and } \emptyset \subseteq (\mathcal{P}(A) - \mathcal{P}(B))
\end{align}
$$
#### (ii) ∅ ∉ (𝒫(𝐴) − 𝒫(𝐵))
$$
\begin{align}
&1.\ \text{First evaluate the difference of power sets, }\mathcal{P}(A) - \mathcal{P}(B) \\
&\quad\begin{aligned}
	&1.1\ \text{Let }A=\{a_1,a_2,...\},B=\{b_1,b_2,...\} \\
	&1.2\ \mathcal{P}(A) = \{\emptyset,\{a_1\},\{a_2\},...\}, \mathcal{P}(B) = \{\emptyset,\{b_1\},\{b_2\},...\}  &\text{(By power set axiom)}\\
	&1.3\ \mathcal{P}(A) - \mathcal{P}(B) = \{\{a_1\},\{a_2\},...,\{b_1\},\{b_2\},...\}   &\text{(By difference of sets)}\\
\end{aligned}\\
&2.\ \emptyset \not\in \mathcal{P}(A) - \mathcal{P}(B), \text{ hence, the intial statement is true}
\end{align}
$$
#### (c) Let 𝑘|𝑛 denote 𝑘 divides 𝑛. Let 𝐴 = {𝑥 ∈ ℤ+ ∶ 𝑥|𝑐} and 𝐵 = {𝑥 ∈ ℤ+ ∶ 𝑥|𝑑}. What is the restriction on 𝑐 and 𝑑 such that 𝐴 ⊆ 𝐵? You do not need to prove this.

By standard definition, $(c,d \in \mathbb{Z}),c|d$. Since by definition in CS1231S, $k|n$ only when $k \neq 0$, 
$$
(c=0,d=0)\lor (c \in \mathbb{Z}, c \neq 0,d \in \mathbb{Z},c|d)
$$

<div style="page-break-after: always;"></div>

## Question 5
>Prove or disprove the following statement:  
>	“There exists an integer 𝑛 greater than 3 such that the numbers 𝑛, 𝑛 + 2, and 𝑛 + 4 are all prime.

$$
\begin{align}
&1.\ \text{Proof by cases. Suppose }\exists n\in \mathbb{Z^+},n>3, \text{ n is prime, n+2 is prime and n+4 is prime.} \\
&2.\ \text{Consider the 2 cases where }k\in\mathbb{Z^+},k>1,n\in\mathbb{Z^+},n>3, n\text{ is prime}\\
&\quad2.1\ n = 3k+1\\
&\quad2.2\ n = 3k+2\\
&\quad2.3\ \text{Notice that } n = 3k \text{ is not prime because } 3|n\\
&3.\ \text{Case 1: n = 3k + 1}\\
&\quad\begin{aligned}
	&3.1\ \text{Consider if n+2 is prime} \\
	&3.2\quad\ \ n + 2 \\
	&3.3\ = (3k + 1) + 2 &\text{(By substitution of n)}\\
	&3.4\ = 3k + 3 &\text{(By basic algebra)}\\
	&3.5\ = 3(k + 1) &\text{(By basic algebra)}\\
	&3.6\ \text{Since 3|(n+2), and n+2 is not prime when n=3k+1}\\
	&3.7\therefore \text{Statement is false when n=3k+1}\\
\end{aligned}\\
&4.\ \text{Case 2: n = 3k + 2}\\
&\quad\begin{aligned}
	&4.1\ \text{Consider if n+4 is prime} \\
	&4.2\quad\ \ n + 4 \\
	&4.3\ = (3k + 2) + 4 &\text{(By substitution of n)}\\
	&4.4\ = 3k + 6 &\text{(By basic algebra)}\\
	&4.5\ = 3(k + 2) &\text{(By basic algebra)}\\
	&4.6\ \text{Since 3|(n+4), and n+4 is not prime when n=3k+2}\\
	&4.7\therefore \text{Statement is false when n=3k+2}\\
\end{aligned}\\
&5.\therefore\forall n\in\mathbb{Z^+},n>3,\sim(\text{n is prime} \land \text{n+2 is prime} \land \text{n+4 is prime} ) \\
&6. \text{ The initial statement is false}
\end{align}
$$