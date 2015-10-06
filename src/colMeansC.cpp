#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

// [[Rcpp::export]]
NumericVector colMeansC(NumericMatrix x) {
  
  // number of rows and columns
  int nCol = x.ncol();
  int nRow = x.nrow();
  
  // temporary variable of size nrow(x) to store column values in
  NumericVector nVal(nRow);
  
  // initialize output vector
  NumericVector out(nCol);
  
  // loop over each column
  for (int i = 0; i < nCol; i++) {
    
    // values in current column
    nVal = x(_, i);
    
    // store mean of current 'nVal' in 'out[i]'
    out[i] = mean(nVal);
  }
  
  return out;
}
