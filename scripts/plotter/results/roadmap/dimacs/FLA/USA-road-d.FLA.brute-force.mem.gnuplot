#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/FLA/USA-road-d.FLA.brute-force.mem.gnuplot
#
# Started on 01/09/2024 14:32:32
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - FLA Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.FLA.brute-force.mem.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1536.71
	2 1547.39
	3 1548.0
	4 1548.0
	5 1548.0
	6 1548.0
	7 1548.0
	8 1549.32
	9 1592.0
	10 1592.0
	20 1592.0
	30 1592.0
	40 1592.0
	50 1592.0
	60 1592.0
	70 1592.0
	80 1592.0
	90 1592.0
	100 1592.0
	200 1592.0
	300 1592.0
	400 1592.0
	500 1592.0
	600 1592.0
	700 1592.0
	800 1584.31
	900 1583.84
	1000 1591.0
	2000 1579.08
	3000 1580.75
	4000 1595.21
	5000 1584.09
	6000 1642.82
	7000 1672.09
	8000 1670.39
	9000 1660.52
	10000 1689.77
end
	1 2448.65
	2 2559.17
	3 2586.96
	4 2611.27
	5 2618.42
	6 2620.81
	7 2623.03
	8 2624.81
	9 2627.03
	10 2628.81
	20 2632.2
	30 2636.2
	40 2639.81
	50 2641.64
	60 2641.64
	70 2646.77
	80 2660.64
	90 2660.64
	100 2662.2
	200 2669.77
	300 2686.35
	400 2688.96
	500 2682.26
	600 2691.92
	700 2699.85
	800 2736.62
	900 2803.01
	1000 2754.64
	2000 2932.68
	3000 3062.79
	4000 3235.6
	5000 3412.44
	6000 3599.71
	7000 3800.67
	8000 4020.11
	9000 4171.65
	10000 4342.83
end
	1 1214.67
	2 1562.29
	3 2146.42
	4 2567.25
	5 3135.83
	6 3562.58
	7 3967.72
	8 4441.98
	9 5110.21
	10 5530.82
end