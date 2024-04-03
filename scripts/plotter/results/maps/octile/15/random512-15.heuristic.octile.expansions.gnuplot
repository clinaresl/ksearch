#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 21:34:37
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - Maps 15 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 13385.11
	5 13385.18
	10 13385.29
	50 13385.61
	100 13385.73
	500 13386.33
	1000 13386.49
	5000 13386.82
	10000 13386.93
end
	1 13783.62
	5 15991.08
	10 16746.02
	50 17465.93
	100 17791.04
	500 18029.61
	1000 18038.98
end
	1 13385.11
	5 66686.2
	10 133248.49
	50 664505.74
	100 1328225.23
end
