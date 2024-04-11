#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.brute-force.mem.gnuplot
#
# Started on 04/11/2024 16:44:38
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.mem.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 5995.18
	2 6077.47
	3 6068.0
	4 6068.0
	5 6068.0
	6 6068.0
	7 6068.0
	8 6068.0
	9 6068.0
	10 6068.0
end
	1 24872.32
	2 25250.15
	3 25425.54
	4 25448.4
	5 25456.02
end
	1 4697.24
	2 5531.04
end
