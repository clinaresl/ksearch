#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/USA/USA-road-d.USA.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:41:12
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - USA Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.USA.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp

	1 1.0
	2 1.18
	3 1.51
	4 1.69
	5 2.17
	6 2.33
	7 2.66
	8 2.82
	9 3.38
	10 3.56
	20 6.48
	30 9.38
	40 12.07
	50 14.73
	60 17.06
	70 19.57
	80 21.85
	90 24.31
	100 26.6
	200 47.12
	300 65.73
	400 82.17
	500 97.14
	600 112.1
	700 125.76
	800 138.44
	900 150.94
	1000 162.62
	2000 258.17
	3000 333.12
	4000 398.49
	5000 458.19
	6000 511.1
	7000 557.05
	8000 601.05
	9000 642.51
	10000 680.16
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
	2000 0.0
	3000 0.0
	4000 0.0
	5000 0.0
	6000 0.0
	7000 0.0
	8000 0.0
	9000 0.0
	10000 0.0
end
