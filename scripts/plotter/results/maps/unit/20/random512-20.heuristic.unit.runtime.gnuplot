#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/20/random512-20.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 19:56:21
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.0363708023
	5 0.036451540899999996
	10 0.036626274
	50 0.0380119394
	100 0.039188286
	500 0.04367752200000001
	1000 0.04393515400000001
	5000 0.081851039
	10000 0.136376121
end
	1 0.09002366099999999
	5 1.3243339399999998
	10 1.3219959000000001
	50 1.3417419099999999
	100 1.3474275999999998
	500 1.4518854600000004
	1000 1.56139041
end
	1 0.032900686900000004
	5 0.12584579299999998
	10 0.24045195200000002
	50 1.3020569699999998
	100 3.684675409999999
end
