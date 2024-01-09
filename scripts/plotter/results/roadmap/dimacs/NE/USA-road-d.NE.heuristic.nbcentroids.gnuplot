#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NE/USA-road-d.NE.heuristic.nbcentroids.gnuplot
#
# Started on 01/09/2024 14:39:20
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - NE Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NE.heuristic.nbcentroids.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 1.0
	2 1.44
	3 2.21
	4 2.69
	5 3.52
	6 4.0
	7 4.79
	8 5.2
	9 6.05
	10 6.5
	20 12.65
	30 18.47
	40 24.18
	50 29.84
	60 35.06
	70 40.36
	80 45.52
	90 50.57
	100 55.27
	200 100.12
	300 139.47
	400 175.33
	500 208.7
	600 239.84
	700 269.3
	800 297.33
	900 324.44
	1000 351.16
	2000 567.1
	3000 741.94
	4000 890.67
	5000 1022.01
	6000 1142.22
	7000 1253.43
	8000 1354.98
	9000 1450.79
	10000 1540.32
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
