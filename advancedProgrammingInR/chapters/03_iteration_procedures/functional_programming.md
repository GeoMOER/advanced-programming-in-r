
# Functional programming

As mentioned in the previous chapter, R is at its core a functional programming language. Put more general, we can say that

* everything that exists is an object
* everything that happens is a function call

### Custom functions

The biggest draw-back of point-and-click statistics software is that they are usually limited in the functionality they provide. The provide you with a suite of pre-defined analysis tools and algorithms but usually it is rather tedious to extend these. R on the other hand is a full-blown programming language which means that there are next to no limits to what you can do. One of the most important features to expand existing functionality is to write your own functions. Functions that do not exist elsewhere. In fact, this is the main reason behind R package development.

So, let's try and create a custom function. A function to calculate the Pythogorean Theorem does not exist in base R. Sure it may exist in some package somewhere, but I argue that it is much easier and quicker to write this yourself.


```r
pythagoreanTheorem <- function(a, b) {
  c <- sqrt(a*a + b*b)
  return(c)
}

pythagoreanTheorem(3, 4)
```

```
[1] 5
```

Easy!

Creating custom functions always follows the same procedure:

1. provide a meaningful name and use `<-` to assign `function(x, y, z)` where
2. `x, y, z` are an arbitrary number of named arguments (need not be x, y, z) that are needed for the calculation(s) in the 
3. function body that does all the calculation(s) using the supplied arguments
4. a `return()` call to specify what the function will return. If this is not supplied, the result of the last calculation is returned

Now it is time for you to try this yourselves.

### Task #1: write your own
In R we can easily calculate a pupolations standard deviation around the the mean using `sd()` but there is no default implementation for the [standard error of the mean](https://en.wikipedia.org/wiki/Standard_error). Therefore, it is up to you to write one now. 

Note, there are far more standard error statistics for which R does not provide standard base functions, such as the [root mean square error RMSE](https://en.wikipedia.org/wiki/Root-mean-square_deviation) or the [absolute error AE](https://en.wikipedia.org/wiki/Approximation_error). Therefore, if you're keen go ahead and practice writing functions to provide these.

<center>
  <img src="https://pixabay.com/static/uploads/photo/2012/04/14/14/04/hourglass-34048_640.png" alt="hourglass" style="width: 200px;"/>
</center>

-------

### Functionals

We've already seen functionals, functions that take other functions as arguments. However, so far we've only used these with standard base R functions. But we can also supply a custom function to a functional.


```r
dat <- data.frame(a = c(3, 7, 11, 1, 24, 2),
                  b = c(4, 3, 2, 3, 12, 5))

sapply(seq(nrow(dat)), function(i) pythagoreanTheorem(dat[i, 1], dat[i, 2]))
```

```
[1]  5.000000  7.615773 11.180340  3.162278 26.832816  5.385165
```

Given that R comes equipped with a great variety of `*apply()` functionals, it is usually not necessary to write a functional yourself. 

-------

### Closures

The counterpart to functionals are so-called closures. These are functions that return (or build) a function according to some supplied argument. To illustrate this, let's consider the following situation:

We have a bunch of possible predictor variables and a bunch of response variables and we want to figure out the best combination in explaining the variances. 


```r
### generate some random data
set.seed(123)
pred <- data.frame(pred1 = rnorm(100, 2, 1),
                   pred2 = 1:100,
                   pred3 = rpois(100, 2),
                   pred4 = 200:101)

set.seed(234)
resp <- data.frame(resp1 = 1:100,
                   resp2 = rnorm(100, 2, 1),
                   resp3 = 200:101,
                   resp4 = rpois(100, 2))
```

We could simply use copy and paste to claculate each combination.


```r
summary(lm(resp$resp1 ~ pred$pred1))$r.squared
summary(lm(resp$resp2 ~ pred$pred1))$r.squared
summary(lm(resp$resp2 ~ pred$pred1))$r.squared
summary(lm(resp$resp4 ~ pred$pred1))$r.squared

summary(lm(resp$resp1 ~ pred$pred2))$r.squared
summary(lm(resp$resp2 ~ pred$pred2))$r.squared
summary(lm(resp$resp3 ~ pred$pred2))$r.squared
summary(lm(resp$resp4 ~ pred$pred3))$r.squared

#.... and so forth
```

This is far from being optimal. Despite the fact that we have to type a lot, we are very prone to introduce errors (can you spot them?) and it is particularly hard to debug. Here, defining a closure can be of great help.


```r
### define closure
calcRsq <- function(pred) {

  function(y) {
    summary(lm(y ~ pred))$r.squared
  }

}
```

We now have a universal way of defining functions to calculate R-squared values


```r
## create function using pred$v1 as predictor
calcRsq_pred1 <- calcRsq(pred$pred1)

calcRsq_pred1(resp$resp1)
```

```
[1] 0.006369369
```

Using it explicitly like above doesn't really help us much, although we have made sure to not introduce any errors related to the respective predictor being used as this is now fixed within the function `calcRsq_pred1()`. However, given that we now have a function which calculates the R-squared value between a fixed predictor and whatever response we give it, we can now use a functional such as `apply()` to calculate the relationship between the predictor and a bunch of responses.


```r
apply(resp, 2, calcRsq_pred1)
```

```
      resp1       resp2       resp3       resp4 
0.006369369 0.012977823 0.006369369 0.038727184 
```

But why stop here, taking advantage of `sapply()` we can calculate every possible combination in one go.


```r
sapply(seq(ncol(pred)), function(i) {
  f <- calcRsq(pred[, i])
  apply(resp, 2, f)
})
```

```
             [,1]        [,2]        [,3]        [,4]
resp1 0.006369369 1.000000000 0.006326898 1.000000000
resp2 0.012977823 0.012414970 0.006192994 0.012414970
resp3 0.006369369 1.000000000 0.006326898 1.000000000
resp4 0.038727184 0.002312083 0.007719519 0.002312083
```

In words, we 

1. iterate over the columns of pred - `seq(ncol(pred))`
2. define a function `f` by setting the closure to use the column of the current iteration - `f <- calcRsq(pred[, i])`
3. apply this function `f` to all columns of resp - `apply(resp, 2, f)`

The result is equivalent to what `cor(resp, pred)` produces.


```r
cor(resp, pred)^2
```

```
            pred1       pred2       pred3       pred4
resp1 0.006369369 1.000000000 0.006326898 1.000000000
resp2 0.012977823 0.012414970 0.006192994 0.012414970
resp3 0.006369369 1.000000000 0.006326898 1.000000000
resp4 0.038727184 0.002312083 0.007719519 0.002312083
```

So you see that the combination of functionals and closures is a powerful, flexible and elegant way of generalising computations. In fact, it is so flexible that we can now use the same functions for any data frames.


```r
df1 <- diamonds[, c(1, 5, 6)]
df2 <- diamonds[, 7:10]

sapply(seq(ncol(df1)), function(i) {
  f <- calcRsq(df1[, i])
  apply(df2, 2, f)
})
```

```
           [,1]         [,2]       [,3]
price 0.8493305 0.0001133672 0.01616303
x     0.9508088 0.0006395460 0.03815939
y     0.9057751 0.0008608750 0.03376779
z     0.9089475 0.0090105434 0.02277947
```

This is especially valuable for large calculations that require iterating over a set of objects. A classic scenario for using closures in combination with functionals is to find a 'best' value, i.e. some parameter that optimises a fit or something along those lines.

A very detailed tutorial on how to use functional programming in R can be found in Hadley Wickham's book [Advanced R](http://adv-r.had.co.nz/Functional-programming.html). This goes quite a bit deeper than what is outlined here and has a rather neat example of how to use functional programming to flexibly deal with encodings for missing data (e.g. -99, -999, -9999, ...).
