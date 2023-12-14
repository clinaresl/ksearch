#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.brute-force.expansions.gnuplot
#
# Started on 12/07/2023 15:24:37
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.expansions.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 2865499.08
	2 2905607.99
	3 2939745.98
	4 2951604.2
	5 2970659.24
	6 2975808.68
	7 2986506.68
	8 2999465.31
	9 3009074.71
	10 3012416.48
end
	1 2905025.5
	2 3052600.9
end
	1 2865499.08
	2 5764147.44
end
