#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/10/random512-10.heuristic.unit.nbcentroids.gnuplot
#
# Started on 04/02/2024 21:34:12
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - Maps 10 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.unit.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 4.64
	10 7.5
	50 14.01
	100 17.18
	500 25.31
	1000 30.46
	5000 39.72
	10000 43.38
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
