
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
> 
> ## create a local data frame
> diamonds <- tbl_df(diamonds)
> diamonds
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
Source: local data frame [2 x 10]

  carat     cut  color clarity depth table price     x     y     z
  (dbl)  (fctr) (fctr)  (fctr) (dbl) (dbl) (int) (dbl) (dbl) (dbl)
1  3.01 Premium      F      I1  62.2    56  9925  9.24  9.13  5.73
2  3.05 Premium      E      I1  60.9    58 10453  9.26  9.25  5.66
```

### Selecting columns via `select`
Selecting specific columns from a data frame follows a similar syntax to 
`filter` and works analogous to SQL standards. Again, the first argument 
represents the data frame under consideration followed by the desired column 
names (without double quotes). 


```r
> select(diamonds, carat, cut, color, clarity)
```

```
Source: local data frame [53,940 x 4]

   carat       cut  color clarity
   (dbl)    (fctr) (fctr)  (fctr)
1   0.23     Ideal      E     SI2
2   0.21   Premium      E     SI1
3   0.23      Good      E     VS1
4   0.29   Premium      I     VS2
5   0.31      Good      J     SI2
6   0.24 Very Good      J    VVS2
7   0.24 Very Good      I    VVS1
8   0.26 Very Good      H     SI1
9   0.22      Fair      E     VS2
10  0.23 Very Good      H     VS1
..   ...       ...    ...     ...
```

Note that both the use of `c` to combine the single column names into a vector 
as well as the need for double quotes became obsolete. In addition to this, 
column names may be treated analogous to numeric indices, eliminating the need 
to count columns when desiring to extract several consecutive columns from a 
rather wide datasets. 


```r
> select(diamonds, carat:clarity, price)
```

```
Source: local data frame [53,940 x 5]

   carat       cut  color clarity price
   (dbl)    (fctr) (fctr)  (fctr) (int)
1   0.23     Ideal      E     SI2   326
2   0.21   Premium      E     SI1   326
3   0.23      Good      E     VS1   327
4   0.29   Premium      I     VS2   334
5   0.31      Good      J     SI2   335
6   0.24 Very Good      J    VVS2   336
7   0.24 Very Good      I    VVS1   336
8   0.26 Very Good      H     SI1   337
9   0.22      Fair      E     VS2   337
10  0.23 Very Good      H     VS1   338
..   ...       ...    ...     ...   ...
```

There's also a set of additional helper functions, including `starts_with`, 
`ends_with`, `matches` and `contains`, which definitely go beyond the scope of 
this short introduction. Still, it's good to know that such things existed in 
case you needed any of them.

### A brief note on chaining (or pipelining)
Now suppose we wanted to select the same columns from the previously created 
subset of data. Traditionally, this would either require a 2-step approach, 
generating otherwise unnecessary intermediate results, via 


```r
> ## first, create subset
> diamonds_sub <- filter(diamonds, carat > 3 &
+                          cut %in% c("Premium", "Ideal") &
+                          color %in% c("D", "E", "F"))
> 
> ## second, select columns
> select(diamonds_sub, carat:clarity, price)
```

or nested function calls which are usually hard to read.


```r
> ## all-in-one nested solution
> select(
+   filter(diamonds, carat > 3 &
+            cut %in% c("Premium", "Ideal") &
+            color %in% c("D", "E", "F")), 
+   carat:clarity, price
+ )
```

**dplyr** introduces the `%>%` operator which is meant to bridge a set of 
connected processing steps, thus eliminating the need for intermediary 
variables ('diamonds_sub') or nested functions. Just think of `%>%` as a "then" 
connecting two parts of a sentence.


```r
> diamonds %>%
+   filter(carat > 3 &
+            cut %in% c("Premium", "Ideal") &
+            color %in% c("D", "E", "F")) %>%
+   select(carat:clarity, price)
```

```
Source: local data frame [2 x 5]

  carat     cut  color clarity price
  (dbl)  (fctr) (fctr)  (fctr) (int)
1  3.01 Premium      F      I1  9925
2  3.05 Premium      E      I1 10453
```

Note that chaining comes in handy when performing multiple operations at once, 
rendering your code much more elegant and reducing the accumulated overhead 
significantly. The single worksteps can be read from left to right and from top 
to bottom, just like you would read the pages of a book. 

### Reordering data frames via `arrange`
`arrange` ensures a fast and easy rearrangement of rows based on certain 
variables. Although this can be done using base-R, its necessity soon becomes 
clear when dealing with rearrangements based on multiple variables as the **dplyr** 
approach requires far less typing.

##### Task #3: base-R rearrangement of rows
Here's a tricky one. Let's assume we wanted to rearrange the rows of a subset of 
'diamonds' (color, price, carat) according to color, with the best color (D) on 
top. Since we're also interested in a preferably low price, the diamonds of 
uniform color should be sorted according to their price, with cheapest ones on 
top. Finally, for each set of diamonds with a specified color and price, we want 
their weights arranged in descending order, with the heaviest specimen on top.

<center>
  <img src="https://pixabay.com/static/uploads/photo/2012/04/14/14/04/hourglass-34048_640.png" alt="hourglass" style="width: 200px;"/>
</center>



Again, the **dplyr** approach is much simpler as it doesn't require to repeat 
the name of the data frame with each newly added variable inside `order`. 


```r
> diamonds %>%
+   select(color, price, carat) %>%
+   arrange(color, price, desc(carat))
```

```
Source: local data frame [53,940 x 3]

    color price carat
   (fctr) (int) (dbl)
1       D   357  0.23
2       D   357  0.23
3       D   361  0.32
4       D   362  0.23
5       D   367  0.24
6       D   367  0.20
7       D   367  0.20
8       D   367  0.20
9       D   373  0.24
10      D   373  0.23
..    ...   ...   ...
```
