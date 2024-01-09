#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NY/USA-road-d.NY.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:34:39
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.59
	3 2.49
	4 3.07
	5 4.03
	6 4.6
	7 5.48
	8 6.02
	9 6.97
	10 7.52
	20 14.52
	30 21.22
	40 27.49
	50 33.95
	60 39.91
	70 45.75
	80 51.59
	90 57.33
	100 63.12
	200 114.09
	300 159.59
	400 201.37
	500 239.87
	600 275.38
	700 309.42
	800 341.28
	900 372.69
	1000 402.26
	2000 650.63
	3000 846.54
	4000 1014.04
	5000 1160.97
	6000 1293.41
	7000 1414.14
	8000 1527.5
	9000 1632.19
	10000 1731.45
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
