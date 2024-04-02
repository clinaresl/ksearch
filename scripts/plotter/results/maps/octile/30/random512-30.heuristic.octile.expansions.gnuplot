#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/30/random512-30.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 19:57:11
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 26274.01
	5 26274.01
	10 26274.01
	50 26274.01
	100 26274.01
	500 26274.01
	1000 26274.01
	5000 26274.01
	10000 26274.01
end
	1 26273.27
	5 28446.01
	10 28990.72
	50 29810.59
	100 30036.89
	500 30116.15
	1000 30126.79
end
	1 26274.01
	5 131364.56
	10 262669.41
	50 1312969.59
	100 2626600.53
end
