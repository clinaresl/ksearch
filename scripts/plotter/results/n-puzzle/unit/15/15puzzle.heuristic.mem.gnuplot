#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/unit/15/15puzzle.heuristic.mem.gnuplot
#
# Started on 12/09/2023 19:56:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 15-Puzzle unit"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 2395.275
	2 6644.4
	3 7699.2
	4 8703.6
	5 9822.2
	6 9823.0
	7 9823.2
	8 9824.0
	9 9996.1
	10 10971.15
	20 11104.0
	30 11575.95
	40 11717.0
	50 11717.0
	60 11717.0
	70 11717.0
	80 11717.0
	90 11717.0
	100 11717.0
end
	1 9821.175
	2 32907.1
	3 40362.7
end
	1 2256.0
	2 10007.25
	3 11932.65
	4 13997.675
	5 17626.8
end
