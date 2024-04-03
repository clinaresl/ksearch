#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/25/random512-25.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 21:35:10
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 25 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 66.14
	5 73.0
	10 77.0
	50 82.14
	100 81.79
	500 92.14
	1000 91.71
	5000 165.58
	10000 254.65
end
	1 85.74
	5 96.71
	10 101.71
	50 111.84
	100 119.97
	500 151.55
	1000 171.09
end
	1 47.06
	5 78.77
	10 204.34
	50 559.76
	100 944.46
end
