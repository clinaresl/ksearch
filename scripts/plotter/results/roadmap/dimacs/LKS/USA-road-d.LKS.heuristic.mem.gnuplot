#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/LKS/USA-road-d.LKS.heuristic.mem.gnuplot
#
# Started on 01/09/2024 14:38:38
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - LKS Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.LKS.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "mA*"      with linesp

	1 3321.55
	2 3387.72
	3 3412.86
	4 3442.2
	5 3446.2
	6 3450.2
	7 3463.48
	8 3493.4
	9 3493.4
	10 3490.72
	20 3476.22
	30 3513.4
	40 3513.4
	50 3513.4
	60 3513.4
	70 3513.4
	80 3505.02
	90 3492.66
	100 3503.32
	200 3488.38
	300 3490.02
	400 3493.05
	500 3500.89
	600 3500.61
	700 3504.81
	800 3508.42
	900 3512.85
	1000 3508.25
	2000 3536.16
	3000 3566.0
	4000 3656.17
	5000 3740.65
	6000 3745.45
	7000 3753.37
	8000 3898.62
	9000 4096.11
	10000 4122.28
end
	1 2751.07
	2 3808.43
	3 5408.76
	4 6079.42
	5 8150.44
	6 9272.32
	7 9798.2
	8 10911.98
	9 13665.02
	10 14538.73
end
