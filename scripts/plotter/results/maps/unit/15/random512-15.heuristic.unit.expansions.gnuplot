#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/15/random512-15.heuristic.unit.expansions.gnuplot
#
# Started on 04/02/2024 19:56:08
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.unit.expansions.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 15438.31
	5 15441.97
	10 15444.75
	50 15452.09
	100 15455.67
	500 15464.94
	1000 15469.29
	5000 15480.34
	10000 15485.72
end
	1 15831.51
	5 142281.66
	10 142281.66
	50 142281.66
	100 142281.66
	500 142281.66
	1000 142281.66
end
	1 15438.31
	5 73454.92
	10 145179.58
	50 712873.4
	100 1417279.15
end
