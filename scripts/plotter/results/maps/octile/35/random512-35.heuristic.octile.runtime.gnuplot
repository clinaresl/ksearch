#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/35/random512-35.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 19:57:25
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.066215305
	5 0.067395067
	10 0.067367628
	50 0.068609628
	100 0.06894288
	500 0.072364621
	1000 0.073862389
	5000 0.11951838399999999
	10000 0.16901075
end
	1 0.10340054999999998
	5 0.11297045200000001
	10 0.11233775099999999
	50 0.12279275
	100 0.13103637999999998
	500 0.15843856
	1000 0.20129771999999996
end
	1 0.05886678699999999
	5 0.25970999999999994
	10 0.53951456
	50 3.1707999000000004
	100 9.089210000000001
end
