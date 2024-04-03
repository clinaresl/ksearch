#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/10/random512-10.heuristic.octile.nbcentroids.gnuplot
#
# Started on 04/02/2024 21:34:21
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set title "#Centroids - Maps 10 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.octile.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 1.87
	10 2.54
	50 4.45
	100 5.55
	500 8.4
	1000 10.14
	5000 13.34
	10000 14.96
end
	1 0.0
	5 0.0
	10 0.0
	50 0.0
	100 0.0
	500 0.0
	1000 0.0
end
	1 0.0
	5 0.0
	10 0.0
	50 0.0
	100 0.0
end
