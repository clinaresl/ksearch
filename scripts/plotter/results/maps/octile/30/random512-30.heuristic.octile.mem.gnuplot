#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/30/random512-30.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 21:35:28
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 30 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 69.27
	5 80.87
	10 88.0
	50 90.86
	100 93.87
	500 98.81
	1000 114.3
	5000 170.71
	10000 274.99
end
	1 95.96
	5 106.11
	10 111.86
	50 119.86
	100 136.79
	500 162.35
	1000 188.35
end
	1 49.45
	5 77.24
	10 234.9
	50 770.29
	100 1632.8
end
