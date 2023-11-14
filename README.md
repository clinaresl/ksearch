# Introduction #

This library implements a number of search algorithms to compute *K*
(non-simple) optimal paths. It provides both *brute-force* and *heuristic*
search algorithms. It also comes with a number of domain-dependent solvers that
test the performance of these algorithms


# Install #

To download the code type the following:

``` sh
    $ git clone https://github.com/clinaresl/libksearch.git
```

To compile the source code, create first the `Makefile` with:

``` sh
    $ cmake . -DCMAKE_BUILD_TYPE=Release
    $ make
```

from the `libksearch/` directory created after the clone. It will build the
library, a solver for every domain defined under `domains/`, and the unit tests.

# Tests #

This library comes with a number of unit tests that have been implemented with
[Google Test](https://github.com/google/googletest). If Google Test is available
in your computer, then you can compile and run the tests with:

``` sh
    $ make gtest
    $ tests/gtest
```

from the `libksearch/` directory created after the clone.

# Search algorithms #

The search algorithms provided in this library solve the *k* non-simple
shortest-paths problem. It provides both brute-force and heuristic search
algorithms:

* Brute-force search algorithms:

+ `mDijkstra`: It consists of expanding every node up to *k* times
+ `belA0`: it is the uninformed variant of BELA*
+ `K0`: Uninformed variant of K*

* Heuristic search algorithms:

+ `belA*`: Informed variant of BELA*
+ `K*`: Informed variant of K*

# Domain-dependent solvers #

This package comes with a number of domain-dependent solvers under `domains/`:

* *Grid*
* *N-Pancake*
* *Roadmap* 

For further details, check out the documentation under `domains/`

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


# Authors #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>

Ian Herman <iankherman@gmail.com>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>  
