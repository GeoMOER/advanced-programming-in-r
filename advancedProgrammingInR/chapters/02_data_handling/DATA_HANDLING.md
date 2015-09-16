
# Data handling and visualization

As noted earlier in this guide, an R workflow basically consists of performing 
calculations and/or visualizing your data which has previously been imported into 
R. In the majority of cases, this workflow usually includes an intermediary step 
referred to as data manipulation meant to get your data into shape for 
visualization. Note that some packages even require you to provide the data to 
be displayed in a particular format (of which more later on).  

Within the scope of this short course, we'll simply assume that you already 
heard a lot about importing your data into R. Even if this were not the case, 
there's plenty of tutorials on the internet demonstrating the use of `read.table` 
and the like. Help on this topic may be found e.g. on 
[Quick-R](http://www.statmethods.net/input/importingdata.html), 
[CRAN](https://cran.r-project.org/doc/manuals/r-release/R-data.html), 
[R-Tutor](http://www.r-tutor.com/r-introduction/data-frame/data-import) and so 
forth. Just remember, Google is your friend &#9786;

### Data manipulation
A hodgepodge of typical data manipulation steps including illustrative examples 
is given at [Cookbook for R](http://www.cookbook-r.com/Manipulating_data/) and 
includes, amongst others,

* **General operations**
    * Sorting
    * Converting between vector types
    * Dealing with duplicate records and missing values
* **Factor operations**
    * Renaming and re-computing factor levels
    * Reordering factor levels (which comes in quite handy when it comes to plotting)
* **Data frame operations**
    * Adding and removing columns
    * Reordering columns
    * Merging data frames
* **Restructuring data**
    * Data format conversions
    * Summarizing data

Assuming that you're already familiar with the basics of data manipulation, we'll 
primarily focus on the latter point which represents an essential step towards 
conveying your results clearly and vividly to a broader audience. 

### The 'diamonds' dataset
In the following, we'll demonstrate the essentials of data restructuring in R on 
the basis of the `diamonds` dataset included in **ggplot2**. In case you're not 
already familiar with the dataset, just take a minute to have a brief look at 
what it's all about. Also, feel free to try out `?diamonds` in order to get a 
more detailed description of the single variables.


```r
> ## load 'ggplot2' package
> library(ggplot2)
> 
> ## show the first 6 rows of 'diamonds'
> head(diamonds)
```

```
  carat       cut color clarity depth table price    x    y    z
1  0.23     Ideal     E     SI2  61.5    55   326 3.95 3.98 2.43
2  0.21   Premium     E     SI1  59.8    61   326 3.89 3.84 2.31
3  0.23      Good     E     VS1  56.9    65   327 4.05 4.07 2.31
4  0.29   Premium     I     VS2  62.4    58   334 4.20 4.23 2.63
5  0.31      Good     J     SI2  63.3    58   335 4.34 4.35 2.75
6  0.24 Very Good     J    VVS2  62.8    57   336 3.94 3.96 2.48
```

```r
> ## show the structure of 'diamonds'
> str(diamonds)
```

```
'data.frame':	53940 obs. of  10 variables:
 $ carat  : num  0.23 0.21 0.23 0.29 0.31 0.24 0.24 0.26 0.22 0.23 ...
 $ cut    : Ord.factor w/ 5 levels "Fair"<"Good"<..: 5 4 2 4 2 3 3 3 1 3 ...
 $ color  : Ord.factor w/ 7 levels "D"<"E"<"F"<"G"<..: 2 2 2 6 7 7 6 5 2 5 ...
 $ clarity: Ord.factor w/ 8 levels "I1"<"SI2"<"SI1"<..: 2 3 5 4 2 6 7 3 4 5 ...
 $ depth  : num  61.5 59.8 56.9 62.4 63.3 62.8 62.3 61.9 65.1 59.4 ...
 $ table  : num  55 61 65 58 58 57 57 55 61 61 ...
 $ price  : int  326 326 327 334 335 336 336 337 337 338 ...
 $ x      : num  3.95 3.89 4.05 4.2 4.34 3.94 3.95 4.07 3.87 4 ...
 $ y      : num  3.98 3.84 4.07 4.23 4.35 3.96 3.98 4.11 3.78 4.05 ...
 $ z      : num  2.43 2.31 2.31 2.63 2.75 2.48 2.47 2.53 2.49 2.39 ...
```
