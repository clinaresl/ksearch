#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.heuristic.mem.gnuplot
#
# Started on 04/09/2024 14:21:30
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1214.55
	2 1421.27
	3 1429.79
	4 1435.16
	5 1421.64
	6 1417.5
	7 1416.2
	8 1427.13
	9 1426.32
	10 1420.15
	20 1417.35
	30 1415.4
	40 1420.65
	50 1422.54
	60 1514.05
	70 1516.5
	80 1516.24
	90 1515.55
	100 1731.49
end
	1 4558.61
	2 4843.76
	3 4937.22
	4 5919.04
	5 5920.0
	6 5920.04
	7 5972.33
	8 5980.0
	9 5980.0
	10 5980.0
	20 10727.19
end
	1 1025.96
	2 1685.32
	3 2138.64
	4 3010.63
	5 3913.9
	6 4278.44
	7 4589.74
	8 5310.05
	9 5855.21
	10 6276.93
	20 12900.75
end
