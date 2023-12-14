#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/CAL/USA-road-d.CAL.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:51:50
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - CAL Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.CAL.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.01
	4 1.01
	5 1.02
	6 1.02
	7 1.03
	8 1.03
	9 1.05
	10 1.05
	20 1.1
	30 1.17
	40 1.27
	50 1.36
	60 1.4
	70 1.46
	80 1.51
	90 1.55
	100 1.6
	200 2.04
	300 2.42
	400 3.18
	500 3.92
	600 4.63
	700 5.3
	800 5.98
	900 6.67
	1000 7.47
	2000 11.94
	3000 13.81
	4000 16.67
	5000 18.58
	6000 20.13
	7000 20.87
	8000 21.45
	9000 22.07
	10000 22.95
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
