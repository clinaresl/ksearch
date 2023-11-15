# N-pancake #

Pancake Flipping is the problem of sorting a stack of pancakes of different
sizes (that is, a permutation of integers in the range *[0, n)*), where the only
allowed operation is to insert a spatula anywhere in the stack and to flip the
pancakes above it (that is, to perform a prefix reversal). 

This domain provides two variants:

* `unit`: all operators cost the same and thus, they are equal to one. The
  heuristic implemented is the GAP heuristic.
  
* `heavy-cost`: every operator has an associated cost equal to the size of the
  disc that becomes first when applying the operator. This has two consequences:
  first, there are null-cost edges (since 0 is a valid disc id); second, it
  engenders a directed graph because the cost of an operator and its inverse are
  not equal. The heuristic implemented is a weighted GAP heuristic.

The test file consist of a line per optimization task with *n+1* digits in each
line. The first digit is the task id; the next *n* digits (that must be integers
in the range *[0, n)*) must be a valid permutation representing the start state.
The goal state is always assumed to be the identity permutation. An example
follows:

``` text
0 1 0 6 4 8 2 5 3 9 7
1 3 7 2 5 8 1 9 6 0 4
2 4 9 6 8 0 3 5 1 7 2
3 0 8 3 9 1 7 4 5 6 2
4 1 8 6 4 9 5 7 2 0 3
5 9 8 4 0 3 5 7 1 6 2
6 2 1 6 4 8 9 3 7 0 5
7 1 5 8 9 3 0 7 6 4 2
8 6 7 9 8 5 4 1 2 3 0
9 3 9 4 2 8 7 5 1 6 0
```

# Usage #

This solver accepts the same flags discussed in the entry above under `domains/`
and no other specific directives are provided.

An example for the computation of test distinct paths for every case specified
above under the heavy-cost variant is shown next:

``` sh
    $ ./npancake --solver "belA0" --file test-10 --variant heavy-cost --k 10
    
      solver       : belA0
      file         : test-10 (10 instances)
      variant      : heavy-cost
      size         : 10
      K            : [10, 10, 1] 

      ⏺ belA0 ( k=10 ): 
      ⏵ 0;10;1 0 6 4 8 2 5 3 9 7 ;0 1 2 3 4 5 6 7 8 9 ;32;3610604;38.47;BELA*;✔ No error
      ⏵ 1;10;3 7 2 5 8 1 9 6 0 4 ;0 1 2 3 4 5 6 7 8 9 ;24;3611232;40.3153;BELA*;✔ No error
      ⏵ 2;10;4 9 6 8 0 3 5 1 7 2 ;0 1 2 3 4 5 6 7 8 9 ;25;3614155;39.6024;BELA*;✔ No error
      ⏵ 3;10;0 8 3 9 1 7 4 5 6 2 ;0 1 2 3 4 5 6 7 8 9 ;16;3140162;24.7753;BELA*;✔ No error
      ⏵ 4;10;1 8 6 4 9 5 7 2 0 3 ;0 1 2 3 4 5 6 7 8 9 ;30;3625310;42.0401;BELA*;✔ No error
      ⏵ 5;10;9 8 4 0 3 5 7 1 6 2 ;0 1 2 3 4 5 6 7 8 9 ;18;1758349;9.63701;BELA*;✔ No error
      ⏵ 6;10;2 1 6 4 8 9 3 7 0 5 ;0 1 2 3 4 5 6 7 8 9 ;20;2847866;19.4298;BELA*;✔ No error
      ⏵ 7;10;1 5 8 9 3 0 7 6 4 2 ;0 1 2 3 4 5 6 7 8 9 ;17;1744411;9.56371;BELA*;✔ No error
      ⏵ 8;10;6 7 9 8 5 4 1 2 3 0 ;0 1 2 3 4 5 6 7 8 9 ;13;151096;0.561578;BELA*;✔ No error
      ⏵ 9;10;3 9 4 2 8 7 5 1 6 0 ;0 1 2 3 4 5 6 7 8 9 ;18;3104798;23.7006;BELA*;✔ No error

      📊 Error summary: 
             Number of errors: 0
      ⚠ No CSV output generated!
      🕒 CPU time: 323.304 seconds
```


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


# Author #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science and Engineering Department <https://www.inf.uc3m.es/en>  
Universidad Carlos III de Madrid <https://www.uc3m.es/home>

Ian Herman <iankherman@gmail.com>  
University of Hawaii at Hilo <https://hilo.hawaii.edu/>  

