#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/35/random512-35.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 21:35:39
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - Maps 35 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 53572.99
	5 53572.99
	10 53572.99
	50 53572.99
	100 53572.99
	500 53572.99
	1000 53572.99
	5000 53572.99
	10000 53572.99
end
	1 53775.38
	5 55492.95
	10 55492.95
	50 55492.95
	100 55492.95
	500 55492.95
	1000 55492.95
end
	1 53572.99
	5 267892.35
	10 535471.06
	50 2676302.99
	100 5353136.21
end
