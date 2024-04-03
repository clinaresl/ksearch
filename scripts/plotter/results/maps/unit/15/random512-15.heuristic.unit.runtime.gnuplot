#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/15/random512-15.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 21:34:23
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 15 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.0203021844
	5 0.0205624982
	10 0.020957695899999997
	50 0.022105594000000006
	100 0.023198879000000002
	500 0.027099215999999995
	1000 0.028084313999999996
	5000 0.063325761
	10000 0.11338487000000001
end
	1 0.061034977000000004
	5 1.66645895
	10 1.6607732899999998
	50 1.68303807
	100 1.69133537
	500 1.79538414
	1000 1.9068660999999998
end
	1 0.0203850105
	5 0.070987274
	10 0.131843437
	50 0.66474287
	100 1.89992778
end
