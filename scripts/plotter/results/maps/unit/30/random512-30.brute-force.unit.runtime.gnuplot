#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.brute-force.unit.runtime.gnuplot
#
# Started on 12/06/2023 14:52:31
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 30 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-30.brute-force.unit.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.21746947999999997
	2 0.21732170999999997
	3 0.21719399
	4 0.21822868000000004
	5 0.21809612
	6 0.21869645
	7 0.21938176999999995
	8 0.22135589999999997
	9 0.22225748
	10 0.21775358
	20 0.21806441
	30 0.21824825000000003
	40 0.21904394000000005
	50 0.22038308
	60 0.22129198
	70 0.22197855
	80 0.22317213
	90 0.22540020999999996
	100 0.22014404000000007
	200 0.22102376999999998
	300 0.223407
	400 0.22663312999999996
	500 0.22723849999999998
	600 0.22953192000000006
	700 0.23245107
	800 0.23213251000000004
	900 0.23343528000000002
	1000 0.23006984
	2000 0.25012381
	3000 0.27274716
	4000 0.28916328999999996
	5000 0.30615739000000003
	6000 0.32517575
	7000 0.34181981
	8000 0.35314028000000003
	9000 0.37035243
	10000 0.38834362999999994
end
	1 0.4671820099999999
	2 0.46458508000000004
	3 0.46702558000000005
	4 0.4685495599999999
	5 0.46944581999999996
	6 0.47090307000000003
	7 0.47197577
	8 0.4721008200000001
	9 0.47356617
	10 0.47078308999999996
	20 0.47266446
	30 0.47522043999999997
	40 0.47752665000000005
	50 0.48074757000000007
	60 0.48302707999999994
	70 0.4855854200000001
	80 0.48815023999999996
	90 0.49084367
	100 0.49052397999999997
	200 0.5155959800000001
	300 0.5445979200000001
	400 0.56369909
	500 0.58406285
	600 0.6049656200000001
	700 0.6255456000000001
	800 0.6469218600000001
	900 0.66852621
	1000 0.69032335
end
	1 0.1950899
	2 0.32812942
	3 0.46123662000000004
	4 0.5874936000000001
end
