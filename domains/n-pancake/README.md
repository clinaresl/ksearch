# N-pancake #

Pancake Flipping is the problem of sorting a stack of pancakes of different
sizes (that is, a permutation of integers in the range *[1, n]*), where the only
allowed operation is to insert a spatula anywhere in the stack and to flip the
pancakes above it (that is, to perform a prefix reversal). 

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The
  heuristic implemented is the GAP heuristic.
  
* `heavy-cost`: every operator has an associated cost equal to the size of the
  disc that becomes first when applying the operator. Consequently, it engenders
  a directed weighted graph because the cost of an operator and its inverse are
  not equal. The heuristic implemented is a weighted GAP heuristic.

The test file consist of a line per optimization task with *n+1* digits in each
line. The first digit is the task id; the next *n* digits (that must be integers
in the range *[1, n]*) must be a valid permutation representing the start state.
The goal state is always assumed to be the identity permutation. The first 10
lines of `benchmark/test-10.test` are shown next:

``` text
0 3 8 2 7 9 1 5 10 6 4 
1 4 1 6 5 10 2 8 3 9 7 
2 9 8 4 1 10 2 6 3 7 5 
3 9 2 7 1 4 8 10 5 6 3 
4 4 1 5 2 8 6 10 9 3 7 
5 2 1 8 6 4 9 5 3 10 7 
6 9 1 6 4 3 8 10 2 5 7 
7 3 5 2 10 8 9 4 6 1 7 
8 4 7 2 9 5 3 6 10 1 8 
9 1 9 6 10 4 2 5 7 8 3 
...
```

# Usage #

This solver accepts the same flags discussed in the entry above under `domains/`
and no other specific directives are provided.

An example for the computation of ten distinct paths for every case specified
above under the heavy-cost variant is shown next:

``` sh
./npancake --solver "BELA*" --file benchmark/test-10.test --variant heavy-cost --k 10

 ✔ solver       : BELA* ijcai-g133903f
 ✔ file         : benchmark/test-10.test (100 instances)
 ✔ variant      : heavy-cost
 ✔ size         : 10
 ✔ K            : [10, 10, 1]

 ⏺ BELA* ( k=10 ):
         id       k                  start                   goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver        doctor    version
 ⏵        0      10   3 8 2 7 9 1 5 10 6 4   1 2 3 4 5 6 7 8 9 10       35        231780           8       0    637.000     0.978       BELA*    ✔ No error  ijcai-g133903f
 ⏵        1      10   4 1 6 5 10 2 8 3 9 7   1 2 3 4 5 6 7 8 9 10       31        134607           6       0    469.000     0.439       BELA*    ✔ No error  ijcai-g133903f
 ⏵        2      10   9 8 4 1 10 2 6 3 7 5   1 2 3 4 5 6 7 8 9 10       26        101459           8       0    469.000     0.343       BELA*    ✔ No error  ijcai-g133903f
 ⏵        3      10   9 2 7 1 4 8 10 5 6 3   1 2 3 4 5 6 7 8 9 10       29        176450           8       0    443.000     0.690       BELA*    ✔ No error  ijcai-g133903f
 ⏵        4      10   4 1 5 2 8 6 10 9 3 7   1 2 3 4 5 6 7 8 9 10       31         30618          10       0    443.000     0.085       BELA*    ✔ No error  ijcai-g133903f
 ⏵        5      10   2 1 8 6 4 9 5 3 10 7   1 2 3 4 5 6 7 8 9 10       40        148580          10       0    437.000     0.471       BELA*    ✔ No error  ijcai-g133903f
 ⏵        6      10   9 1 6 4 3 8 10 2 5 7   1 2 3 4 5 6 7 8 9 10       33        143996          10       0    462.000     0.462       BELA*    ✔ No error  ijcai-g133903f
 ⏵        7      10   3 5 2 10 8 9 4 6 1 7   1 2 3 4 5 6 7 8 9 10       32         61662           9       0    462.000     0.187       BELA*    ✔ No error  ijcai-g133903f
 ⏵        8      10   4 7 2 9 5 3 6 10 1 8   1 2 3 4 5 6 7 8 9 10       35        245369           8       0    622.000     0.869       BELA*    ✔ No error  ijcai-g133903f
 ⏵        9      10   1 9 6 10 4 2 5 7 8 3   1 2 3 4 5 6 7 8 9 10       32         73077          10       0    622.000     0.230       BELA*    ✔ No error  ijcai-g133903f

         id       k                  start                   goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver        doctor    version
 ⏵       10      10   6 4 3 7 10 8 2 5 1 9   1 2 3 4 5 6 7 8 9 10       37         52120          10       0    622.000     0.145       BELA*    ✔ No error  ijcai-g133903f
 ⏵       11      10   1 7 9 4 2 5 6 3 10 8   1 2 3 4 5 6 7 8 9 10       38         69085          10       0    622.000     0.200       BELA*    ✔ No error  ijcai-g133903f
...
 ⏵       98      10   10 4 7 1 8 6 2 9 3 5   1 2 3 4 5 6 7 8 9 10       31        355465           7       0    901.000     1.354       BELA*    ✔ No error  ijcai-g133903f
 ⏵       99      10   6 9 5 8 2 7 3 1 4 10   1 2 3 4 5 6 7 8 9 10       29         32190           7       0    901.000     0.093       BELA*    ✔ No error  ijcai-g133903f
 
   🕒 (k=10) CPU time: 75.6722 seconds

   🕒 (BELA*) CPU time: 75.6723 seconds

 📊 Error summary:
	Number of errors: 0
        Cross-validation: Ok
 ⚠ No CSV output generated!
 🕒 CPU time: 75.6738 seconds
```

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

