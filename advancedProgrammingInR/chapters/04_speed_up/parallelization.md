
# Parallelization

There's actually a whole bunch of packages that allow you to split particular 
tasks to multiple cores on your local machine. Out of those, we decided to focus 
on **doParallel** (in conjunction with **foreach**) as it represents a straightforward 
solution that does not require the user to manually export objects from the 
global environment to the single nodes (e.g. required by **parallel**).

Let's start with detecting the number of cores available on your machine by 
running the following piece of code. 


```r
> ## load 'doParallel' package
> library(doParallel)
> 
> ## no. of local cores
> detectCores()
```

```
[1] 4
```

In my particular case, there are 4 nodes available for parallelization. However, 
this number typically varies between computers. Note also that `library(doParallel)` 
automatically attaches the **foreach** package which we will require later on. 
Next, let's create and register a socket cluster for parallel processing. Just think of 
this as a set of workers among which a particular work step should be divided 
equally. With `registerDoParallel`, we tell R that it should use the created 
cluster for any further multi-core operations performed with `foreach`.


```r
> ## create parallel socket cluster
> cl <- makeCluster(3)
> 
> ## register parallel backend
> registerDoParallel(cl)
```

You will probably notice that we told R to use 3 nodes to work with. Why's he 
doing that?, you might wonder since our machine is equipped with 4 nodes in 
total. Well, parallel processing distributed among all nodes available tends to 
slow down your computer considerably. Since we'd possibly like to perform some 
other actions while R is occupied (e.g. browsing the internet, checking e-mails, 
etc.), we should leave some remaining computational power for such operations as 
well.  

Now that we're set up properly, it's time to give our cluster something to work 
with. Remember the previous example on calculating the linear relationship 
between 'carat' and 'price' iteratively? We're gonna do the same thing again now 
except this time it's `foreach` we'll be working with. Similar to `*apply`, 
`foreach` requires a set of input data and a function to perform on the very same. 
Have a look at the following example. 


```r
> foreach(i = 1:4) %do% sqrt(i)
```

```
[[1]]
[1] 1

[[2]]
[1] 1.414214

[[3]]
[1] 1.732051

[[4]]
[1] 2
```

Similar to `lapply`, `foreach` returns an object of class 'list' by default. We 
may change this by telling R how to `.combine` the output of each iteration via 


```r
> foreach(i = 1:4, .combine = "c") %do% sqrt(i)
```

```
[1] 1.000000 1.414214 1.732051 2.000000
```

et voilà, we transformed the `lapply`-style list output of our loop into an 
`sapply`-style vector output.
