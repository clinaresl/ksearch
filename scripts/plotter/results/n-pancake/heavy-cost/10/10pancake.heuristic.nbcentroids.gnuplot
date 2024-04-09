#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.heuristic.nbcentroids.gnuplot
#
# Started on 04/09/2024 14:21:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.7
	3 2.4
	4 2.98
	5 3.7
	6 4.26
	7 4.85
	8 5.53
	9 6.04
	10 6.62
	20 11.71
	30 16.47
	40 20.7
	50 24.56
	60 28.43
	70 32.02
	80 35.66
	90 38.94
	100 42.5
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
