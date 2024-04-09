#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.brute-force.nbcentroids.gnuplot
#
# Started on 04/09/2024 14:21:04
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.66
	3 2.4
	4 2.98
	5 3.66
	6 4.23
	7 4.81
	8 5.42
	9 6.01
	10 6.58
end
	1 0.0
	2 0.0
end
	1 0.0
	2 0.0
end
