#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/10/10pancake.brute-force.nbcentroids.gnuplot
#
# Started on 04/11/2024 16:44:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 10-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.59
	3 2.17
	4 2.54
	5 2.99
	6 3.37
	7 3.66
	8 4.0
	9 4.34
	10 4.67
end
	1 0.0
	2 0.0
	3 0.0
	4 0.0
	5 0.0
end
	1 0.0
	2 0.0
	3 0.0
	4 0.0
	5 0.0
end
