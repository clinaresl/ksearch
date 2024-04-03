#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.heuristic.unit.nbcentroids.gnuplot
#
# Started on 04/02/2024 21:35:23
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - Maps 30 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.unit.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 1.39
	10 1.59
	50 2.28
	100 2.55
	500 3.36
	1000 3.6
	5000 4.33
	10000 4.72
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
