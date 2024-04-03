#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/35/random512-35.heuristic.unit.runtime.gnuplot
#
# Started on 04/02/2024 21:35:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 35 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.heuristic.unit.runtime.png'

plot "-" title "BELA$^*$"      with linesp, "-" title "K$^*$"      with linesp, "-" title "mA$^*$"      with linesp

	1 0.061565190000000006
	5 0.062539225
	10 0.062920008
	50 0.06335383400000001
	100 0.06388425700000001
	500 0.067554423
	1000 0.07180448300000002
	5000 0.120848675
	10000 0.19244891000000003
end
	1 0.14375271
	5 0.153194805
	10 0.15353170400000002
	50 0.16316928399999997
	100 0.17172341000000002
	500 0.2563688
	1000 0.3609423400000001
end
	1 0.037304870999999996
	5 0.15560015800000002
	10 0.3119102
	50 1.7358471200000003
	100 4.7836565
end
