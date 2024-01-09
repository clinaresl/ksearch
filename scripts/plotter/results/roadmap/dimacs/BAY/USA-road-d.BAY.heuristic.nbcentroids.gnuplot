#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/BAY/USA-road-d.BAY.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:35:56
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - BAY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.BAY.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.82
	3 2.75
	4 3.61
	5 4.55
	6 5.41
	7 6.35
	8 7.19
	9 8.12
	10 9.01
	20 17.71
	30 26.13
	40 34.38
	50 42.36
	60 50.07
	70 57.81
	80 65.41
	90 72.99
	100 80.49
	200 147.81
	300 207.67
	400 261.74
	500 312.81
	600 361.5
	700 407.37
	800 450.73
	900 492.71
	1000 532.33
	2000 871.94
	3000 1144.22
	4000 1380.98
	5000 1590.95
	6000 1782.91
	7000 1959.44
	8000 2122.13
	9000 2275.1
	10000 2421.44
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
