#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/CAL/USA-road-d.CAL.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:30:55
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - CAL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.CAL.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.55
	3 2.32
	4 2.88
	5 3.69
	6 4.23
	7 5.06
	8 5.63
	9 6.48
	10 7.01
	20 13.74
	30 20.23
	40 26.49
	50 32.68
	60 38.71
	70 44.81
	80 50.47
	90 56.13
	100 61.77
	200 113.65
	300 160.64
	400 203.43
	500 242.87
	600 280.6
	700 316.31
	800 350.49
	900 382.11
	1000 413.41
	2000 675.93
	3000 884.41
	4000 1061.85
	5000 1217.28
	6000 1358.53
	7000 1490.39
	8000 1610.67
	9000 1722.52
	10000 1829.07
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
