#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/20/20pancake.heuristic.nbcentroids.gnuplot
#
# Started on 12/07/2023 15:24:48
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 20-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '20pancake.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.99
	3 2.99
	4 3.98
	5 4.95
	6 5.9
	7 6.87
	8 7.85
	9 8.81
	10 9.79
	20 19.29
	30 28.54
	40 37.54
	50 46.17
	60 54.54
	70 62.55
	80 70.07
	90 77.52
	100 84.97
	200 153.57
	300 214.23
	400 268.35
	500 320.75
	600 370.88
	700 422.4
	800 473.27
	900 524.78
	1000 578.92
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
	700 0.0
	800 0.0
	900 0.0
	1000 0.0
end
