#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.heuristic.octile.nbcentroids.gnuplot
#
# Started on 04/02/2024 19:56:19
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.octile.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 1.07
	10 1.1
	50 1.19
	100 1.23
	500 1.3
	1000 1.33
	5000 1.42
	10000 1.5
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
