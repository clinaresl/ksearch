#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/10/10pancake.brute-force.expansions.gnuplot
#
# Started on 04/09/2024 14:20:55
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 10-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.expansions.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 2364431.61
	2 2562835.5
	3 2660250.05
	4 2687476.56
	5 2732512.39
	6 2783545.59
	7 2801348.28
	8 2831192.26
	9 2842686.33
	10 2853129.41
end
	1 2492750.01
	2 3168860.61
	3 3253023.22
	4 3313270.05
	5 3331028.25
end
	1 2364431.61
	2 5196491.94
	3 7966378.19
	4 10563943.9
	5 13271017.4
end
