#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/25/random512-25.heuristic.unit.nbcentroids.gnuplot
#
# Started on 04/02/2024 21:35:06
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - Maps 25 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.unit.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 3.38
	10 4.83
	50 9.09
	100 10.76
	500 15.51
	1000 18.4
	5000 23.41
	10000 25.58
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
