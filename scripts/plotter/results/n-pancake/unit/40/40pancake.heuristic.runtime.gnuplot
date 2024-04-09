#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/40/40pancake.heuristic.runtime.gnuplot
#
# Started on 04/09/2024 14:21:22
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 40-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '40pancake.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 0.60928064618
	2 0.9234553559000002
	3 1.1900919060000001
	4 1.483391623
	5 1.7180125910000001
	6 1.9630990369999999
	7 2.18526997
	8 2.40397046
	9 2.70770717
	10 2.8903464899999993
	20 4.49858895
	30 5.734847450000001
	40 7.1624317
end
	1 6.1216742790000005
end
	1 0.5953269184300001
	2 0.9242559176000001
	3 1.274464002
	4 1.6417047889999998
	5 1.900287385
	6 2.1404801959999995
	7 2.4090902229999993
	8 2.72126113
	9 3.06107093
	10 3.1945773399999995
	20 5.3009398899999995
end
