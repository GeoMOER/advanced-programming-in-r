
# Summarizing data

The **dplyr** package is a marvelous and very intuitive set of tools to perform 
fast and easy data manipulation and summarization in preparation e.g. for 
visualization. In the following, we'll guide you through a selection of 
operations you may find helpful within the course of future R workflows on the 
basis of the 'diamonds' dataset. Based on a very comprehensive 
[RPubs tutorial](https://rpubs.com/justmarkham/dplyr-tutorial), these include

* `filter`, 
* `select`, 
* `arrange`, 
* `mutate`, 
* `summarise` and 
* `group_by`, 

each of which will be accompanied by an equivalent base-R approach.

### A brief note on `tbl_df`
You probably noticed earlier that 'diamonds' consists of a rather large number 
of observations. 


```r
> ## number of columns and rows of 'diamonds'
> dim(diamonds)
```

```
[1] 53940    10
```

Unless you are willing to stumble into truncated console output when typing 
`diamonds` (please don't do that), you are inevitably required to perform 
`head(diamonds)` or `str(diamonds)` in order to get an impression of the data 
you are dealing with. Along with **dplyr** comes a function called `tbl_df` that 
helps to overcome such ritual-like investigation procedures. 


```r
> ## required package
> library(dplyr)
```

```

Attaching package: 'dplyr'

The following objects are masked from 'package:stats':

    filter, lag

The following objects are masked from 'package:base':

    intersect, setdiff, setequal, union
```

```r
> ## create a local data frame
> tbl_df(diamonds)
```

```
Source: local data frame [53,940 x 10]

   carat       cut  color clarity depth table price     x     y     z
   (dbl)    (fctr) (fctr)  (fctr) (dbl) (dbl) (int) (dbl) (dbl) (dbl)
1   0.23     Ideal      E     SI2  61.5    55   326  3.95  3.98  2.43
2   0.21   Premium      E     SI1  59.8    61   326  3.89  3.84  2.31
3   0.23      Good      E     VS1  56.9    65   327  4.05  4.07  2.31
4   0.29   Premium      I     VS2  62.4    58   334  4.20  4.23  2.63
5   0.31      Good      J     SI2  63.3    58   335  4.34  4.35  2.75
6   0.24 Very Good      J    VVS2  62.8    57   336  3.94  3.96  2.48
7   0.24 Very Good      I    VVS1  62.3    57   336  3.95  3.98  2.47
8   0.26 Very Good      H     SI1  61.9    55   337  4.07  4.11  2.53
9   0.22      Fair      E     VS2  65.1    61   337  3.87  3.78  2.49
10  0.23 Very Good      H     VS1  59.4    61   338  4.00  4.05  2.39
..   ...       ...    ...     ...   ...   ...   ...   ...   ...   ...
```

Notice that not only the class and the dimensions of the dataset are displayed, 
but also the variable types of the single columns. Furthermore, the console 
output is truncated after the first 10 rows (which would also apply to the 
displayed columns if we were dealing with a somewhat wider dataset). Practically, 
this means that the appropriate use of `tbl_df` lets you kill two birds with one 
stone compared with the 2-step approach via `head` and `str` shown above. 

### Subsetting data via `filter`
Just like base-R `subset` or simply indexing via conditional expressions 
included in square brackets, `filter` creates a subset of your data based on 
some user-defined criteria. For the time being, let's have a look at your 
subsetting abilities.

##### Task #2: base-R subsetting
Suppose we want to create a subset of 'diamonds' keeping all D-, E- or F-colored 
specimen with a 'Premium' or 'Ideal' cut quality and a weight of more than 3 
carat. Try to figure out two different solutions, one <u>with</u> and one 
<u>without</u> using `subset`. 

<center>
  <img src="https://pixabay.com/static/uploads/photo/2012/04/14/14/04/hourglass-34048_640.png" alt="hourglass" style="width: 200px;"/>
</center>



Using `filter` is straightforward and very similar to `subset`. The first 
argument represents the data frame, whereas any subsequent argument represents a 
logical expression to filter particular rows. Note that in contrast to the 
direct subsetting via square brackets, you are not required to repeat the name 
of the data frame with every single condition.


```r
> filter(diamonds, carat > 3 &
+          cut %in% c("Premium", "Ideal") &
+          color %in% c("D", "E", "F"))
```

```
  carat     cut color clarity depth table price    x    y    z
1  3.01 Premium     F      I1  62.2    56  9925 9.24 9.13 5.73
2  3.05 Premium     E      I1  60.9    58 10453 9.26 9.25 5.66
```
