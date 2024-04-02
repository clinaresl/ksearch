#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/25/random512-25.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 19:56:49
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set terminal png enhanced font "Ariel,10"
set output 'random512-25.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.053777280999999996
	5 0.055135885
	10 0.054877935999999995
	50 0.05622175500000001
	100 0.05649077000000001
	500 0.059673409999999996
	1000 0.06324304900000001
	5000 0.09731808599999997
	10000 0.14469718
end
	1 0.08006523699999998
	5 0.09476354899999999
	10 0.096225664
	50 0.105360476
	100 0.11249127300000002
	500 0.14271786999999997
	1000 0.17990062
end
	1 0.04680158100000001
	5 0.21903038000000002
	10 0.43770139
	50 2.6336962000000006
	100 7.339361299999999
end
