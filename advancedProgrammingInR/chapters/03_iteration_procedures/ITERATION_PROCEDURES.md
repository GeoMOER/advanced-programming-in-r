
# Iteration procedures

In computer programming the term iterative process usually refers to some block of code being repeated more than once to achieve a result. Two terms are frequently used in this context:

* Iteration
* Recursion

In this part of the tutorial we will have a closer look at **Iteration** and how to achieve it in numerous ways in R. In general, Iteration executes a certain function or combination of functions repeatedly until some condition is met.

* The classical `for`-loop is common in pretty much all programming languages, 
* while `*apply` is pretty exclusive to R. 
* Finally, given that R is a so-called functional programming language, we will also see how we can use `functionals` and `closures` which are very elegant and flexible structures to iteratively achieve a certain aim.

-----

**Recursion** is another aspect of repeated computation, however, we will not delve into any detail on this topic here. In contrast to Iteration, recursive structures define functions that call themselves until a certain condition is met. Below is a classical example of a recursive function, the so-called `quicksort` algorithm. This function sorts a numerical vector from small to large values by 

* first selecting an arbitrary value from the vector (in the below case the first) called the pivot
* rearranging the remainding values so that all values smaller than pivot come before it and all larger values after it
* repeating this procedure within the function by calling itself for either side of the pivot.


* Once there are only single values left on either side of the pivot, the function stops

Note, without the inherent stopping condition this function will run for eternity.

This particular example of quicksort is adapted from [Rosetta Code](http://rosettacode.org/wiki/Quicksort)


```r
qsort <- function(v) {
  if (length(v) > 1) {
    pivot <- v[1]
    c(qsort(v[v < pivot]), v[v == pivot], qsort(v[v > pivot])) 
  } else v
}

qsort(rnorm(10))
```

```
 [1] -0.68554516 -0.41921218 -0.29225789 -0.21221874  0.04981660
 [6]  0.07262787  0.46414326  0.85004373  1.35735729  1.55588778
```

Because they call themselves, recursive functions are inherently hard to code, understand and debug. Therefore, we leave it at this small example and continue with the more understandable structures of iterative comutation, namely `for`-loops, `*apply` functions and `functional programming`.
