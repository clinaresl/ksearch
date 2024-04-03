#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/25/random512-25.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 21:35:04
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - Maps 25 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 29288.04
	5 29291.15
	10 29293.48
	50 29300.69
	100 29303.92
	500 29313.3
	1000 29318.81
	5000 29327.96
	10000 29331.41
end
	1 29014.37
	5 71128.52
	10 71128.52
	50 71128.52
	100 71128.52
	500 71128.52
	1000 71128.52
end
	1 29288.04
	5 144978.79
	10 288311.76
	50 1431643.71
	100 2852704.86
end
