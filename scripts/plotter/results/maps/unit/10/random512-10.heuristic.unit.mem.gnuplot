#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/10/random512-10.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 21:34:08
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 10 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 42.36
	5 245.08
	10 289.08
	50 294.0
	100 294.0
	500 294.0
	1000 294.0
	5000 315.84
	10000 377.06
end
	1 118.47
	5 381.45
	10 398.0
	50 404.89
	100 422.0
	500 494.7
	1000 532.11
end
	1 24.51
	5 229.36
	10 329.05
	50 1368.48
	100 2653.92
end
