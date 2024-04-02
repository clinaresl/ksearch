#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/20/random512-20.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 19:56:30
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-20.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.045372276999999996
	5 0.045958541000000006
	10 0.04665543100000002
	50 0.04774936800000001
	100 0.048002046
	500 0.050713206
	1000 0.052832369000000004
	5000 0.09583833200000001
	10000 0.13159605900000002
end
	1 0.08658212600000001
	5 0.10830728800000002
	10 0.11272949200000001
	50 0.12171498099999999
	100 0.129119591
	500 0.1719131
	1000 0.22980681
end
	1 0.039027403
	5 0.18109500999999997
	10 0.36921729999999997
	50 2.1781281000000003
	100 5.975811400000001
end
