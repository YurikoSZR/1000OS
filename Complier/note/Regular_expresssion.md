#### Kleene Algebra
A **Kleene algebra** is a [Boolean algebra](https://en.wikipedia.org/wiki/Boolean_algebra) with a [Kleene star](https://en.wikipedia.org/wiki/Kleene_star) operation. The Kleene star is defined as the [universal closure](https://en.wikipedia.org/wiki/Universal_closure) of the [universal set](https://en.wikipedia.org/wiki/Universal_set).

Let $\Sigma$ be a alphabet. We can define the regular expression Kleene Algebra over $\Sigma$ as:
$$c_1+c_2=\{c_1,c_2\},\quad c_1c_2= " c_1c_2 " ,\quad C_1C_2="c_1c_2":c_1\in C_1,c_2\in C_2$$
$C^*= \bigcup_{i=0}^{\infty}\prod_{j=1}^{i}c_j,\quad C^+=C C^*$,and $\varepsilon$ is the empty string as the unit in Kleene Algebra.