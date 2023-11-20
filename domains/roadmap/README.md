# Roadmap #

The roadmap domain is taken from the [9th DIMACS Implementation Challenge:
Shortest Paths](http://www.diag.uniroma1.it/~challenge9/download.shtml#benchmark)

The available benchmarks are stored in the directory `benchmarks`. The graphs
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

The particular instances to solve are given in the files `.test` which consist
of precisely two lines. The i-th test case uses the i-th vertex from the first
line as the starting vertex and the i-th vertex from the second line as the
goal. For example, the test file:

``` text
109245 150503
149048 71654
```

defines two instances. The first one consists of getting to node 149048 from
vertex 109245; and second one gets from 150503 to 71654.

# Usage #

In addition to all directives discussed for all domains, this solver honours an
additional flag:

* `--graph`: filename with the `.gr` graph definition file

which is a mandatory argument.

An example for the computation of one thousand distinct paths for every case
specified above is shown next:

``` sh
    $ ./roadmap --graph benchmark/bayareadist.gr --solver "belA0" --file benchmark/bayareadist.test --variant dimacs --k 1000
    
      graph        : benchmark/bayareadist.gr (794830 edges processed)
      solver       : belA0
      file         : benchmark/bayareadist.test (2 instances)
      variant      : dimacs
      K            : [1000, 1000, 1] 

      ⏺ belA0 ( k=1000 ): 
      ⏵ 0;1000;109245;149048;0;9797;0.0410145;BELA*;✔ No error
      ⏵ 1;1000;150503;71654;0;114029;0.284162;BELA*;✔ No error

     📊 Error summary: 
            Number of errors: 0
     ⚠ No CSV output generated!
     🕒 CPU time: 0.485845 seconds
```

Note that solutions are automatically checked for correctness, because
`--no-doctor` has not been used.

# License #

MIT License

Copyright (c) 2016, 2023, Carlos Linares López, Ian Herman

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
University of Hawaii at Hilo <https://hilo.hawaii.edu/>  
