
# Towards a structured workflow

### What is it actually? 
Briefly put, the meaning of 'workflow' in a general sense may be summarized as 

<blockquote>
"[...] an abstraction of an automated and computerized business 
process. It consists of a set of activities that are interconnected by control 
flows [...]. Each activity is a naturally defined task in a workflow and has 
associated servers that are either humans or executions of programs commonly 
called processes [...]." (Son and Kim, 2001)
</blockquote>

Accordingly, from an R perspective, the term typically describes a set of 
interconnected scripts and functions, which sequentially perform individual 
tasks to generate a desired result. In the majority of cases, such workflows
rely on certain input data based on which some statistical metrics or figures
shall be produced.

### What's the use of it?
This section will introduce you to two fundamental pillars of a structured 
workflow in R, i.e. 

* [**RStudio projects**](chapters/01_structured_workflow/projects.md) and
* [**GitHub version control**](chapters/01_structured_workflow/version_control.md).

While the former is meant to separate the single projects you are working on 
and, at the same time, keep your stuff together in unique project-related 
working directories, the latter helps you to keep track of your single R code 
snippets and, more precisely, the changes you (or somebody else) made over time.
