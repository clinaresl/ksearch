#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/COL/USA-road-d.COL.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:54:02
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - COL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.COL.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.7
	3 2.64
	4 3.34
	5 4.32
	6 5.0
	7 5.94
	8 6.63
	9 7.6
	10 8.25
	20 15.84
	30 23.16
	40 30.07
	50 36.84
	60 43.21
	70 49.83
	80 55.78
	90 61.85
	100 68.09
	200 122.95
	300 170.34
	400 213.47
	500 254.26
	600 292.17
	700 327.86
	800 361.05
	900 392.39
	1000 423.08
	2000 679.45
	3000 880.68
	4000 1051.23
	5000 1202.34
	6000 1339.26
	7000 1463.58
	8000 1577.4
	9000 1685.37
	10000 1786.44
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
