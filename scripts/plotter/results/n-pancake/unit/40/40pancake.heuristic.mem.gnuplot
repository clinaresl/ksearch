#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/40/40pancake.heuristic.mem.gnuplot
#
# Started on 04/09/2024 14:21:23
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 40-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '40pancake.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 5778.1
	2 8969.52
	3 15938.05
	4 15931.72
	5 16131.12
	6 16133.72
	7 16863.91
	8 17476.62
	9 17945.12
	10 19000.14
	20 22701.99
	30 31786.51
	40 38396.69
end
	1 25859.42
end
	1 5677.61
	2 8258.9
	3 17701.0
	4 17780.14
	5 19042.8
	6 19417.22
	7 20379.78
	8 21162.53
	9 21803.58
	10 23489.86
	20 28206.66
end
