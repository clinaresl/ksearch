#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/unit/15/15puzzle.heuristic.expansions.gnuplot
#
# Started on 12/09/2023 19:56:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 15-Puzzle unit"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1424234.4
	2 1672167.65
	3 1951506.575
	4 2172058.175
	5 2183909.925
	6 2216361.025
	7 2280452.775
	8 2328218.075
	9 2387405.675
	10 2391232.975
	20 2909560.35
	30 3041468.75
	40 3060866.0
	50 3231294.275
	60 3258095.1
	70 3349538.175
	80 3377115.925
	90 3393359.925
	100 3393442.425
end
	1 1769909.625
	2 3595062.075
	3 4358364.55
end
	1 1424234.4
	2 2896697.55
	3 4361614.5
	4 6173265.75
	5 7205712.475
end
