#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 21:34:35
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 15 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 47.54
	5 48.14
	10 53.64
	50 55.08
	100 61.84
	500 69.73
	1000 72.62
	5000 139.41
	10000 234.29
end
	1 65.64
	5 80.92
	10 85.8
	50 93.84
	100 99.92
	500 133.1
	1000 154.29
end
	1 34.66
	5 64.67
	10 161.95
	50 429.95
	100 506.22
end
