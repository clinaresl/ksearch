#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NY/USA-road-d.NY.heuristic.runtime.gnuplot
#
# Started on 01/09/2024 14:39:57
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - NY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 0.38620563999999996
	2 0.3916335
	3 0.39152396
	4 0.39153541
	5 0.3925430200000001
	6 0.3938363300000001
	7 0.39310772000000005
	8 0.39239244
	9 0.39433687
	10 0.39368319
	20 0.39731795000000003
	30 0.40009047000000003
	40 0.40357319999999997
	50 0.4072956599999999
	60 0.41331228
	70 0.41454518000000007
	80 0.41375437000000004
	90 0.41873420999999994
	100 0.42099013999999996
	200 0.45024076
	300 0.48040144999999995
	400 0.50884109
	500 0.53520929
	600 0.56403659
	700 0.59012352
	800 0.6178618400000001
	900 0.64350213
	1000 0.6661779399999999
	2000 0.9215405500000001
	3000 1.16893182
	4000 1.4129946000000002
	5000 1.6535723300000003
	6000 1.8938542600000003
	7000 2.12128104
	8000 2.3461696099999996
	9000 2.5768674300000005
	10000 2.80976275
end
	1 0.6530995900000001
	2 0.65438085
	3 0.65756106
	4 0.6577947199999999
	5 0.6599884699999999
	6 0.6611799700000001
	7 0.6621344299999999
	8 0.6622485800000001
	9 0.6633338500000001
	10 0.6597092600000001
	20 0.66309424
	30 0.6661404900000001
	40 0.6668561
	50 0.6700683800000001
	60 0.6723782
	70 0.6731094199999998
	80 0.6756019500000001
	90 0.67822722
	100 0.6747205700000002
	200 0.6856843400000001
	300 0.69988654
	400 0.7109772200000001
	500 0.7294155600000002
	600 0.74383322
	700 0.7563604799999998
	800 0.7714630200000001
	900 0.7839183700000001
	1000 0.78988134
	2000 0.90839714
	3000 1.0288783799999999
	4000 1.1487142499999998
	5000 1.2772615
	6000 1.4026953100000004
	7000 1.52742208
	8000 1.6531736299999997
	9000 1.7846394899999998
	10000 1.9156270199999996
end
	1 0.34241666000000004
	2 0.5799951
	3 0.8306956000000001
	4 1.07311689
	5 1.3413104999999999
	6 1.6009908000000002
	7 1.8707664499999999
	8 2.13565618
	9 2.44227559
	10 2.7191089
end