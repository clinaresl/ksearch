#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/20/random512-20.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 19:56:24
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 180.64
	5 230.0
	10 230.0
	50 229.4
	100 211.82
	500 192.84
	1000 217.58
	5000 194.24
	10000 287.28
end
	1 228.48
	5 252.6
	10 263.0
	50 273.69
	100 284.19
	500 389.05
	1000 457.43
end
	1 115.22
	5 187.59
	10 279.05
	50 921.65
	100 1779.79
end
