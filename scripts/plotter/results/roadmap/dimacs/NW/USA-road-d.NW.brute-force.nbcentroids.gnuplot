#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NW/USA-road-d.NW.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:55:58
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NW Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NW.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.68
	3 2.55
	4 3.23
	5 4.2
	6 4.82
	7 5.68
	8 6.3
	9 7.25
	10 7.87
	20 15.26
	30 22.42
	40 29.54
	50 36.52
	60 43.2
	70 49.75
	80 56.22
	90 62.52
	100 68.65
	200 125.51
	300 176.85
	400 223.82
	500 266.95
	600 307.92
	700 346.75
	800 384.32
	900 419.6
	1000 453.8
	2000 741.53
	3000 969.36
	4000 1163.39
	5000 1334.17
	6000 1488.03
	7000 1630.7
	8000 1762.05
	9000 1882.01
	10000 1997.9
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
