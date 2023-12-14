#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/40/40pancake.heuristic.nbcentroids.gnuplot
#
# Started on 12/07/2023 15:24:59
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 40-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '40pancake.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.99
	3 2.98
	4 3.95
	5 4.93
	6 5.9
	7 6.88
	8 7.85
	9 8.83
	10 9.78
	20 19.52
	30 29.1
	40 38.69
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
end
