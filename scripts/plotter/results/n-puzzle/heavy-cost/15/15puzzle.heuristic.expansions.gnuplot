#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/heavy-cost/15/15puzzle.heuristic.expansions.gnuplot
#
# Started on 12/09/2023 19:56:49
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 15-Puzzle heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 2631269.225
	2 2777954.125
	3 2903006.875
	4 2909562.125
	5 2935147.475
	6 2936207.425
	7 2960122.375
	8 2971537.75
	9 2979007.625
	10 3083726.15
end
	1 2773920.575
end
	1 2631269.225
	2 5310034.125
	3 8227468.075
end
