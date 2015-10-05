
# Parallelization

As regards multi-core operations, there's actually a whole bunch of packages 
that allow you to distribute particular processing steps to multiple cores on 
your local machine. Out of those, we decided to focus on **doParallel** (in 
conjunction with **foreach**) as it represents a straightforward solution that 
does not require the user to manually export objects from the global environment 
to the single nodes (e.g. required by **parallel**).

Let's start straight away with detecting the number of cores available on your 
machine via `detectCores`. In the end, there's no use in trying to split a 
certain task into 5 parts when there are only 4 horses available, is it? &#128521; 


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

In our case, there are 4 nodes available for parallelization among which 
iteration procedures may be divided. Note that this number typically varies 
between computers. Note also that `library(doParallel)` automatically attaches 
the **foreach** package which we will require later on. 

Next, let's create a socket cluster for parallel processing via `makeCluster`. 
Think of this as a set of workers among which a particular work step should be 
divided equally - just like in the image on the previous page. With 
`registerDoParallel`, we tell R that it should use the created cluster for any 
further multi-core operations performed with `foreach`.


```r
> ## create parallel socket cluster
> cl <- makeCluster(3)
> 
> ## register parallel backend
> registerDoParallel(cl)
```

You will probably notice that we told R to use 3 nodes to work with. Why not use  
all 4 nodes?, you might wonder. Well, parallel processing distributed among all 
nodes available tends to slow down your computer considerably. Since we'd 
possibly like to perform some other actions while R is occupied (e.g. browsing 
the internet, checking e-mails, etc.), it is wise to leave some remaining 
computational power for such operations as well.  

Now that we're set up properly, it's time to give our cluster something to work 
with. Remember the previous example on calculating the linear relationship 
between 'carat' and 'price' for each 'cut' quality iteratively? We're gonna do the same thing again now 
except this time it's `foreach` we'll be working with. Similar to `*apply`, 
`foreach` requires a set of input data and a function to perform on the very same. 
Have a look at the following example. 


```r
> ## calculate square root, return list
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

Similar to `lapply`, `foreach` returns an object of class 'list' by default. The 
function body, by contrast, is connected with the part on variable 
definition via the binary operator `%do%`. This might seem odd at first, but you 
will notice later on that this syntax comes in quite handy when forwarding 
`foreach`-related operations to a parallel cluster. 

We may easily change the output type of `foreach` by telling R how to `.combine` 
the results of the single iterations via 


```r
> ## calculate sqare root, return vector
> foreach(i = 1:4, .combine = "c") %do% sqrt(i)
```

```
[1] 1.000000 1.414214 1.732051 2.000000
```

et voilà, we transformed the `lapply`-style list output of our loop into an 
`sapply`-style vector output.  

Since you may just take the function body from the previous application with 
`lapply`, it's now up to you to calculate the linear model between 'carat' and 
'price' for each group of 'cut'. Remember to `split` the 'diamonds' dataset 
first and pass the thus created list (with each list entry representing a group 
of uniform cuts) to `foreach`.

<center>
  <img src="https://pixabay.com/static/uploads/photo/2012/04/14/14/04/hourglass-34048_640.png" alt="hourglass" style="width: 200px;"/>
</center>



Let's see how long this code chunk takes to perform. The **microbenchmark** 
package is just the right thing for such an operation.


```r
> ## load 'microbenchmark' package
> library(microbenchmark)
> 
> ## load 'ggplot2' and split 'diamonds' dataset
> library(ggplot2)
> ls_diamonds <- split(diamonds, f = diamonds$cut)
> 
> ## speed test (this might take some time)
> microbenchmark({
+   foreach(i = ls_diamonds) %do% lm(carat ~ price, data = i)
+ }, times = 20L) # number of times to evaluate the expression
```

```
Warning: closing unused connection 13 (<-localhost:11930)
```

```
Warning: closing unused connection 12 (<-localhost:11930)
```

```
Warning: closing unused connection 11 (<-localhost:11930)
```

```
Unit: milliseconds
                                                              expr
 {     foreach(i = ls_diamonds) %do% lm(carat ~ price, data = i) }
      min       lq     mean   median       uq      max neval
 42.55027 45.54159 54.74253 47.64679 49.23392 125.9018    20
```

Hm, quite some time... if we could only speed things up a little b---
Oh wait, WE CAN!


```r
> microbenchmark({
+   foreach(i = ls_diamonds) %dopar% lm(carat ~ price, data = i)
+ }, times = 20L)
```

```
Unit: milliseconds
                                                                 expr
 {     foreach(i = ls_diamonds) %dopar% lm(carat ~ price, data = i) }
      min      lq    mean   median       uq      max neval
 118.8078 156.919 220.579 189.4326 218.6658 921.4322    20
```

By replacing the binary operator `%do%` with `%dopar%`, we can tell R to outsource the `lm` task and operate on the initially created cluster `cl`. Since we're  
