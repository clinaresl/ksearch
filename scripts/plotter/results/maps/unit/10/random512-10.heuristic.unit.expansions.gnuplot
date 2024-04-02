#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/10/random512-10.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 19:55:51
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 11809.25
	5 11813.09
	10 11816.19
	50 11823.35
	100 11826.92
	500 11836.06
	1000 11841.99
	5000 11852.64
	10000 11856.64
end
	1 13534.28
	5 170904.97
	10 170904.97
	50 170904.97
	100 170904.97
	500 170904.97
	1000 170904.97
end
	1 11809.25
	5 54965.83
	10 108292.38
	50 528246.21
	100 1047714.97
end
