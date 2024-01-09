#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NW/USA-road-d.NW.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:39:50
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NW Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NW.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.68
	3 2.55
	4 3.23
	5 4.2
	6 4.82
	7 5.68
	8 6.31
	9 7.26
	10 7.88
	20 15.3
	30 22.51
	40 29.68
	50 36.75
	60 43.5
	70 50.18
	80 56.75
	90 63.1
	100 69.29
	200 127.59
	300 180.4
	400 228.81
	500 273.84
	600 317.06
	700 358.09
	800 397.46
	900 434.83
	1000 470.93
	2000 778.42
	3000 1027.18
	4000 1238.68
	5000 1423.4
	6000 1592.57
	7000 1754.56
	8000 1899.79
	9000 2032.26
	10000 2161.96
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
