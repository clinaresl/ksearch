#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/unit/10/10pancake.brute-force.runtime.gnuplot
#
# Started on 04/09/2024 14:20:53
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 10-Pancake unit"

set terminal png enhanced font "Ariel,10"
set output '10pancake.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 17.2876529
	2 19.422571700000002
	3 20.6793217
	4 21.039231500000007
	5 21.66468830000001
	6 22.3144325
	7 22.5525455
	8 22.8830095
	9 23.054267199999998
	10 23.1681383
end
	1 60.37144700000001
	2 82.08114499999999
	3 87.24793100000001
	4 89.10279900000002
	5 89.83587200000002
end
	1 12.3814568
	2 28.8679621
	3 45.64931299999999
	4 60.13520999999999
	5 76.01700199999999
end
