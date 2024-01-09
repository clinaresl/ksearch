#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/NE/USA-road-d.NE.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:29:33
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NE Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NE.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.01
	4 1.02
	5 1.03
	6 1.04
	7 1.05
	8 1.06
	9 1.08
	10 1.09
	20 1.22
	30 1.31
	40 1.41
	50 1.51
	60 1.59
	70 1.7
	80 1.76
	90 1.93
	100 2.09
	200 3.32
	300 4.57
	400 5.5
	500 6.33
	600 7.22
	700 8.01
	800 8.72
	900 9.02
	1000 9.27
	2000 11.64
	3000 16.17
	4000 21.74
	5000 27.7
	6000 32.23
	7000 35.02
	8000 37.78
	9000 39.56
	10000 41.92
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
