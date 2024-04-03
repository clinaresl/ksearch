#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/20/random512-20.heuristic.octile.expansions.gnuplot
#
# Started on 04/02/2024 21:34:55
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - Maps 20 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.octile.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 17294.59
	5 17294.59
	10 17294.59
	50 17294.59
	100 17294.59
	500 17294.59
	1000 17294.59
	5000 17294.59
	10000 17294.59
end
	1 17348.07
	5 19556.95
	10 20176.09
	50 20868.4
	100 21210.73
	500 21396.82
	1000 21418.56
end
	1 17294.59
	5 86273.1
	10 172467.35
	50 862191.12
	100 1724099.18
end
