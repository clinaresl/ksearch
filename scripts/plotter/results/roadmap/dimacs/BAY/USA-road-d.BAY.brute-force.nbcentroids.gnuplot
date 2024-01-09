#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/BAY/USA-road-d.BAY.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:30:26
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - BAY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.BAY.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.82
	3 2.75
	4 3.61
	5 4.55
	6 5.4
	7 6.34
	8 7.17
	9 8.1
	10 9.0
	20 17.68
	30 26.08
	40 34.3
	50 42.26
	60 49.99
	70 57.65
	80 65.25
	90 72.85
	100 80.32
	200 147.41
	300 207.12
	400 261.08
	500 311.93
	600 360.54
	700 405.86
	800 448.91
	900 490.89
	1000 530.17
	2000 868.88
	3000 1140.92
	4000 1377.47
	5000 1587.26
	6000 1779.28
	7000 1955.52
	8000 2118.48
	9000 2270.78
	10000 2417.15
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
