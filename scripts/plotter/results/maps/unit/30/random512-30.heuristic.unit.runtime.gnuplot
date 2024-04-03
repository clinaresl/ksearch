#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 21:35:17
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 30 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.023813455999999997
	5 0.024019710799999996
	10 0.0242125532
	50 0.025011393700000002
	100 0.0248489264
	500 0.027806634799999997
	1000 0.031701254699999994
	5000 0.069238195
	10000 0.117738455
end
	1 0.05918293500000002
	5 0.283208141
	10 0.27882962400000005
	50 0.2878279
	100 0.293988726
	500 0.36148971
	1000 0.43148140000000007
end
	1 0.0325264741
	5 0.12812492109999998
	10 0.24665565599999997
	50 1.3398286339999999
	100 3.7395029999999996
end
