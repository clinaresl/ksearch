#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/LKS/USA-road-d.LKS.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:29:18
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - LKS Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.LKS.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.0
	4 1.0
	5 1.01
	6 1.01
	7 1.02
	8 1.02
	9 1.02
	10 1.02
	20 1.04
	30 1.05
	40 1.08
	50 1.11
	60 1.13
	70 1.18
	80 1.2
	90 1.23
	100 1.25
	200 1.34
	300 1.43
	400 1.76
	500 2.07
	600 2.37
	700 2.66
	800 2.99
	900 3.29
	1000 3.61
	2000 6.59
	3000 7.58
	4000 8.1
	5000 8.43
	6000 8.64
	7000 8.88
	8000 9.06
	9000 9.31
	10000 9.49
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
