# Roadmap #

The roadmap domain is taken from the [9th DIMACS Implementation Challenge:
Shortest
Paths](http://www.diag.uniroma1.it/~challenge9/download.shtml#benchmark). They
can be downloaded automatically, see [Additional CMake
Options](../../README.md#additional-cmake-options).

The available benchmarks are stored in the directory `benchmark/`. The graphs
are suffixed with `.gr` where, every line consists of an edge identified by its
two vertices represented with integer indices, and the third value is the edge
cost. Data lines are preceded with `a` whereas comments are preceded with the
hash symbol `#`. See the available
[help](http://www.diag.uniroma1.it/~challenge9/format.shtml) for more
information on the formats.

This domain provides two variants:

* `unit`: all operators cost the same, i.e., the edge cost is ignored and set to
  one for all edges.
* `dimacs`: in this variant the edge cost is preserved as retrieved from the
  `.gr`file.

No heuristics are used in any case.

The particular instances to solve are given in the files `.test` which specify a
test case per line. The first value is the task id. Next, the vertex id of both
the start and goal states are given. For example, the first 10 cases of the file
`benchmark/USA-road-d.COL.test` are shown next:

``` text
00 294756 116704
01 325364 11913
02 285177 334042
03 236480 66871
04 249937 358183
05 278932 202424
06 430744 255885
07 167653 110820
08 389224 209129
09 361295 369960
...
```

To ease experimentation, the first time a graph is loaded into main memory, a
`.bin` file with the same information is automatically created. Subsequent
experiments over the same graph use the `.bin` file instead which are much
faster to load. These can be removed anytime, though.


# Usage #

In addition to all directives discussed for all domains, this solver honours an
additional flag:

* `--graph`: filename with the `.gr` graph definition file

which is a mandatory argument.

An example for the computation of one thousand distinct *simple* paths for every
case specified above is shown next:

``` sh
./roadmap --graph benchmark/USA-road-d.COL.gr --solver "sBELA0" --file benchmark/USA-road-d.COL.test --k 1000 --variant dimacs
    
 graph        : benchmark/USA-road-d.COL.gr (1042400 edges processed)
 solver       : sBELA0 ijcai-2026-base-17-g133903f-dirty
 file         : benchmark/USA-road-d.COL.test (100 instances)
 variant      : dimacs
 K            : [1000, 1000, 1]

 ⏺ sBELA0 ( k=1000 ):
         id       k        start         goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor     version
 ⏵        0    1000       294756       116704        0        306479        1465       0    538.000     0.756      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        1    1000       325364        11913        0        307977         865       0    564.000     0.649      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        2    1000       285177       334042        0        242846        1022       0    564.000     0.448      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        3    1000       236480        66871        0        292944        1047       0    573.000     0.577      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        4    1000       249937       358183        0        417071         781       0    725.000     0.771      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        5    1000       278932       202424        0        416303        1125       0    745.000     0.803      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        6    1000       430744       255885        0        213671        1045       0    745.000     0.492      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        7    1000       167653       110820        0        125211         838       0    745.000     0.299      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        8    1000       389224       209129        0        281015         187       0    745.000     0.425      sBELA0   ✔ No error  ijcai-g133903f
 ⏵        9    1000       361295       369960        0         18790        1080       0    745.000     0.095      sBELA0   ✔ No error  ijcai-g133903f

         id       k        start         goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor     version
 ⏵       10    1000        63073       122016        0        146465         742       0    745.000     0.274      sBELA0   ✔ No error  ijcai-g133903f
 ⏵       11    1000       377280        89101        0        249458         777       0    745.000     0.580      sBELA0   ✔ No error  ijcai-g133903f
 ...
 ⏵       98    1000        72512       386851        0        356140         349       0   1943.000     0.711      sBELA0   ✔ No error  ijcai-g133903f
 ⏵       99    1000       148594       414859        0        216982        3400       0   1943.000     0.415      sBELA0   ✔ No error  ijcai-g133903f

   🕒 (k=1000) CPU time: 81.1581 seconds

   🕒 (sBELA0) CPU time: 81.1581 seconds

 📊 Error summary:
	Number of errors: 0
        Cross-validation: Ok
 ⚠ No CSV output generated!
 🕒 Total CPU time: 81.3538 seconds
```

Note that solutions are automatically checked for correctness, because
`--no-doctor` has not been used.

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
