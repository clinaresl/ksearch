#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.brute-force.runtime.gnuplot
#
# Started on 12/07/2023 15:24:33
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 29.045735800000003
	2 29.921466600000002
	3 30.694983199999996
	4 31.039786499999995
	5 31.385189499999996
	6 31.5707795
	7 31.790656200000004
	8 32.047626300000005
	9 32.2334452
	10 32.31170110000001
end
	1 78.5818656
	2 86.8447667
end
	1 25.8814132
	2 52.55856740000001
end
