#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/COL/USA-road-d.COL.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:36:56
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - COL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.COL.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.7
	3 2.64
	4 3.34
	5 4.32
	6 5.0
	7 5.94
	8 6.63
	9 7.61
	10 8.25
	20 15.85
	30 23.19
	40 30.12
	50 36.95
	60 43.33
	70 49.96
	80 55.92
	90 61.96
	100 68.2
	200 123.15
	300 170.76
	400 214.11
	500 255.25
	600 292.98
	700 328.81
	800 361.88
	900 393.26
	1000 424.35
	2000 682.16
	3000 885.15
	4000 1054.86
	5000 1205.99
	6000 1342.76
	7000 1467.22
	8000 1582.65
	9000 1689.96
	10000 1790.2
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