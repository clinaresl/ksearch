#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 21:35:19
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 30 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 80.54
	5 113.0
	10 123.48
	50 128.18
	100 140.0
	500 128.32
	1000 139.57
	5000 173.22
	10000 256.57
end
	1 131.83
	5 169.36
	10 171.62
	50 186.36
	100 198.54
	500 232.61
	1000 275.86
end
	1 59.53
	5 100.7
	10 123.75
	50 251.58
	100 441.84
end
