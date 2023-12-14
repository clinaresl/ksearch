#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/W/USA-road-d.W.heuristic.nbcentroids.gnuplot
#
# Started on 12/13/2023 19:00:11
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - W Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.W.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp

	1 1.0
	2 1.5
	3 2.32
	4 2.84
	5 3.75
	6 4.25
	7 5.07
	8 5.54
	9 6.44
	10 6.94
	20 13.13
	30 19.25
	40 24.98
	50 30.78
	60 36.06
	70 41.2
	80 46.24
	90 51.12
	100 56.06
	200 99.32
	300 136.76
	400 170.48
	500 201.29
	600 229.69
	700 255.93
	800 281.46
	900 304.83
	1000 327.93
	2000 519.65
	3000 670.89
	4000 802.13
	5000 915.68
	6000 1018.79
	7000 1114.81
	8000 1200.41
	9000 1279.05
	10000 1350.97
end
	1 0.0
	2 0.0
	3 0.0
	4 0.0
	5 0.0
	6 0.0
	7 0.0
	8 0.0
	9 0.0
	10 0.0
	20 0.0
	30 0.0
	40 0.0
	50 0.0
	60 0.0
	70 0.0
	80 0.0
	90 0.0
	100 0.0
	200 0.0
	300 0.0
	400 0.0
	500 0.0
	600 0.0
	700 0.0
	800 0.0
	900 0.0
	1000 0.0
	2000 0.0
	3000 0.0
	4000 0.0
	5000 0.0
	6000 0.0
	7000 0.0
	8000 0.0
	9000 0.0
	10000 0.0
end
