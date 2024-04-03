#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/15/random512-15.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 21:34:26
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 15 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 138.87
	5 239.0
	10 206.0
	50 248.0
	100 206.0
	500 206.0
	1000 185.64
	5000 192.72
	10000 279.33
end
	1 265.52
	5 325.0
	10 330.49
	50 341.47
	100 355.83
	500 433.61
	1000 467.07
end
	1 72.72
	5 191.84
	10 311.4
	50 1089.38
	100 2087.03
end
