#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.heuristic.octile.runtime.gnuplot
#
# Started on 04/02/2024 21:34:32
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 15 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.heuristic.octile.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.037188117189999995
	5 0.0385934693
	10 0.0384692768
	50 0.039375507500000004
	100 0.039427328
	500 0.04309319139999999
	1000 0.0466847138
	5000 0.08240285899999998
	10000 0.12716694399999998
end
	1 0.0702494
	5 0.11778426
	10 0.12360826600000001
	50 0.13312341300000002
	100 0.14000392099999998
	500 0.18205813000000004
	1000 0.23658361
end
	1 0.03126911791
	5 0.14262357099
	10 0.28332371225999997
	50 1.6690255758
	100 4.6697425351
end
