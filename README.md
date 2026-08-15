# Introduction #

This library implements a number of search algorithms to compute *K* *simple*
and *non-simple* optimal paths. It provides both *brute-force* and *heuristic*
search algorithms with consistent heuristics.

It also comes with a number of domain-dependent solvers that test the
performance of these algorithms

# Dependencies #

All algorithms are implemented in C++23.

Tests have been created using the [Google Test
Framework](https://github.com/google/googletest) which is necessary for both
compiling and running the tests, see [Tests](#tests).

It is also possible to directly download some datafiles ---see [Additional CMake
Options](#additional-cmake-options). An installation of `7zip`, `gunzip`, or
`gzip` is needed for automatic downloads to work.

In addition, a number of scripts are provided for various purposes. They all
have been implemented in Python 3.8.

# Install #

To download the code type the following:

``` sh
git clone https://github.com/clinaresl/ksearch.git
```

To compile the source code, create first the `Makefile` with:

``` sh
cmake . -DCMAKE_BUILD_TYPE=Release
make
```

from the `ksearch/` directory created after the clone. It will build the
library, a solver for every domain defined under `domains/`, along with a random
generator for it , and all the unit tests.

It is also possible to compile separately different parts of the whole bundle.
To compile only the library do:

``` sh
make ksearch
```

which generates a static library.

To generate an executable to solve instances in a specific domain use `make 
domain` where *domain* can be any of the following: `grid`, `map`, `npancake`,
`npuzzle` or `roadmap`.

Finally, all domains (but *grid*) come with their own random generator of
instances. To generate them execute `make gen-domain` where *domain* can be any
of the following: `map`, `npancake`, `npuzzle` or `roadmap`.

# Tests #

This library comes with a number of unit tests that have been implemented with
[Google Test](https://github.com/google/googletest). If Google Test is available
in your computer, then you can compile and run the tests with:

``` sh
make gtest
cd tests
./gtest
```

It is important to run the tests from `ksearch/tests` because they make use
of a number of files that have to be found under `testdata/` which is loocated
indeed under `ksearch/tests`. 

# Search algorithms #

The search algorithms provided in this library solve the *k* shortest-path
problem, both in the *simple* and *non-simple* variants. It provides brute-force
and heuristic search algorithms using consistent heuristics:

* Brute-force search algorithms:

  + *k* *non-simple* shortest-path problem: the following algorithms compute *k*
    shortest paths between two designated vertices. Solution paths might be *non-simple*

    - `mDijkstra`: It consists of expanding every node up to *k* times
    - `K0`: Uninformed variant of K*
    - `BELA0`: uninformed variant of BELA*

  + *k* *simple* shortest-path problem: the following algorithms compute *k*
    shortest paths between two designated vertices. Solution are guaranteed to
    be *simple*

    - `bBELA0`: uninformed variant of bBELA*
    - `sBELA0`: uninformed variant of sBELA*
  

* (consistent) Heuristic search algorithms:

  + *k* *non-simple* shortest-path problem: the following algorithms compute *k*
    shortest paths between two designated vertices. Solution paths might be *non-simple*

    - `mA*`: expands every node up to *k* times.
    - `K*`: adapation of Eppstein algorithm to solve the *k* shortest-path problem with (consistent) heuristics
    - `BELA*`: extends the definition of *sidetrack edges* with that of *centroids*

  + *k* *simple* shortest-path problem: the following algorithms compute *k*
    shortest paths between two designated vertices. Solution are guaranteed to
    be *simple*

    - `bBELA*`: computes as many paths as necessary using BELA* and discard all
      *non-simple* paths until *k* simple paths are produced.
    - `sBELA*`: uses the notion of *centroids* to compute directly *k* shortest
      simple paths.
  

# Domain-dependent solvers #

This package comes with a number of domain-dependent solvers under `domains/`:

* *2D Pathfinding*
* *Grid*
* *N-Pancake*
* *N-Puzzle*
* *Roadmap* 

Every domain-dependent solver provides all the search algorithms described
above. For further details, check out the documentation under `domains/`

# Additional CMake Options

Various CMake options are available that automatically download datafiles for
some domains.

* `-DDATA_ALL`: downloads all data files for the roadmap domain to `domains/roadmap/benchmark`.
* `-DDATA_*`: downloads the data files for specific maps in the roadmap domain to `domains/roadmap/benchmark`.
  + `-DDATA_USA`: Full USA
  + `-DDATA_CTR`: Central USA
  + `-DDATA_W`: Western USA
  + `-DDATA_E`: Eastern USA
  + `-DDATA_LKS`: Great Lakes
  + `-DDATA_CAL`: California and Nevada
  + `-DDATA_NE`: Northeast USA
  + `-DDATA_NW`: Northwest USA
  + `-DDATA_FLA`: Florida
  + `-DDATA_COL`: Colorado
  + `-DDATA_BAY`: San Fransisco Bay Area
  + `-DDATA_NY`: New York City

**NOTE:** If your installation of `7zip`, `gunzip` or `gzip` is not in a default
location, a path can be provided through the following CMake options:

* `-D7ZIP_LOCATION`: Path to 7zip executable
* `-DGUNZIP_LOCATION`: Path to Gunzip executable
* `-DGZIP_LOCATION`: Path to gzip executable

# Publications #

* Linares López, Carlos; Herman, Ian, 2024. Evolving A* to Efficiently Solve the
  *k* Shortest-Path Problem. Proceedings of the Twenty-Seventh European
  Conference on Artificial Intelligence (ECAI 2024), pages 4352-4359. Santiago
  de Compostela (Spain).

  An extended version of the same paper with an example of BELA0 and the results
  of all the empirical evaluation can be found at:

  + Linares López, Carlos; Herman, Ian, 2024. Evolving A* to Efficiently Solve
    the k Shortest-Path Problem (Extended Version). arXiv 2024. eprint
    [2408.08227](https://arxiv.org/abs/2408.08227).

    In addition, all the data used in the experimentation is available in Zenodo:

  + Linares López, C., & Herman, I. (2024). Experimental evaluation of
    BELA0/BELA* (ECAI 2024) (Data set). Evolving A* to Efficiently Solve the k
    Shortest-Path Problem (ECAI), Santiago de Compostela (Spain). Zenodo.
    https://doi.org/10.5281/zenodo.13293103

* Linares López, Carlos; Herman, Ian, 2026. Finding Simple Shortest-Paths via
  Centroids. Proceedings of the 35th International Joint Conference on Artificial
  Intelligence (IJCAI 2026). Bremen (Germany).
  
  All the data used in the experimentation is available in Zenodo:
  
    + Linares López, C. (2026). Experimental evaluation of sBELA0/sBELA* (IJCAI
      2026) (Version 1.0.0) [Dataset]. Zenodo. International Joint Conference on
      Artificial Intelligence (IJCAI), Bremen, Germany.
      https://doi.org/10.5281/zenodo.20184563

# License #

MIT License

Copyright (c) 2016, 2025, 2026 Carlos Linares López, Ian Herman

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


# Authors #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>

Ian Herman <iankherman@gmail.com>  
Computer Science and Engineering Department https://www.inf.uc3m.es/en  
Universidad Carlos III de Madrid https://www.uc3m.es/home

