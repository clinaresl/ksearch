#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/15/random512-15.heuristic.unit.nbcentroids.gnuplot
#
# Started on 04/02/2024 21:34:30
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - Maps 15 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.unit.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 4.52
	10 6.86
	50 13.0
	100 15.99
	500 23.31
	1000 26.86
	5000 35.58
	10000 39.91
end
	1 0.0
	5 0.0
	10 0.0
	50 0.0
	100 0.0
	500 0.0
	1000 0.0
end
	1 0.0
	5 0.0
	10 0.0
	50 0.0
	100 0.0
end
