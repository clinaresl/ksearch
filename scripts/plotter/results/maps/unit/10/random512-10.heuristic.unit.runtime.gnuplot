#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/10/random512-10.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 21:34:06
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 10 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.0095319015
	5 0.0091299487
	10 0.0092643903
	50 0.0099173531
	100 0.010432438399999999
	500 0.0134595168
	1000 0.016826354
	5000 0.042469361000000004
	10000 0.07576407900000001
end
	1 0.038943073
	5 1.6595031600000005
	10 1.6566014800000002
	50 1.6741290300000002
	100 1.67907859
	500 1.7537440199999998
	1000 1.8245922300000001
end
	1 0.015921115700000002
	5 0.05150346000000001
	10 0.09579583800000002
	50 0.47699214999999995
	100 1.3410743299999996
end
