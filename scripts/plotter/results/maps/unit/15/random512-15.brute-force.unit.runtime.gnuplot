#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/15/random512-15.brute-force.unit.runtime.gnuplot
#
# Started on 04/03/2024 23:08:15
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 15 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.brute-force.unit.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.27772199000000003
	2 0.27788053999999995
	3 0.27764126
	4 0.27855817
	5 0.2791477
	6 0.28062104
	7 0.28034943000000007
	8 0.28315348
	9 0.28361059
	10 0.27803684
	20 0.27793804
	30 0.27975562
	40 0.28042301000000003
	50 0.2816202400000001
	60 0.28269829
	70 0.28443049
	80 0.28573394
	90 0.28663813000000005
	100 0.2797409
	200 0.28199697
	300 0.28521931
	400 0.28653157
	500 0.28845219000000005
	600 0.2919910599999999
	700 0.29297026000000004
	800 0.29420935
	900 0.29474594
	1000 0.28964575000000004
	2000 0.31748179
	3000 0.3364412599999999
	4000 0.35209458999999993
	5000 0.37128702000000013
	6000 0.39041355999999994
	7000 0.41222266999999996
	8000 0.43530280000000005
	9000 0.4447911500000001
	10000 0.46356741999999995
end
	1 0.54248274
	2 0.5366017099999998
	3 0.53632229
	4 0.53636642
	5 0.53965103
	6 0.5458321199999999
	7 0.5479315900000001
	8 0.5451250099999999
	9 0.54722957
	10 0.54806051
	20 0.54392979
	30 0.5499222500000002
	40 0.55548663
	50 0.55720952
	60 0.55838529
	70 0.56007687
	80 0.5625860699999999
	90 0.5651231699999999
	100 0.5568688900000001
	200 0.56801029
	300 0.5917125299999999
	400 0.62336958
	500 0.6489743
	600 0.6710551699999999
	700 0.6939359199999998
	800 0.70831093
	900 0.7253805100000001
	1000 0.73099458
end
	1 0.24811473
	2 0.44348421999999993
	3 0.63392926
	4 0.8145568300000001
end
