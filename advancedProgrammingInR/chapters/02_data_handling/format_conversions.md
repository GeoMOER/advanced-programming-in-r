
# Data format conversions

Usually, your data is arranged in matrix format with rows and columns 
representing observations and variables, respectively. This is the common case 
not only for R, but also for many other statistical software packages, including 
Excel and SPSS. 
However, certain R packages, including visualization via **ggplot2**, are easier to handle when 
using a long data format rather than a wide one, thus requiring a "rearrangement of the form, but not the content, of the 
data" (Wickham, 2007) according to so-called 
identifier (or ID) and measured variables. Here's a simple example taken from 
[Cookbook for R](http://www.cookbook-r.com/Manipulating_data/Converting_data_between_wide_and_long_format/) 
to illustrate this point. Let's start off with the wide format:


```r
olddata_wide <- read.table(header = TRUE, text = '
 subject sex control cond1 cond2
       1   M     7.9  12.3  10.7
       2   F     6.3  10.6  11.1
       3   F     9.5  13.1  13.8
       4   M    11.5  13.4  12.9
')
```

Notice the strict structure of the `data.frame` with observations of subjects 
1-4 (two males and two females) arranged in rows and measured variables ('control', 
'cond1', 'cond2') in columns? Now, let's see what the long version of the 
dataset would look like:


```r
library(reshape2)
melt(olddata_wide, id.vars = c("subject", "sex"))
```

```
   subject sex variable value
1        1   M  control   7.9
2        2   F  control   6.3
3        3   F  control   9.5
4        4   M  control  11.5
5        1   M    cond1  12.3
6        2   F    cond1  10.6
7        3   F    cond1  13.1
8        4   M    cond1  13.4
9        1   M    cond2  10.7
10       2   F    cond2  11.1
11       3   F    cond2  13.8
12       4   M    cond2  12.9
```

What just happened? `melt` (or, more precisely, `melt.data.frame`) from the 
**reshape2** package was cast upon the dataset, forcing an increase in `nrow` to 
the expense of `ncol`. Columns 'subject' and 'sex' were thereby specified as 
ID variables, which prevented them from being split apart. The remaining variables, 
on the other side, now got arranged among each other rather than next to each other, 
with each row representing a unique ID-variable combination. 
You will notice later on that such a format is much easier to handle when it 
comes to visualizing grouped variables.

##### Task #5: Melting 'diamonds'
The 'diamonds' dataset included in **ggplot2** features quite a variety of 
variables per specimen (see `?diamonds` for further information). For the time 
being, let's assume we want to convert the dataset from wide into long format, 
thus resulting in a significant reduction of columns. Since no 'true' ID column 
is included so far, create a new column 'ID' from `rownames(diamonds)` and 
subsequently `melt` the dataset using all factor columns and the newly 
established ID column as `id.vars`.

<center>
  <img src="https://upload.wikimedia.org/wikipedia/commons/2/25/Hourglass_2.svg" alt="hourglass" style="width: 200px;"/>
</center>
