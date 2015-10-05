
# Parallelization

As regards multi-core operations, there's actually a whole bunch of packages 
that allow you to distribute particular processing steps to multiple cores on 
your local machine. Out of those, we decided to focus on **doParallel** (in 
conjunction with **foreach**) as it represents a straightforward solution that 
does not require the user to manually export objects from the global environment 
to the single nodes (e.g. required by `par*apply` along with **parallel**).

### Getting started
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
the **foreach** package which we will require later on, so there's no need to 
manually load it. In fact, this would only be required if we decided to use 
`foreach` on a single core, but for such operations, we highly recommend to use 
`*apply`.

Next, let's create a socket cluster for parallel processing via `makeCluster`. 
Think of this as a set of workers among which a particular work step should be 
divided equally - just like in the image on the previous page. With 
`registerDoParallel`, we tell R that it should use the cluster 'cl' for any 
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

### The `foreach` syntax
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

Let's now come back to the 'diamonds' dataset. Since you may just take the 
function body from the previous application with `lapply`, it's up to you to 
calculate the linear model between 'carat' and 'price' for each group of 'cut'. 
Remember to `split` the 'diamonds' dataset first and pass the thus created list 
(with each list entry representing a group of uniform cuts) to `foreach`.

<center>
  <img src="https://pixabay.com/static/uploads/photo/2012/04/14/14/04/hourglass-34048_640.png" alt="hourglass" style="width: 200px;"/>
</center>



### How *not* to use parallel features
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
Warning: closing unused connection 10 (<-localhost:11160)
```

```
Warning: closing unused connection 9 (<-localhost:11160)
```

```
Warning: closing unused connection 8 (<-localhost:11160)
```

```
Unit: milliseconds
                                                              expr      min       lq     mean
 {     foreach(i = ls_diamonds) %do% lm(carat ~ price, data = i) } 51.06636 51.41955 52.12576
   median       uq      max neval
 51.82103 52.27765 54.32115    20
```

Hm, quite some time. Let's see how long it takes on when using multiple cores. 
The only thing that's required in order to let this operation run on multiple 
cores is to replace `%do%` with `%dopar%`. In doing so (and distributing the 
iterative `lm` calculation on multiple cores), the operation should perform 
much faster, right?


```r
> microbenchmark({
+   foreach(i = ls_diamonds) %dopar% lm(carat ~ price, data = i)
+ }, times = 20L)
```

```
Unit: milliseconds
                                                                 expr      min       lq     mean
 {     foreach(i = ls_diamonds) %dopar% lm(carat ~ price, data = i) } 138.7644 151.5377 210.7941
   median       uq     max neval
 177.3854 191.2622 916.877    20
```

Oops, what's going on now? Obviously, this action doesn't perform faster at all 
although we told R to run in parallel via `%dopar%`. In fact, this is a bad 
example for the parallelized use of `foreach`. `lm` is a highly optimized base-R 
function that performs quite fast without the need to go parallel. "With [such] 
small tasks, the overhead of scheduling the task and returning the result can be 
greater than the time to execute the task itself, resulting in poor 
performance." (Weston and Calaway, 2014)

### How to use parallel features
Now that you've learned how *not* to use `%dopar%`, let's see what a proper use 
would look like. 'Proper' in this prospect refers to a piece of code that is 
computationally expensive and needs to be repeated at least a couple of times. 
For example, let's assume we wanted to separately predict 'cut', 'color' and 
'carat' for each specimen from 'diamonds' based on all remaining variables. 
Using `foreach` (or `lapply`), the referring code would roughly look as follows.



```r
> system.time({
+   
+   ## conditional inference trees 
+   ls_ct <- foreach(i = c("cut", "color", "carat")) %do% {
+   
+     # formula
+     frml <- as.formula(paste(i, ".", sep = " ~ "))
+   
+     # classification
+     ctree(frml, data = diamonds,
+           controls = ctree_control(testtype = "MonteCarlo",
+                                    nresample = 999,
+                                    mincriterion = 0.999,
+                                    maxdepth = 3))
+   }
+ })
```

```
   user  system elapsed 
 24.482   0.010  24.483 
```

`ctree` performs rather slowly, which is particularly owing to `nresample` that 
tells the function to perform 1000 internal Monte-Carlo replications. Luckily, 
we can easily split this operation into 3 parts, i.e. one node takes 'cut' as 
response variable, another node 'color' and a third node 'carat' - at the same 
time!


```r
> system.time({
+   
+   ## conditional inference trees 
+   ls_ct <- foreach(i = c("cut", "color", "carat"), 
+                    .packages = c("ggplot2", "party")) %dopar% {
+   
+     # formula
+     frml <- as.formula(paste(i, ".", sep = " ~ "))
+   
+     # classification
+     ctree(frml, data = diamonds,
+           controls = ctree_control(testtype = "MonteCarlo",
+                                    nresample = 999,
+                                    mincriterion = 0.999,
+                                    maxdepth = 3))
+   }
+ })
```

```
   user  system elapsed 
  4.230   0.408  20.828 
```

Of course, this is seconds we are talking about. Nonetheless, everyone of you 
will eventually end up with quite big datasets upon which computationally 
expensive operations need to be performed in an iterative manner. It might be 
hours or even days (trust us, we know what we're talking about...) that one 
single operation takes to perform - and then, the `%dopar%` might come in quite 
handy.

### Closing a parallel backend
One final remark on the proper use of parallel backends in R. When working on 
multiple cores, you can easily lose track of how many parallel backends, if any, 
you registered during your current session, especially when some error prevents 
your script from finishing. If you should ever find yourself in such a 
situation, do not hesitate to use `showConnections` to print information on 
currently open connections to the R console. 


```r
> showConnections()
```

```
   description         class            mode  text     isopen   can read can write
3  "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
4  "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
5  "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
7  "output"            "textConnection" "wr"  "text"   "opened" "no"     "yes"    
11 "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
12 "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
13 "<-localhost:11160" "sockconn"       "a+b" "binary" "opened" "yes"    "yes"    
```

There's 3 socket connections (i.e. cores) registered at the moment, just as we 
initially defined via `registerDoParallel`. In order to close these connections 
(which we recommend to consider at the end of each parallelized R script), 
simply perform 


```r
> stopImplicitCluster()
```

which explicitly deregisters the implicitly created cluster.
