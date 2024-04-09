#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/30/30pancake.heuristic.mem.gnuplot
#
# Started on 04/09/2024 14:21:17
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - 30-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '30pancake.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 463.91
	2 589.6
	3 716.28
	4 802.83
	5 916.57
	6 923.79
	7 886.45
	8 1071.02
	9 1079.81
	10 1411.62
	20 1847.65
	30 2559.85
	40 3713.35
	50 5854.3
	60 6748.1
	70 7162.05
	80 8028.05
	90 9193.55
	100 9904.4
	200 14393.35
	300 21328.1
	400 24497.5
	500 27222.9
	600 29718.95
	700 31494.45
	800 34631.3
	900 39316.2
end
	1 2038.93
end
	1 461.08
	2 581.44
	3 765.3
	4 866.65
	5 1037.75
	6 1089.11
	7 1116.92
	8 1233.36
	9 1316.28
	10 1745.31
	20 2959.47
	30 2988.0
	40 5008.51
	50 5660.95
	60 8621.57
	70 9902.1
	80 12441.5
	90 12863.0
	100 13498.8
	200 19876.35
	300 27154.7
	400 34082.65
	500 37434.35
	600 41202.35
end
