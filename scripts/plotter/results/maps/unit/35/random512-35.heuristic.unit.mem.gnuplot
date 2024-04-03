#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/35/random512-35.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 21:35:37
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 35 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 112.43
	5 127.0
	10 140.0
	50 140.0
	100 142.57
	500 155.19
	1000 157.46
	5000 280.2
	10000 420.67
end
	1 165.43
	5 186.68
	10 195.36
	50 205.04
	100 219.04
	500 289.76
	1000 335.12
end
	1 75.27
	5 152.99
	10 304.64
	50 574.51
	100 970.9
end
