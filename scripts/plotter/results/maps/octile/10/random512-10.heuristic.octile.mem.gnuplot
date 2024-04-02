#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/10/random512-10.heuristic.octile.mem.gnuplot
#
# Started on 04/02/2024 19:55:57
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.octile.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 38.91
	5 39.78
	10 38.56
	50 47.59
	100 55.65
	500 59.36
	1000 68.63
	5000 138.62
	10000 211.64
end
	1 56.63
	5 91.67
	10 100.0
	50 118.0
	100 125.68
	500 157.8
	1000 183.11
end
	1 26.96
	5 48.2
	10 107.75
	50 350.91
	100 422.52
end
