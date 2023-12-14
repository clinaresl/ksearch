#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/LKS/USA-road-d.LKS.heuristic.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:58:50
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - LKS Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.LKS.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.39
	3 1.98
	4 2.41
	5 3.09
	6 3.5
	7 4.16
	8 4.54
	9 5.24
	10 5.69
	20 10.89
	30 15.72
	40 20.26
	50 24.9
	60 29.1
	70 33.65
	80 37.84
	90 42.01
	100 46.1
	200 82.59
	300 114.29
	400 144.0
	500 170.76
	600 195.56
	700 220.21
	800 242.68
	900 264.09
	1000 285.03
	2000 453.72
	3000 590.86
	4000 706.85
	5000 809.8
	6000 901.85
	7000 987.06
	8000 1063.95
	9000 1134.68
	10000 1203.49
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
end
