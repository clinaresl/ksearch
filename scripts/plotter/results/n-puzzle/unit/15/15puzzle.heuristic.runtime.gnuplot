#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/unit/15/15puzzle.heuristic.runtime.gnuplot
#
# Started on 12/09/2023 19:56:33
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 15-Puzzle unit"

set terminal png enhanced font "Ariel,10"
set output '15puzzle.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 2.3640134925
	2 2.73905353
	3 3.2616631575
	4 3.6932876625000004
	5 3.695340365
	6 3.7554621249999998
	7 3.86580615
	8 3.9563159
	9 4.051885400000001
	10 4.029640725
	20 4.99187685
	30 5.2447418
	40 5.422654199999999
	50 5.6843445
	60 5.723851375000001
	70 5.898584725000001
	80 6.016919175
	90 5.9991939
	100 6.048060874999999
end
	1 9.6606839
	2 19.89836225
	3 24.53756975
end
	1 2.296950225
	2 4.2380618875
	3 6.17817765
	4 8.470600725
	5 9.577144275
end
