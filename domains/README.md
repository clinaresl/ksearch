# Domain-dependent solvers #

This directory contains a number of domain-dependent solvers that test the
different search algorithms implemented in `libksearch`

## N-Pancake ##

The N-Pancake domain is implemented under `domains/n-pancake`

# Usage #

All solvers acknowledge the flags `--version` and `--help`. Other than these,
they also honour the following directives:

* `--solvers:` Every domain-dependent solver provides a collection of solvers to
  choose from:
  
  - **belA0**: Brute-force Bidirectional Edge-labelling A* search algorithm
  - **mDijkstra**: Brute-force mA* search algorithm 

* `--file`: file with the description of the test set to solve. Details might be
  different among solvers but, in general, they consist of several lines with
  each line describing a different task with the format: `<id> <start state>`.
  The identifier is always given as a number, and the representation of the
  start state depends on the specific solver.
  
* `--variant`: all domains provide, at least, two different variants. One
  considers all edge costs to be the same and always equal to one. Other
  variants might be given for implementing arbitrary cost domains with null-cost
  edges or not.
  
* `--k`: basically, this flag expects the number of paths to compute, e.g., `--k
  1000` would request the computation of one thousand shortest paths. However,
  it can be used also to automate experiments and thus, to provide an arbitrary
  collection of *k* values. A *full specification* consists of a semicolon
  separated list of *single specifications*, where a *single specification*
  consists of three digits: the first represents the initial value of *k*; the
  second is the last values of *k* to consider and the third is the step between
  successive values of *k**. In case the third value is not given, then 1 is
  assumed by default and, in case only one value is given then the second value
  equals the first one by default. For example: `--k 10 50 10; 100 1000 100`
  will run every solver selected with the following values of *k*: 10, 20, 30,
  40, 50 (according to the first *single specification*) and next, 100, 200, 300
  ... 1000.

Additionally, if an argument is given to `--csv`, then a CSV file with that name
recording all results will be created as soon as the execution is over. Finally,
`--verbose` provides additional information, typically consisting of every
single solution to every instance of the *k* shortest-path problem
