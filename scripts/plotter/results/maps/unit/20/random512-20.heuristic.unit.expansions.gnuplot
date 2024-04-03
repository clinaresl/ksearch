#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/20/random512-20.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 21:34:46
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - Maps 20 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 27484.18
	5 27487.69
	10 27490.14
	50 27497.1
	100 27500.14
	500 27508.68
	1000 27512.94
	5000 27523.89
	10000 27528.33
end
	1 26976.17
	5 109026.82
	10 109026.82
	50 109026.82
	100 109026.82
	500 109026.82
	1000 109026.82
end
	1 27484.18
	5 135533.55
	10 269489.7
	50 1334538.66
	100 2660764.85
end
