#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/CAL/USA-road-d.CAL.heuristic.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:57:15
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - CAL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.CAL.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.55
	3 2.32
	4 2.88
	5 3.69
	6 4.23
	7 5.06
	8 5.63
	9 6.48
	10 7.02
	20 13.81
	30 20.39
	40 26.82
	50 33.18
	60 39.31
	70 45.45
	80 51.34
	90 57.22
	100 63.03
	200 116.29
	300 164.72
	400 209.1
	500 250.11
	600 289.16
	700 326.3
	800 362.18
	900 395.11
	1000 427.1
	2000 702.11
	3000 926.95
	4000 1122.17
	5000 1291.98
	6000 1446.5
	7000 1589.95
	8000 1719.18
	9000 1841.34
	10000 1958.31
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
