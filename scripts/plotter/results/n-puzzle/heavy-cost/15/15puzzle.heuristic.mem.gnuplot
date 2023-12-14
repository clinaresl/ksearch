#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/heavy-cost/15/15puzzle.heuristic.mem.gnuplot
#
# Started on 12/09/2023 19:56:49
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 15-Puzzle heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 15824.6
	2 25962.6
	3 28296.95
	4 28697.9
	5 28770.0
	6 28770.0
	7 28770.0
	8 28770.0
	9 28770.0
	10 28770.0
end
	1 43985.225
end
	1 14538.35
	2 27020.325
	3 34132.9
end
