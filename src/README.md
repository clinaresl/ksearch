# Source code #

This directory contains the source code with the implementation of all search
algorithms and necessary data structures.

The directory `structs/` contains the definition of every data structure used by
every search algorithm in `algorithm/`.

All implementations are given in C++23.

# Data structures #

Typical data structures contain the definition of nodes (of different types,
since different algorithms require different sorts of information), closed and
open lists and other data structures necessary to store the set of solution
paths.

All the data structures implemented in the directory `structs/` come with a
number of unit tests that can be found in `tests/structs/` under the root
directory of this repository.

# Search algorithms #

Every search algorithm inherits from `bsolver` (given in `KHSbsolver.h`) the
interface to implement which is then given in all the other files. 

Every file contains the implementation of the brute-force and heuristic variant.

All the algorithms implemented in the directory `algorithm/` come with a number
of unit tests that can be found in `tests/algorithm` under the root directory of
this repository.

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


# Author #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>

Ian Herman <iankherman@gmail.com>  
Computer Science and Engineering Department https://www.inf.uc3m.es/en  
Universidad Carlos III de Madrid https://www.uc3m.es/home
