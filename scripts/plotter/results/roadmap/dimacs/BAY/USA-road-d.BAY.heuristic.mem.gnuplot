#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/BAY/USA-road-d.BAY.heuristic.mem.gnuplot
#
# Started on 01/09/2024 14:35:44
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - BAY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.BAY.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "mA*"      with linesp

	1 507.99
	2 533.97
	3 534.93
	4 532.59
	5 531.37
	6 531.37
	7 525.94
	8 519.67
	9 519.33
	10 519.07
	20 518.69
	30 511.53
	40 492.73
	50 487.12
	60 486.97
	70 486.98
	80 487.27
	90 487.27
	100 487.0
	200 487.36
	300 487.74
	400 488.11
	500 487.88
	600 487.92
	700 487.92
	800 488.16
	900 488.76
	1000 488.22
	2000 499.03
	3000 507.86
	4000 511.19
	5000 517.54
	6000 534.24
	7000 537.86
	8000 548.75
	9000 553.11
	10000 577.12
end
	1 374.51
	2 506.06
	3 670.45
	4 850.76
	5 974.92
	6 1141.2
	7 1226.07
	8 1407.23
	9 1616.46
	10 1711.82
end
