#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/10/random512-10.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 19:55:55
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.010201574080000003
	5 0.010523871999999998
	10 0.011127146950000001
	50 0.011161612900000001
	100 0.011568151400000002
	500 0.0141655519
	1000 0.017293110299999997
	5000 0.040400897000000005
	10000 0.06971650700000001
end
	1 0.036012199
	5 0.19167723599999997
	10 0.198682301
	50 0.20897294400000002
	100 0.21296253500000004
	500 0.24349660000000004
	1000 0.28189185
end
	1 0.0175049452
	5 0.07175557192
	10 0.13849216809
	50 0.7673659076
	100 2.1582880803
end
