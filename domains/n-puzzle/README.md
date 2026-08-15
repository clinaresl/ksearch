# N-puzzle #

Ths domain implements the *sliding-tile puzzle* over square matrices, i.e., it
consists of a permutation of the symbols *[0, N^2)*, where *0* denotes the
*blank tile* over a square of length *N*. The only allowed moves consist of
moving the blank tile either horizontally or vertically without getting out of
bounds. The goal consists of reaching the identity permutation from a designated
start permutation.

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The
  heuristic implemented is the Manhattan heuristic. 
  
  The best known heuristic for solving problems in the *N-Puzzle* are additive
  pattern database (ADD PDBs). However, they yield inconsistent estimates and
  their integration with BELA* is left for future work.
  
* `heavy-cost`: every operator has an associated cost equal to the content of
  the tile which is swapped with the blank tile. The heuristic implemented is a
  weighted version of the Manhattan heuristic.

The test file consist of a line per optimization task with *N^2* digits in each
line preceded by the problem id ---thus following Korf's format. The goal state
is always assumed to be the identity permutation. The first 10 lines of the file
`benchmark/16puzzle.test` are shown next:

``` text
00 0 1 9 7 11 13 5 3 14 12 4 2 8 6 10 15
01 14 1 9 6 4 8 12 5 7 2 3 0 10 11 13 15
02 4 5 7 2 9 14 12 13 0 3 6 11 8 1 15 10
03 13 8 14 3 9 1 0 7 15 5 4 10 12 2 6 11
04 9 14 5 7 8 15 1 2 10 4 13 6 12 0 11 3
05 7 11 8 3 14 0 6 15 1 4 13 9 5 12 2 10
06 5 7 11 8 0 14 9 13 10 12 3 15 6 1 4 2
07 6 10 1 14 15 8 3 5 13 0 2 7 4 9 11 12
08 13 9 14 6 12 8 1 2 3 4 0 7 5 10 11 15
09 3 14 9 11 5 4 8 2 13 12 6 7 10 1 15 0
...
```

which are, indeed, the 10 easiest instances from Korf's test suite.

# Usage #

This solver accepts the same flags discussed in the entry above under `domains/`
and no other specific directives are provided.

An example for the computation of ten distinct *simple* paths for the 40 easiest
instances from Korf's test suite under the unit variant is shown next:

``` sh
./npuzzle --solver "sBELA*" --file benchmark/16puzzle.test --variant unit --k "10" --no-doctor --summary --csv test.csv--summary --csv test.csv
    
 solver       : sBELA* ijcai-g133903f
 file         : benchmark/16puzzle.test (40 instances)
 variant      : unit
 size         : 4
 K            : [10, 10, 1]

 ⏺ sBELA* ( k=10 ):
         id       k                       start                        goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor      version
 ⏵       00      10   0 1 9 7 11 13 5 3 14 1...   0 1 2 3 4 5 6 7 8 9 10...       28        240337          56       0    283.000     0.372      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       01      10   14 1 9 6 4 8 12 5 7 2 ...   0 1 2 3 4 5 6 7 8 9 10...       35        299467         105       0    376.000     0.391      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       02      10   4 5 7 2 9 14 12 13 0 3...   0 1 2 3 4 5 6 7 8 9 10...       30        846135         107       0   1004.000     1.441      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       03      10   13 8 14 3 9 1 0 7 15 5...   0 1 2 3 4 5 6 7 8 9 10...       29        476299          50       0   1013.000     0.680      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       04      10   9 14 5 7 8 15 1 2 10 4...   0 1 2 3 4 5 6 7 8 9 10...       32       1368310          89       0   1700.000     2.022      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       05      10   7 11 8 3 14 0 6 15 1 4...   0 1 2 3 4 5 6 7 8 9 10...       36        677552         101       0   1700.000     0.866      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       06      10   5 7 11 8 0 14 9 13 10 ...   0 1 2 3 4 5 6 7 8 9 10...       45        556544          55       0   1700.000     0.704      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       07      10   6 10 1 14 15 8 3 5 13 ...   0 1 2 3 4 5 6 7 8 9 10...       35        742025          34       0   1700.000     1.119      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       08      10   13 9 14 6 12 8 1 2 3 4...   0 1 2 3 4 5 6 7 8 9 10...       34       4450307         113       0   5050.000     7.764      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       09      10   3 14 9 11 5 4 8 2 13 1...   0 1 2 3 4 5 6 7 8 9 10...       32       1070043         116       0   5050.000     1.536      sBELA*   ? Unchecked  ijcai-g133903f

         id       k                       start                        goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor      version
 ⏵       10      10   14 13 4 11 15 8 6 9 0 ...   0 1 2 3 4 5 6 7 8 9 10...       46       9676891         148       0  12073.000    16.956      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       11      10   8 11 4 6 7 3 10 9 2 12...   0 1 2 3 4 5 6 7 8 9 10...       39        625133          88       0  12073.000     0.803      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       12      10   12 15 2 6 1 14 4 8 5 3...   0 1 2 3 4 5 6 7 8 9 10...       35        907425         143       0  12073.000     1.304      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       13      10   6 0 5 10 11 12 9 2 1 7...   0 1 2 3 4 5 6 7 8 9 10...       35        393124           6       0  12073.000     0.562      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       14      10   12 8 15 13 1 0 5 4 6 3...   0 1 2 3 4 5 6 7 8 9 10...       38       3259127         139       0  12073.000     5.073      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       15      10   4 7 13 10 1 2 9 6 12 8...   0 1 2 3 4 5 6 7 8 9 10...       32        848776          82       0  12073.000     1.256      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       16      10   6 14 10 5 15 8 7 1 3 4...   0 1 2 3 4 5 6 7 8 9 10...       37        843058         105       0  12073.000     1.221      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       17      10   13 14 6 12 4 5 1 0 9 3...   0 1 2 3 4 5 6 7 8 9 10...       36       2841720           3       0  12073.000     4.034      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       18      10   3 14 9 7 12 15 0 4 1 8...   0 1 2 3 4 5 6 7 8 9 10...       39       2420604          85       0  12073.000     3.493      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       19      10   6 13 3 2 11 9 5 10 1 7...   0 1 2 3 4 5 6 7 8 9 10...       31       1232862           8       0  12073.000     1.735      sBELA*   ? Unchecked  ijcai-g133903f

         id       k                       start                        goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor      version
 ⏵       20      10   4 3 6 13 7 15 9 0 10 5...   0 1 2 3 4 5 6 7 8 9 10...       34       2947832          59       0  12073.000     5.007      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       21      10   5 4 7 1 11 12 14 15 10...   0 1 2 3 4 5 6 7 8 9 10...       36       3510469          35       0  12073.000     5.652      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       22      10   5 11 6 9 4 13 12 0 8 2...   0 1 2 3 4 5 6 7 8 9 10...       36       2142587         104       0  12073.000     3.164      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       23      10   5 7 0 11 12 1 9 10 15 ...   0 1 2 3 4 5 6 7 8 9 10...       30       1854931          71       0  12073.000     2.818      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       24      10   13 0 9 12 11 6 3 5 15 ...   0 1 2 3 4 5 6 7 8 9 10...       39       7678233         135       0  13878.000    12.533      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       25      10   3 14 13 6 4 15 8 9 5 1...   0 1 2 3 4 5 6 7 8 9 10...       41       3102049          45       0  13878.000     5.015      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       26      10   4 7 14 13 10 3 9 12 11...   0 1 2 3 4 5 6 7 8 9 10...       42       3023383           6       0  13878.000     4.965      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       27      10   3 6 5 2 10 0 15 14 1 4...   0 1 2 3 4 5 6 7 8 9 10...       36       2641506          65       0  13878.000     4.090      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       28      10   1 7 15 14 2 6 4 9 12 1...   0 1 2 3 4 5 6 7 8 9 10...       35       5119908          61       0  13878.000     7.861      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       29      10   5 0 15 8 4 6 1 14 10 1...   0 1 2 3 4 5 6 7 8 9 10...       37       1638986           4       0  13878.000     2.416      sBELA*   ? Unchecked  ijcai-g133903f

         id       k                       start                        goal       h0   #expansions  #centroids  #paths  mem usage   runtime      solver      doctor      version
 ⏵       30      10   13 5 4 10 9 12 8 14 2 ...   0 1 2 3 4 5 6 7 8 9 10...       43       4916507           7       0  13878.000     7.179      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       31      10   10 9 3 11 0 13 2 14 5 ...   0 1 2 3 4 5 6 7 8 9 10...       33       5329965          62       0  13878.000     8.344      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       32      10   6 12 11 3 13 7 9 15 2 ...   0 1 2 3 4 5 6 7 8 9 10...       36       8167236         203       0  13878.000    12.901      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       33      10   1 3 2 5 10 9 15 6 8 14...   0 1 2 3 4 5 6 7 8 9 10...       24       2774920          59       0  13878.000     4.009      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       34      10   0 13 2 4 12 14 6 9 15 ...   0 1 2 3 4 5 6 7 8 9 10...       34       9319514          26       0  13878.000    14.641      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       35      10   7 8 3 2 10 12 4 6 11 1...   0 1 2 3 4 5 6 7 8 9 10...       31       3300493          70       0  13878.000     5.333      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       36      10   9 0 4 10 1 14 15 3 12 ...   0 1 2 3 4 5 6 7 8 9 10...       35       3969591           2       0  13878.000     6.091      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       37      10   6 11 7 8 13 2 5 4 1 10...   0 1 2 3 4 5 6 7 8 9 10...       36       3615611           5       0  13878.000     5.339      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       38      10   8 4 6 1 14 12 2 15 13 ...   0 1 2 3 4 5 6 7 8 9 10...       35       2234064           3       0  13878.000     3.143      sBELA*   ? Unchecked  ijcai-g133903f
 ⏵       39      10   4 6 12 0 14 2 9 13 11 ...   0 1 2 3 4 5 6 7 8 9 10...       43       6172800           1       0  13878.000     9.958      sBELA*   ? Unchecked  ijcai-g133903f

   🕒 (k=10) CPU time: 296.952 seconds

   🕒 (sBELA*) CPU time: 296.952 seconds

 📊 Error summary: <doctor disabled>
        Cross-validation: Ok
 🛈 CSV file generated!
 🕒 CPU time: 296.953 seconds
 ```

Note that in this case a csv file is explicitly requested which contains the
same information shown on the console. Also, the start and goal states are
automatically pruned to avoid creating too long lines.

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

