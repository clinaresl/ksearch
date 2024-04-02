#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/35/random512-35.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 19:57:29
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 30316.61
	5 30316.61
	10 30316.61
	50 30316.61
	100 30316.61
	500 30316.61
	1000 30316.61
	5000 30316.61
	10000 30316.61
end
	1 30316.62
	5 32286.48
	10 32685.68
	50 33685.52
	100 33904.78
	500 33932.38
	1000 33932.38
end
	1 30316.61
	5 151504.82
	10 302980.53
	50 1515151.71
	100 3030190.9
end
