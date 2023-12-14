#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NE/USA-road-d.NE.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:55:38
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NE Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NE.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.44
	3 2.21
	4 2.69
	5 3.52
	6 4.0
	7 4.79
	8 5.2
	9 6.05
	10 6.5
	20 12.65
	30 18.48
	40 24.16
	50 29.8
	60 34.95
	70 40.22
	80 45.42
	90 50.41
	100 55.07
	200 99.62
	300 138.61
	400 174.14
	500 207.3
	600 238.26
	700 267.63
	800 295.51
	900 322.35
	1000 349.0
	2000 563.42
	3000 737.54
	4000 885.79
	5000 1016.39
	6000 1135.56
	7000 1246.41
	8000 1347.5
	9000 1442.73
	10000 1531.9
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
