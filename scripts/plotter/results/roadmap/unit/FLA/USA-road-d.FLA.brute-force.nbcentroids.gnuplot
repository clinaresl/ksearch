#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/FLA/USA-road-d.FLA.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:52:21
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - FLA Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.FLA.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.0
	3 1.0
	4 1.0
	5 1.02
	6 1.02
	7 1.03
	8 1.03
	9 1.07
	10 1.07
	20 1.15
	30 1.26
	40 1.31
	50 1.43
	60 1.49
	70 1.57
	80 1.63
	90 1.74
	100 1.82
	200 2.5
	300 3.12
	400 3.65
	500 4.24
	600 4.85
	700 5.52
	800 6.15
	900 6.75
	1000 7.4
	2000 13.04
	3000 16.44
	4000 20.37
	5000 24.02
	6000 28.65
	7000 33.8
	8000 38.25
	9000 42.66
	10000 47.38
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
