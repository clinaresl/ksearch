#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/40/40pancake.heuristic.expansions.gnuplot
#
# Started on 12/07/2023 15:24:58
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - 40-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '40pancake.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 60967.15
	2 92890.58
	3 127932.52
	4 157109.89
	5 178490.21
	6 200582.2
	7 221284.28
	8 242248.91
	9 261946.2
	10 278710.47
	20 422505.16
	30 562455.59
	40 702102.51
end
	1 88682.66
end
	1 60967.15
	2 94289.65
	3 136928.29
	4 174598.55
	5 202870.34
	6 226989.84
	7 251935.89
	8 281294.34
	9 307123.26
	10 328252.54
	20 525144.21
end
