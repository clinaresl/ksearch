#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/35/random512-35.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 21:35:45
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 35 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 80.17
	5 88.0
	10 92.72
	50 97.0
	100 104.7
	500 111.7
	1000 128.11
	5000 147.1
	10000 254.02
end
	1 98.07
	5 110.26
	10 117.26
	50 128.52
	100 137.52
	500 162.35
	1000 189.96
end
	1 55.29
	5 143.83
	10 176.87
	50 939.99
	100 1503.66
end
