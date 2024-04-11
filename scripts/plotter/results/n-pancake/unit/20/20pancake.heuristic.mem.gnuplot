#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/20/20pancake.heuristic.mem.gnuplot
#
# Started on 04/11/2024 16:44:47
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 20-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '20pancake.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 43.76
	2 53.6
	3 65.29
	4 83.94
	5 130.96
	6 133.0
	7 134.37
	8 120.79
	9 115.13
	10 120.74
	20 209.4
	30 219.76
	40 228.67
	50 238.0
	60 280.66
	70 320.56
	80 391.41
	90 527.67
	100 594.86
	200 727.13
	300 1103.11
	400 1410.35
	500 1754.66
	600 2082.57
	700 2592.69
	800 2809.06
	900 2811.03
	1000 2812.03
end
	1 128.88
	2 23910.57
	3 36834.92
	4 39387.0
	5 42481.35
	6 42562.55
	7 42572.83
	8 42563.9
	9 42573.55
	10 42565.51
end
	1 43.07
	2 66.1
	3 72.75
	4 86.99
	5 141.11
	6 161.54
	7 167.35
	8 178.24
	9 179.42
	10 300.14
	20 308.45
	30 349.25
	40 463.26
	50 507.63
	60 632.36
	70 719.25
	80 796.14
	90 932.01
	100 1003.06
	200 1223.16
	300 2023.43
	400 2608.12
	500 3305.91
	600 3810.95
	700 4586.73
	800 5656.88
	900 6060.71
	1000 6700.59
end
