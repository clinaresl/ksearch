#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/LKS/USA-road-d.LKS.brute-force.nbcentroids.gnuplot
#
# Started on 12/13/2023 18:55:19
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - LKS Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.LKS.brute-force.nbcentroids.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.0
	2 1.4
	3 1.99
	4 2.43
	5 3.11
	6 3.53
	7 4.18
	8 4.58
	9 5.28
	10 5.74
	20 10.99
	30 15.8
	40 20.48
	50 25.09
	60 29.27
	70 33.8
	80 38.02
	90 42.18
	100 46.17
	200 82.04
	300 113.28
	400 142.33
	500 168.87
	600 193.25
	700 217.24
	800 239.29
	900 260.05
	1000 280.24
	2000 444.55
	3000 578.22
	4000 689.59
	5000 789.16
	6000 877.57
	7000 959.11
	8000 1035.37
	9000 1105.76
	10000 1172.23
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
