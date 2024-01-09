#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/COL/USA-road-d.COL.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:28:32
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - COL Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.COL.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.01
	4 1.02
	5 1.07
	6 1.07
	7 1.11
	8 1.11
	9 1.17
	10 1.18
	20 1.48
	30 1.74
	40 2.03
	50 2.33
	60 2.5
	70 2.82
	80 3.05
	90 3.34
	100 3.58
	200 5.34
	300 6.98
	400 8.72
	500 10.48
	600 12.21
	700 13.87
	800 15.74
	900 17.9
	1000 19.93
	2000 38.79
	3000 55.75
	4000 67.45
	5000 76.16
	6000 84.8
	7000 92.16
	8000 99.11
	9000 106.2
	10000 112.78
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
