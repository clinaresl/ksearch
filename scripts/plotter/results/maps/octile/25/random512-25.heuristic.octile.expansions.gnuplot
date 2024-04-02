#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/25/random512-25.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 19:56:53
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 21590.39
	5 21590.39
	10 21590.39
	50 21590.39
	100 21590.39
	500 21590.39
	1000 21590.39
	5000 21590.39
	10000 21590.39
end
	1 21655.18
	5 24040.75
	10 24405.47
	50 25126.8
	100 25391.46
	500 25556.53
	1000 25578.73
end
	1 21590.39
	5 107969.47
	10 216026.46
	50 1079752.14
	100 2158730.46
end
