## if required, install 'Rgitbook' package
# library(devtools)
# install_github("raulossada/Rgitbook")
library(Rgitbook)

## check for local installation of Gitbook
checkForGitbook()

## check for newer version of Gitbook and, if required, install
log_avl <- gitbookInfo()
if (log_avl[2] != log_avl[1])
  installGitbook()

## create new Gitbook
# newGitbook("advancedProgrammingInR")
# initGitbook("advancedProgrammingInR/")
buildGitbook("advancedProgrammingInR/")
