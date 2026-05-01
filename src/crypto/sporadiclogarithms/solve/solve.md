# sporadiclogarithms

I came up with this challenge while doing some research for qss when I ran into the paper ["On the Semidirect Discrete Logarithm Problem in Finite Groups"](https://hal.science/hal-04663959/file/Slaying_The_Monster__SDLP_.pdf)


## Overview

This challenge implements a black-box group oracle in sage and asks the adversary to solve the semidirect discrete logarithm problem problem on a **semidirect product group** with a limited number of queries.

Each round provides:
- `one`: identity element  
- `g`, `c`: generators  
- `h`: target value  
- `bound`: upper bound on the exponent  

We are asked to find:

$x \in [0, \text{bound}]$

such that:

$s(x) = h$

where:

$s(x) = \text{first component of } (g, c)^x$


## Blackbox Structure

The group operation is defined as:

$
(a, c) \cdot (b, d) =
\left(a \cdot (c b c^{-1}),\; c d\right)
$

This forms a **semidirect product**:

$G \rtimes \langle c \rangle$

### Important properties

Exponentiation behaves as:
$
(g, c)^x =
\left(
g \cdot (c g c^{-1}) \cdot (c^2 g c^{-2}) \cdots,
\;
c^x
\right)
$

So:
- Second component = $( c^x )$
- First component = product of conjugates of \( g \)

## Attack Strategy

We are given only the **first component** \( h \), but the true structure is:

$
(g, c)^x = (h, c^k)
$

for some unknown \( k \).

I solve this using BSGS as described in the paper.


## Step 1: Compute the Order of \( c \)

We first compute the order of \( c \):

$c^{\text{ord}_c} = 1$

```python
cur = one
ord_c = 1
while True:
    cur = bb.mul(cur, c)
    if bb.eq(cur, one):
        break
    ord_c += 1
```

## Step 2: Lift into Product space

I will define a $base = (g,c)$ and use this to solve $(g,c)^x=(h,c^x)$

## Step 3: BSGS

BSGS or (Baby-step Giant-step)

starting at $n = \lceil{\sqrt{bound}}\rceil$

compute the baby steps $(g,c)^j \quad\forall\space j \in[0,n)$
```python
table = {}
cur = (one, one)

for j in range(n):
    table[cur] = j
    cur = hol_mul(bb, cur, base)
```

Then compute the giant steps $(g,c)^{-n}$
```python3
factor = hol_inv(bb, hol_pow(bb, base, n, one))

cur = (h, c_pow)

for i in range(n):
    if cur in table:
        x = i * n + table[cur]
```

## Step 4: brute
Finally, the group is sufficiently small to brute force $c^x$

```python3
c_pow = one

for _ in range(ord_c):
    cur = (h, c_pow)

    for i in range(n):
        if cur in table:
            x = i * n + table[cur]

            if 0 <= x <= bound:
                if bb.eq(s_eval(bb, g, c, x, one), h):
                    return x

        cur = hol_mul(bb, cur, factor)

    c_pow = bb.mul(c_pow, c)

```

## Wrap-up

This BSGS attack works because we extracted the expenonentiation problem to 

$(g,c)^x = (h,c^k) = target$

Which is the discrete logarithm problem in the semidirect product group

This allows us to use BSGS to compute 

$(g,c)^j\quad\forall \space j\in[0,\sqrt{bound})$

then find a collision such that

$(g,c)^j = (h,c^k)(g,c)^{-ni}$

where $c^k$ is bruteable.

This lets us recover $x = i\cdot n + j$