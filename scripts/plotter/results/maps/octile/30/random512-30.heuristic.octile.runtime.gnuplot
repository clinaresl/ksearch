#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/30/random512-30.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 19:57:07
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.040400145
	5 0.041070046
	10 0.04137597700000001
	50 0.04218218799999999
	100 0.042180489999999994
	500 0.04379183200000001
	1000 0.046599666000000005
	5000 0.07330689200000001
	10000 0.103078877
end
	1 0.09004087200000001
	5 0.10115912199999999
	10 0.103129787
	50 0.11048676
	100 0.11707368000000001
	500 0.14847238000000001
	1000 0.19469838
end
	1 0.053974096
	5 0.25377251
	10 0.5005661600000001
	50 2.997881600000001
	100 8.3810109
end
