#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/unit/15/15puzzle.heuristic.nbcentroids.gnuplot
#
# Started on 12/09/2023 19:56:35
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - 15-Puzzle unit"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.65
	3 2.3
	4 2.875
	5 3.6
	6 4.25
	7 4.975
	8 5.6
	9 6.3
	10 6.95
	20 14.3
	30 22.05
	40 30.025
	50 37.825
	60 45.325
	70 52.65
	80 59.675
	90 66.525
	100 73.025
end
	1 0.0
	2 0.0
	3 0.0
end
	1 0.0
	2 0.0
	3 0.0
	4 0.0
	5 0.0
end
