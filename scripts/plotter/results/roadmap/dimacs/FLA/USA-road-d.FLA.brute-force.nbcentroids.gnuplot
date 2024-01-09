#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/FLA/USA-road-d.FLA.brute-force.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:32:46
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - FLA Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.FLA.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.44
	3 2.27
	4 2.73
	5 3.6
	6 4.03
	7 4.86
	8 5.26
	9 6.14
	10 6.58
	20 12.48
	30 18.18
	40 23.49
	50 28.92
	60 34.23
	70 39.29
	80 44.33
	90 49.25
	100 54.05
	200 97.15
	300 135.35
	400 170.38
	500 203.1
	600 233.9
	700 262.9
	800 291.17
	900 317.7
	1000 342.57
	2000 557.76
	3000 731.67
	4000 881.63
	5000 1015.34
	6000 1136.41
	7000 1248.23
	8000 1350.37
	9000 1448.13
	10000 1538.73
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
