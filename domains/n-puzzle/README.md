# N-puzzle #

Ths domain implments the *sliding-tile puzzle* over square matrices, i.e., it
consists of a permutation of the symbols *[0, N^2)*, where *0* denotes the
*blank tile* over a square of length *N*. The only allowed moves consist of
moving the blank tile either horizontally or vertically without getting out of
bounds. The goal consists of reaching the identity permutation from a designated
start permutation.

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The
  heuristic implemented is the Manhattan heuristic.
  
* `heavy-cost`: every operator has an associated cost equal to the content of
  the tile which is swapped with the blank tile. The heuristic implemented is a
  weighted version of the Manhattan heuristic.
  
The best known heuristic for solving problems in the *N-Puzzle* are additive
pattern database (ADD PDBs). However, they yield inconsistent estimates and
their integration with belA* is left for future work.

The test file consist of a line per optimization task with *N^2* digits in each
line preceded by the problem id ---thus following Korf's format. The goal state
is always assumed to be the identity permutation. An example follows:

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
```

which are, indeed, the 10 easiest instances from Korf's test suite.

# Usage #

This solver accepts the same flags discussed in the entry above under `domains/`
and no other specific directives are provided.

An example for the computation of test distinct paths for every case specified
above under the heavy-cost variant is shown next:

``` sh
    $ ./npuzzle --solver "belA*" --file test --variant unit --k "10" --no-doctor --summary --csv test.csv
    
       solver       : belA* 1.6.0-1-gf0e36d8
       file         : test (50 instances)
       variant      : unit
       size         : 4
       K            : [10, 10, 1] 

       ⏺ belA* ( k=10 ): 
       ⏵ 00;10;0 1 9 7 11 13 5 3 14 12 4 2 8 6 10 15 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;28;240337;8;267;0.404067;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 01;10;14 1 9 6 4 8 12 5 7 2 3 0 10 11 13 15 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;35;163769;9;313;0.220804;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 02;10;4 5 7 2 9 14 12 13 0 3 6 11 8 1 15 10 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;30;210855;10;375;0.316843;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 03;10;13 8 14 3 9 1 0 7 15 5 4 10 12 2 6 11 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;29;199299;9;270;0.328133;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 04;10;9 14 5 7 8 15 1 2 10 4 13 6 12 0 11 3 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;32;745067;6;822;1.36985;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 05;10;7 11 8 3 14 0 6 15 1 4 13 9 5 12 2 10 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;36;677552;5;822;0.951289;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 06;10;5 7 11 8 0 14 9 13 10 12 3 15 6 1 4 2 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;45;323300;7;822;0.418809;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 07;10;6 10 1 14 15 8 3 5 13 0 2 7 4 9 11 12 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;35;742025;4;930;1.20249;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 08;10;13 9 14 6 12 8 1 2 3 4 0 7 5 10 11 15 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;34;1201536;8;1427;1.92061;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 09;10;3 14 9 11 5 4 8 2 13 12 6 7 10 1 15 0 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;32;1070043;7;1492;1.64729;BELA*;? Unchecked;1.6.0-1-gf0e36d8
         ...
       ⏵ 47;10;14 4 0 10 6 5 1 3 9 2 13 15 12 7 8 11 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;30;5686547;4;14376;9.94194;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 48;10;1 6 12 14 3 2 15 8 4 5 13 9 0 7 11 10 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;39;19510282;8;22407;37.2476;BELA*;? Unchecked;1.6.0-1-gf0e36d8
       ⏵ 49;10;8 13 10 9 11 3 15 6 0 1 2 14 12 5 4 7 ;0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ;36;11948711;9;22407;21.9823;BELA*;? Unchecked;1.6.0-1-gf0e36d8

         🕒 CPU time: 615.123 seconds

       📊 Error summary: <doctor disabled>
       🛈 CSV file generated!
       🕒 CPU time: 615.129 seconds
```


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

