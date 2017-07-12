**Weakness of hashing:**  For any choice of hash function, it always exist bad set of keys that all hash to same slot.
**Idea:** Choose hash function at random,independently from the keys.


----------


<h2> Universal Hashing </h2>

**Definition1:** Let $U$ be a universal of keys, and let $H$ be a finite collection of hash functions,mapping $U$ to $\{0,1,...,m-1\}$.

So $H$ is universal:
$$\text{if }\forall x,y \in U,\text{where $x \neq y$},\text{then } \{h \in H,h(x) = h(y)\}=\frac{\vert{H}\vert} {m}$$.
*I.e.* if $h$ is chosen randomly from $H$, the probability of collision between $x$ and $y$ is $\frac1x$.

***Theorem1:***
Choose $h$ randomly from $H$, suppose hashing $n$ keys into $m$ slots in Table T, then for a given key $x$,its expect number of collisions with $x$ is:
$$E\left[\# \text{collisions with $x$}\right]\lt \frac{n}{m}$$

**Proof:**
Let $c_x$ be random variable, the total number collisions of keys in T with $x$, and let
$$c_{xy}=\begin{cases}
1,&\text{if $h(x)=h(y)$} \\
0,&\text{otherwise}
\end{cases}$$
Note:	$E[c_{xy}]=\frac1m$ and $C_x=\sum_{y\in T-\{x\}} c_{xy}$ ,and y is the element not equal with x in Table T.

So:$$E[C_x]=E[\sum_{y\in T-\{x\}} c_{xy}]
=\sum_{y\in T-\{x\}} E[c_{xy}]
=\sum_{y\in T-\{x\}}\frac1m=\frac{n-1}m$$


----------

<h3>Constructing a universal hash function</h3>

Let $m$ be prime, decompose key $k$ into $r+1$ digits, so
$$k=\langle{k_0,k_1,...,k_r}\rangle\ \text{where $0\le k_i \le m-1$}$$
in here we treating $k$ as an $r+1$ base $m$ number.
Now we're going to pick an $a$ at random which we're also going to look at as a base $m$ number, so pick $a=\langle{a_0,a_1,...,a_r}\rangle$, each $a_i$ is chosen randomly from $\{0,1,...,m-1\}$, so the hash function as below:

**Definition2:**
$$h_a(k)=(\sum_{i=0}^r a_ik_i)\%m$$

*We want to know how big is the set of hash function here? how many different hash functions do I have in this set?*
**Conclusion:**
$$\vert{H}\vert = m^{r+1}$$
**Explanation:**
Because it have $m$ choices for each $a_i$ ($0 \le a_i \le m-1$), and  for $a$ vector has $r+1$ elements from $a=\langle{a_0,a_1,...,a_r}\rangle$.

***Theorem2:***  $H$ **is universal.**

**Proof:**
Let $x=\langle x_0,x_1,...,x_r\rangle$ , $y=\langle y_0,y_1,...,y_r\rangle$  be distinct keys and they differ $m$ at least one digit. They could differ in any one of these digits, in here we assume they differ in position 0.

 *Question:*  How many hash functions in universal $h_a(x) \in H$ do $x$ and $y$ collide? 
 
 It must have $h_a(x) = h_a(y)$ if they collide.
 $$\Rightarrow \quad \left(\sum_{i=0}^r a_ix_i\right)\%m \equiv \left(\sum_{i=0}^r a_iy_i\right)\%m$$
  $$\Rightarrow \quad \left(\sum_{i=0}^r a_ix_i\right)\ \equiv \left(\sum_{i=0}^r a_iy_i\right) \quad (\%m) \quad\text{//do module m after everything is being done. }$$
   $$\Rightarrow \quad \sum_{i=0}^r a_i(x_i-y_i) \equiv 0 \quad (\%m)$$
   $$\Rightarrow \quad a_0(x_0-y_0)+\sum_{i=1}^r a_i(x_i-y_i) \equiv 0 \quad (\%m)$$
   $$\Rightarrow \quad a_0(x_0-y_0)=-\sum_{i=1}^r a_i(x_i-y_i) \quad (\%m)$$
   since $x_0 \neq y_0$, so $\exists (x_0-y_0)^{-1}$ to make following formula is true according with **Number Theory Fact**.
   $$a_0=\left(-\sum_{i=1}^r a_i(x_i-y_i)\right)(x_0-y_0)^{-1}$$
	**Number Theory Fact:**

> let $m$ be prime, for any $z \in \mathcal {Z_m}\ \text{($\mathcal {Z_m}$ are intergers module $m$)}$, so for any $z$ is not congruent to 0, there exists a unique $z$ inverse in $\mathcal {Z_m}$, such that if I multiply $z$ times the inverse, it produces something congruent to one mod $m$.
> *I.e.*$$\$z \not \equiv 0, \exists\ \text{unique}\ z^{-1} \in \mathcal {Z_m}\ \Rightarrow \$zz^{-1} \equiv 1\ \text{(mod $m$)}$$.

**Conclusion:**
Thus, for any choice of $a_1,a_2,...,a_r$ exactly 1 of the $m$ choices for $a_0$ cause $x$ and $y$ to collide, and no collision for other $m-1$ choices for $a_0$.
So the number of $h_a$'s that cause $x$, $y$ to collide:
	 $$\Rightarrow \quad m*m...*m*1=m^r=\frac {\vert H \vert}m$$
because $a_1$ has $m$ choices, and $m$ choices for $a_2$ ..., but only 1 choice for $a_0$ if want to cause collision.


----------
<h2> Perfect Hashing </h2>

*Situation:* Given $n$ keys construct a static hash table of size $m=O (n)$, such that search takes $O(1)$ time in the worst case.

**Idea:** Use a 2-level scheme with universal hashing at both levels. And the idea is that we're going to do it in such a way that we have no collisions at level 2 and we'll take any collides at level 1.
If $n_i$ items hash to level-1 slot $i$, then use $m_i=n_i^2$ slots in level-2 table $s_i$.


----------
<h3>Level-2 Analysis</h3>
***Theorem:*** Hash $n$ keys into $m=n^2$ slots using random $h$ in universal $H$, we can get$$E[\#\text{collisions}] \lt \frac{1}{2}$$

**Proof:** Probability 2 given keys collide under $h$ is $\frac1m=\frac1n^2$.
$$E[\#\text{collisions}]={2 \choose n}*\frac1n^2$$
$$\Rightarrow \quad =\frac{n(n-1)}2 \frac1n^2 \lt \frac{1}{2}$$
*Note:* ${2 \choose n}=C_n^2$.

**Markov inequality**
	

> For randomly variable $x \ge 0 ,\ \text{Pr{$x \ge t$} $\le$ $\frac{E[x]}{t}$}$.

**Proof:**
$$E[x]=\sum_{x=0}^\infty x*\text{Pr{$X=x$}}\ \ge \sum_{x=t}^\infty x*\text{Pr{$X=x$}}$$
$$\Rightarrow \quad \ge \sum_{x=t}^\infty t*\text{Pr{$X=x$}}=t*\text{Pr{$X \ge t$}}$$

***Corollary:***
$$\text{Pr{no collisions} $\ge$ $1 \over 2$}$$
Now we can use the Markov inequality theorem to prove that the corollary is correct.

**Proof:**
$$\text{Pr{$\ge 1$ collisions}} \le \frac {E[\# collisions]}{1} \lt \frac {1}{2}$$
$$\Rightarrow \quad \text{Pr{no collisions}}=1-\text{Pr{$\ge 1$ collisions}} \ge \frac{1}{2}$$

*Conclusion:* So we can know that to find a good level-2 hash function, just test a few at random, and we'll find one quickly since at least half will work.

***Analysis of storage***

For level-1 choose $m=n$, and let $n_i$ be the random variable for the number of keys that hash to slot $i$ in table T, use $m_i=n_i^2$ slots in each level-2 table $s_i$, so

> $$E[\text{total storage}]=n+E[\sum_{i=0}^{m-1} \theta(n_i^2)]=\theta(n)$$

*Note:* we can get $E[\sum_{i=0}^{m-1} \theta(n_i^2)]=\theta(n)$ by bucket sort analysis.