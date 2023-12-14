#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/30/30pancake.heuristic.nbcentroids.gnuplot
#
# Started on 12/07/2023 15:24:55
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 30-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '30pancake.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 2.0
	3 3.0
	4 4.0
	5 4.98
	6 5.97
	7 6.96
	8 7.93
	9 8.92
	10 9.9
	20 19.61
	30 29.16
	40 38.72
	50 48.2
	60 57.68
	70 67.07
	80 76.24
	90 85.52
	100 94.35
	200 176.82
	300 246.4
	400 306.64
	500 358.72
	600 405.41
	700 445.99
	800 485.09
	900 521.66
end
	1 0.0
end
	1 0.0
	2 0.0
	3 0.0
	4 0.0
	5 0.0
	6 0.0
	7 0.0
	8 0.0
	9 0.0
	10 0.0
	20 0.0
	30 0.0
	40 0.0
	50 0.0
	60 0.0
	70 0.0
	80 0.0
	90 0.0
	100 0.0
	200 0.0
	300 0.0
	400 0.0
	500 0.0
	600 0.0
end
