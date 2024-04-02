#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/10/random512-10.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 19:55:59
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 6757.34
	5 6758.21
	10 6758.94
	50 6761.41
	100 6763.38
	500 6767.09
	1000 6769.3
	5000 6774.56
	10000 6776.86
end
	1 9251.91
	5 12377.43
	10 13208.27
	50 14251.64
	100 14384.48
	500 14738.24
	1000 14738.24
end
	1 6757.34
	5 33398.59
	10 66381.42
	50 329774.57
	100 658894.69
end
