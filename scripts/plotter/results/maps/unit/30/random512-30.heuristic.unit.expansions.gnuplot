#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 19:57:02
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 29038.96
	5 29039.69
	10 29039.92
	50 29041.57
	100 29042.04
	500 29043.77
	1000 29044.32
	5000 29047.34
	10000 29048.69
end
	1 27321.11
	5 34980.56
	10 34980.56
	50 34980.56
	100 34980.56
	500 34980.56
	1000 34980.56
end
	1 29038.96
	5 145708.04
	10 291192.51
	50 1447465.04
	100 2892114.46
end
