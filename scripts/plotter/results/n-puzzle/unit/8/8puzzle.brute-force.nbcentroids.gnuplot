#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/unit/8/8puzzle.brute-force.nbcentroids.gnuplot
#
# Started on 12/09/2023 19:56:10
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 8-Puzzle unit"

set terminal png enhanced font "Ariel,10"
set output '8puzzle.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.48
	3 2.01
	4 2.41
	5 2.91
	6 3.4
	7 3.99
	8 4.64
	9 5.34
	10 6.01
	20 13.96
	30 22.37
	40 30.61
	50 37.86
	60 44.49
	70 51.23
	80 58.26
	90 64.62
	100 70.34
	200 117.49
	300 153.54
	400 181.58
	500 199.23
	600 212.29
	700 222.17
	800 230.36
	900 239.42
	1000 249.6
	2000 341.42
	3000 412.37
	4000 462.04
	5000 514.94
	6000 560.34
	7000 597.06
	8000 623.33
	9000 652.5
	10000 678.35
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
