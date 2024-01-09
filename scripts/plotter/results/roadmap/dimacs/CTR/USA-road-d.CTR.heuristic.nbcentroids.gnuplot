#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/CTR/USA-road-d.CTR.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:37:24
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - CTR Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.CTR.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp

	1 1.0
	2 1.25
	3 1.64
	4 1.93
	5 2.47
	6 2.71
	7 3.15
	8 3.37
	9 4.01
	10 4.25
	20 7.78
	30 11.02
	40 14.34
	50 17.64
	60 20.57
	70 23.54
	80 26.38
	90 29.3
	100 32.06
	200 56.85
	300 78.2
	400 97.33
	500 114.87
	600 132.04
	700 147.7
	800 161.87
	900 176.71
	1000 189.96
	2000 302.12
	3000 389.87
	4000 462.18
	5000 529.45
	6000 588.42
	7000 643.9
	8000 692.31
	9000 738.17
	10000 782.07
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
