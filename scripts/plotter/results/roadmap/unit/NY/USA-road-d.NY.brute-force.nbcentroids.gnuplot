#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/NY/USA-road-d.NY.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:53:04
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NY Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.0
	4 1.01
	5 1.02
	6 1.02
	7 1.03
	8 1.03
	9 1.04
	10 1.04
	20 1.07
	30 1.11
	40 1.19
	50 1.25
	60 1.26
	70 1.3
	80 1.34
	90 1.38
	100 1.45
	200 1.73
	300 1.85
	400 2.08
	500 2.3
	600 2.58
	700 2.84
	800 3.02
	900 3.3
	1000 3.48
	2000 7.39
	3000 10.01
	4000 10.75
	5000 11.36
	6000 12.01
	7000 12.47
	8000 12.99
	9000 13.45
	10000 14.59
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
