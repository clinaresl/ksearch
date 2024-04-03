#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/25/random512-25.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 21:34:59
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 25 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.0385829295
	5 0.039431271500000004
	10 0.039428469699999996
	50 0.039730405
	100 0.04109482099999999
	500 0.04575308799999999
	1000 0.047223013999999994
	5000 0.08664703199999998
	10000 0.155431974
end
	1 0.09287490200000001
	5 1.09139031
	10 1.08871796
	50 1.11021171
	100 1.11684191
	500 1.2142021699999999
	1000 1.32489966
end
	1 0.02309770119999999
	5 0.09582658200000001
	10 0.186544447
	50 0.9969203589999998
	100 2.7052826299999997
end
