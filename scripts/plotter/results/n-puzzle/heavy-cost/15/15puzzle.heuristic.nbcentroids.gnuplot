#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/heavy-cost/15/15puzzle.heuristic.nbcentroids.gnuplot
#
# Started on 12/09/2023 19:56:50
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 15-Puzzle heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.65
	3 2.525
	4 3.275
	5 4.15
	6 4.875
	7 5.625
	8 6.275
	9 6.925
	10 7.55
end
	1 0.0
end
	1 0.0
	2 0.0
	3 0.0
end
