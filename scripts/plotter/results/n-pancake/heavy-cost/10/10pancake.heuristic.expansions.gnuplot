#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.heuristic.expansions.gnuplot
#
# Started on 04/11/2024 16:45:10
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 111678.48
	2 115712.71
	3 118121.8
	4 119667.32
	5 123586.3
	6 126293.07
	7 127118.26
	8 127979.36
	9 129329.54
	10 130662.44
	20 143041.62
	30 150130.88
	40 154974.37
	50 159696.94
	60 162785.77
	70 166033.44
	80 169213.08
	90 172491.49
	100 175101.89
end
	1 114392.78
	2 139269.31
	3 167311.88
	4 179339.99
	5 187885.53
	6 207425.1
	7 223291.15
	8 238880.32
	9 247490.42
	10 253205.45
	20 320020.44
end
	1 111678.48
	2 220928.78
	3 328706.86
	4 431463.74
	5 545180.49
	6 654069.39
	7 751896.91
	8 854825.54
	9 960337.53
	10 1066913.43
	20 2097869.59
end
