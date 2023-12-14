#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NY/USA-road-d.NY.heuristic.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:59:52
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.57
	3 2.49
	4 3.05
	5 4.02
	6 4.59
	7 5.47
	8 6.02
	9 6.96
	10 7.52
	20 14.53
	30 21.24
	40 27.58
	50 34.09
	60 40.03
	70 45.92
	80 51.82
	90 57.57
	100 63.37
	200 114.52
	300 160.27
	400 202.32
	500 241.78
	600 277.27
	700 311.23
	800 343.97
	900 375.82
	1000 405.42
	2000 656.32
	3000 854.87
	4000 1024.75
	5000 1172.82
	6000 1306.15
	7000 1427.79
	8000 1541.85
	9000 1646.9
	10000 1746.6
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
