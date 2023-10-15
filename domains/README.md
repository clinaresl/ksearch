# Domain-dependent solvers #

This directory contains a number of domain-dependent solvers that test the
different search algorithms implemented in `libksearch`

## N-Pancake ##

The N-Pancake domain is implemented under `domains/n-pancake`

## Grid ##

The Grid domain is implemented under `domains/grid`

## Roadmap ##

The Roadmap domain is implemented under `domains/roadmap`

# Usage #

All solvers acknowledge the flags `--version` and `--help`. Other than these,
they also honour a number of mandatory and optional directives.

Typically, the mandatory arguments are the following:

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
  successive values of *k*. In case the third value is not given, then 1 is
  assumed by default and, in case only one value is given then the second value
  equals the first one by default. For example: `--k "1 5; 10 50 10; 100"` will
  run every solver selected with the following values of *k*: 1, 2, 3, 4 and 5
  (according to the first *single specification*), 10, 20, 30, 40, 50 (as given
  in the second *single specification*) and finally 100.

The typical optional arguments are shown next:

* `--csv`: if given, a CSV file with the specified name is created which records
  all results as soon as the execution is over.
  
* `--no-doctor`: once every optimization task is finished, the solver
  automatically check it for correctness ---see the following section.
  Sometimes, if the number of paths generated is too large, this might take a
  very long time. This directive suppress the automated verification.

* `--verbose`: provides additional information, typically consisting of every
  single solution to every instance of the *k* shortest-path problem
  
However, some specific solvers might have a different selection of directives.
For a detailed description of them, see the corresponding directories listed at
the top of this entry.

# Error checking #

After every execution, the solver will take care of the correctness of the
solutions found. Various verifications are automatically carried out, both at
the level of every single solution path, or the solution of the *k*
shortest-path problem.

In particular, the following conditions are automatically verified for every
single solution path found:

* The number of *expanded nodes* is not null unless the start state equals the
  goal state
  
* Every solution path found effectively starts at the specified start state

* Every solution path found effectively ends at the specified goal state

* Every solution path consists of states which are adjacent to each other, i.e.,
  every state given in every solution path is a descendant of the previous state
  
* The solution cost of every solution path found equals the cost of the solution
  declared by the search algorithm, i.e., its *g*-value.
  
The following conditions are verified automatically for every solution of a *k*
shortest-path problem ---which consists of *k* single solution paths:

* The cost of the solution paths grows monotonically, i.e., the algorithm used
  for solving the task is *stable*
  
* That there are no duplicate solution paths, i.e., all *k* solution paths are
  distinct
  
* The number of solution paths found equals *k*. It should be noted, however,
  that this might not be an error and in some **directed** graphs, the number of
  paths between two designated vertices is bounded.

# Reporting #

As noted above, when using the directive `--csv` a full report with the solution
of every task is automatically created. The following information is given:

* *Domain*: Name of the domain, e.g., `n-pancake`
* *Variant*: Name of the variant used, e.g., `unit`, `heavy-cost`, etc.
* *Id*: every id consists of two identifiers separated by a slash. The first one is the task id, and the second one is the path id of the path solution found, e.g., `013/092` refers to the stats of the 92-th solution path found in task `013`.
* *k*: requested number of paths to compute. This value serves as an upper bound in the second id given in the previous column.
* *start*: explicit representation of the start state
* *goal*: explicit representation of the goal state
* *h0*: heuristic value of the start state
* *length*: number of transitions in the solution path
* *cost*: cost of the solution path. Note that *length* and +cost* must be equal
  in unit cost domains, but maybe different in arbitrary cost domains.
* *expansions*: number of expanded nodes for finding one specific solution path. One solving a *k* shortest-path problem, this value might be increasing between successive solution paths, but this is not necessarily true always.
* *runtime*: CPU runtime until this solution path is found. Also, this value
  might be monotonically increasing between successive solutions of the same *k*
  shortest-path problem, but it is not necessarily true.  
* *expansions/sec*: it is just the quotient of the two previous columns. This
  value is given only to provide an overall idea of the efficiency of the
  implementations provided.
* *solver*: signature of the solver used for computing this solution
* *doctor*: output of the automated error checking process. See the previous section

# License #

MIT License

Copyright (c) 2016, 2023, Carlos Linares López

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


# Author #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>
