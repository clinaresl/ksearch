#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/20/random512-20.heuristic.unit.nbcentroids.gnuplot
#
# Started on 04/02/2024 19:56:28
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.unit.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 4.05
	10 5.87
	50 11.0
	100 13.2
	500 19.19
	1000 21.95
	5000 29.46
	10000 32.39
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
