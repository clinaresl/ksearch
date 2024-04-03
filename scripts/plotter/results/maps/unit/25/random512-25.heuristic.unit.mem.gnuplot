#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/25/random512-25.heuristic.unit.mem.gnuplot
#
# Started on 04/02/2024 21:35:01
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - Maps 25 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.unit.mem.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 118.13
	5 123.0
	10 127.32
	50 155.0
	100 135.6
	500 135.98
	1000 145.41
	5000 189.35
	10000 279.62
end
	1 200.61
	5 222.65
	10 230.65
	50 239.95
	100 258.6
	500 343.96
	1000 384.35
end
	1 73.7
	5 160.71
	10 234.71
	50 590.51
	100 1155.08
end
