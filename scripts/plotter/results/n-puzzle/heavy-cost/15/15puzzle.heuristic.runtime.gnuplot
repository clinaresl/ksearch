#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/heavy-cost/15/15puzzle.heuristic.runtime.gnuplot
#
# Started on 12/09/2023 19:56:48
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 15-Puzzle heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 5.2614129975
	2 5.4259285175
	3 5.7489382475
	4 5.738816474999999
	5 5.806451545
	6 5.849303109999999
	7 5.902509637500001
	8 5.897955525
	9 5.928721059999999
	10 6.167045325
end
	1 18.58468248
end
	1 4.981759085
	2 8.9508330925
	3 13.589430557499998
end
