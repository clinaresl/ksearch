# Maps #

The maps domain is taken from the *movingai* repository using [2d
benchmarks](https://movingai.com/benchmarks/grids.html). 

The available benchmarks are stored in the directory `benchmark/`. The
information of each map is given in files with suffix `.map`. Information on the
file format can be found [here](https://movingai.com/benchmarks/formats.html).

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The only
  allowed operators are to move either horizontally or vertically. The heuristic
  implemented is the Manhattan distance to the goal.
  
* `octile`: in this variant it is possible also to move diagonally in any
  direction. Horizontal and vertical moves have a cost equal to 10 whereas
  diagonal moves have a cost equal to 14. The heuristic implemented is the
  Octile distance to the goal.

The particular instances to solve are given in the files `.test` which specify a
test case per line. The first value is the task id. Next, the *x* and *y*
coordinates of the start vertex are given first, and then the *x* and *y*
coordinates of the goal vertex. For example, the first 10 cases of the file
`benchmark/random512-10-0.test` are shown next:

``` text
0 497 10 27 502
1 507 0 31 504
2 7 14 507 496
3 31 2 476 497
4 489 511 35 18
5 9 508 461 1
6 499 1 43 472
7 480 0 27 466
8 19 510 487 57
9 36 490 497 15
...
```


# Usage #

An example for the computation of one thousand distinct *simple* paths for every
case specified above is shown next:

``` sh
./map --map benchmark/random512-10-0.map  --solver "bBELA*<1>" --file benchmark/random512-10-0.test --variant octile --k 1000
    
 map          : benchmark/random512-10-0.map
    width     : 512
    height    : 512
 solver       : bBELA*<1> ijcai-2026-base-17-g133903f-dirty
 file         : benchmark/random512-10-0.test (100 instances)
 variant      : octile
 K            : [1000, 1000, 1]

 ⚠ Warning: bBELA*<1> does not guarantee to produce correct solutions!
   In case a possibility of generating incorrect solutions is detected, execution will be halted immediately!

 ⏺ bBELA*<1> ( k=1000 ):
         id       k     start      goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver    doctor      version
 ⏵        0    1000   497  10    27 502     6800          9398           1       0     43.000     0.061   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        1    1000   507   0    31 504     6944         10689           1       0     63.000     0.055   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        2    1000     7  14   507 496     6928         10816           1       0     72.000     0.052   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        3    1000    31   2   476 497     6730         10717           1       0     74.000     0.050   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        4    1000   489 511    35  18     6746          7835           1       0     87.000     0.047   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        5    1000     9 508   461   1     6878         12184           1       0    109.000     0.052   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        6    1000   499   1    43 472     6534          8794           1       0    105.000     0.045   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        7    1000   480   0    27 466     6472         10407           1       0    116.000     0.054   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        8    1000    19 510   487  57     6492          7391           1       0    100.000     0.046   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵        9    1000    36 490   497  15     6594          6551           1       0    122.000     0.042   bBELA*<1>  ✔ No error  ijcai-g133903f

         id       k     start      goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor    version
 ⏵       10    1000   507  20    43 483     6492          8314           1       0    119.000     0.046   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵       11    1000    22 461   507  15     6634          7162           1       0    141.000     0.042   bBELA*<1>  ✔ No error  ijcai-g133903f
 ...
 ⏵       98    1000    26  42   497 501     6546          6841           1       0    809.000     0.047   bBELA*<1>  ✔ No error  ijcai-g133903f
 ⏵       99    1000    50 497   510  22     6590          6178           1       0    816.000     0.042   bBELA*<1>  ✔ No error  ijcai-g133903f

   🕒 (k=1000) CPU time: 17.0328 seconds

   🕒 (bBELA*<1>) CPU time: 17.0328 seconds

 📊 Error summary:
	Number of errors: 0
        Cross-validation: Ok
 ⚠ No CSV output generated!
 🕒 CPU time: 17.189 seconds
 ```

Note that solutions are automatically checked for correctness, because
`--no-doctor` has not been used. In this case, bBELA* has been used with *n=1*.

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
