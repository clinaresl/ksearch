#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/NW/USA-road-d.NW.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:29:48
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NW Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NW.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.0
	4 1.0
	5 1.01
	6 1.01
	7 1.02
	8 1.02
	9 1.03
	10 1.03
	20 1.09
	30 1.12
	40 1.2
	50 1.29
	60 1.35
	70 1.44
	80 1.51
	90 1.58
	100 1.7
	200 2.28
	300 2.95
	400 3.43
	500 3.93
	600 4.44
	700 4.82
	800 5.24
	900 5.58
	1000 5.91
	2000 9.11
	3000 14.41
	4000 19.62
	5000 24.06
	6000 26.4
	7000 28.61
	8000 30.74
	9000 32.75
	10000 34.84
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
