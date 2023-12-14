#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/BAY/USA-road-d.BAY.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:51:39
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - BAY Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.BAY.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.02
	4 1.02
	5 1.05
	6 1.05
	7 1.08
	8 1.08
	9 1.14
	10 1.14
	20 1.38
	30 1.61
	40 1.83
	50 2.08
	60 2.18
	70 2.38
	80 2.61
	90 2.77
	100 2.97
	200 4.72
	300 6.43
	400 7.79
	500 9.51
	600 11.14
	700 12.57
	800 14.17
	900 15.48
	1000 16.75
	2000 31.15
	3000 41.61
	4000 51.54
	5000 58.67
	6000 64.38
	7000 69.87
	8000 74.98
	9000 79.19
	10000 84.49
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
