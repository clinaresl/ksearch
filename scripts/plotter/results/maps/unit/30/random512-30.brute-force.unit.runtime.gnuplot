#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/30/random512-30.brute-force.unit.runtime.gnuplot
#
# Started on 04/07/2024 22:01:56
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
	1 0.46341934
	2 0.46044032
	3 0.46303995
	4 0.46428221000000014
	5 0.46525980999999994
	6 0.46589189999999997
	7 0.46683809
	8 0.46810332999999993
	9 0.46893408999999997
	10 0.4661563
	20 0.46917126000000003
	30 0.4706937599999999
	40 0.47328258000000006
	50 0.47584109000000013
	60 0.47885369
	70 0.48226026
	80 0.48485366999999996
	90 0.48722417000000007
	100 0.4864893500000001
	200 0.51187033
	300 0.54046931
	400 0.5597981200000001
	500 0.5796502
	600 0.60170994
	700 0.6210283299999999
	800 0.64260802
	900 0.66393929
	1000 0.68183984
	2000 0.91682432
	3000 1.1832325
	4000 1.4763070000000003
	5000 1.800319
	6000 2.1492349999999996
	7000 2.5243557
	8000 2.9352795999999994
	9000 3.3796531
	10000 3.846836299999999
end
	1 0.1950899
	2 0.32812942
	3 0.46123662000000004
	4 0.5874936000000001
	5 0.7238271299999998
	6 0.8618403099999999
	7 1.00121459
	8 1.1446349
	9 1.2639623
	10 1.3833308
	20 2.8469892
	30 4.533121500000001
	40 6.614540600000001
	50 8.7073319
	60 10.944664
	70 15.888609
	80 19.131851
	90 22.46444
	100 25.91492
end
