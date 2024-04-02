#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/25/random512-25.heuristic.octile.nbcentroids.gnuplot
#
# Started on 04/02/2024 19:56:56
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "nbcentroids"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.octile.nbcentroids.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 1.0
	5 1.0
	10 1.0
	50 1.0
	100 1.0
	500 1.0
	1000 1.0
	5000 1.0
	10000 1.0
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
