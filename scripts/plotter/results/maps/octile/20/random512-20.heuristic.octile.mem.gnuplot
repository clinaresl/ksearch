#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/20/random512-20.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 19:56:33
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 55.31
	5 57.37
	10 58.13
	50 63.12
	100 68.35
	500 82.17
	1000 91.45
	5000 124.29
	10000 265.95
end
	1 72.01
	5 85.92
	10 90.92
	50 101.57
	100 107.24
	500 139.92
	1000 158.77
end
	1 40.28
	5 83.61
	10 136.54
	50 481.62
	100 1131.45
end
