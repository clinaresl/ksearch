#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/10/10pancake.brute-force.mem.gnuplot
#
# Started on 12/07/2023 15:24:30
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 10-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.mem.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 4984.12
	2 4974.63
	3 5100.6
	4 5106.52
	5 5105.15
	6 5107.37
	7 5107.58
	8 5107.9
	9 5108.14
	10 5108.29
end
	1 24279.74
	2 25189.48
	3 25430.95
	4 25461.47
	5 25476.73
end
	1 4349.86
	2 6427.79
	3 8683.1
end
