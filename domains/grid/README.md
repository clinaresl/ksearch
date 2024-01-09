# Grid #

The Grid domain consists of a square grid with no obstacles of size *n*. The
start and goal states can be located anywhere in the grid where the *x* and *y*
coordinates range from 0 until *n-1*.

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The only
  allowed operators are to move either horizontally or vertically. The heuristic
  implemented is the Manhattan distance to the goal.
  
* `octile`: in this variant it is possible also to move diagonally in any
  direction. Horizontal and vertical moves have a cost equal to 10 whereas
  diagonal moves have a cost equal to 14. The heuristic implemented is the
  Octile distance to the goal.

The test file consist of five digits per line: the first one is the task id; the
second and third digits are the *x* and *y* coordinates of the start state; the
last two digits are the *x* and *y* coordinates of the goal state. An example
follows:

``` text
00 0 0 9 9
01 1 1 8 8
02 2 2 7 7
03 3 3 6 6
04 4 4 5 5
05 5 5 4 4
06 6 6 3 3
07 7 7 2 2
08 8 8 1 1
09 9 9 0 0
```

# Usage #

In addition to all directives discussed for all domains, this solver honours an
additional flag:

* `--size`: length of the square grid.

which is a mandatory argument.

An example for the computation of one million distinct paths for every case
specified above is shown next:

``` sh
    $ ./grid --size 10 --solver "belA0" --file test-10 --variant unit --k 1000000 --no-doctor
    
      size         : 10
      solver       : belA0
      file         : test-10 (10 instances)
      variant      : unit
      size         : 10
      K            : [1000000, 1000000, 1] 

      ⏺ belA0 ( k=1000000 ): 
      ⏵ 00;1000000;(0, 0);(9, 9);18;99;1.35236;BELA*;? Unchecked
      ⏵ 01;1000000;(1, 1);(8, 8);14;99;1.06715;BELA*;? Unchecked
      ⏵ 02;1000000;(2, 2);(7, 7);10;99;1.19695;BELA*;? Unchecked
      ⏵ 03;1000000;(3, 3);(6, 6);6;99;1.11041;BELA*;? Unchecked
      ⏵ 04;1000000;(4, 4);(5, 5);2;99;1.0797;BELA*;? Unchecked
      ⏵ 05;1000000;(5, 5);(4, 4);2;99;1.09474;BELA*;? Unchecked
      ⏵ 06;1000000;(6, 6);(3, 3);6;99;1.13207;BELA*;? Unchecked
      ⏵ 07;1000000;(7, 7);(2, 2);10;99;1.1462;BELA*;? Unchecked
      ⏵ 08;1000000;(8, 8);(1, 1);14;99;1.0636;BELA*;? Unchecked
      ⏵ 09;1000000;(9, 9);(0, 0);18;99;1.34752;BELA*;? Unchecked

      📊 Error summary: <doctor disabled>
      ⚠ No CSV output generated!
      🕒 CPU time: 14.3588 seconds
```

Note the importance of using `no-doctor` here and that `--verbose` is unfeasible
due to the high value of *k*

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
Computer Science and Engineering Department https://www.inf.uc3m.es/en
Universidad Carlos III de Madrid https://www.uc3m.es/home
