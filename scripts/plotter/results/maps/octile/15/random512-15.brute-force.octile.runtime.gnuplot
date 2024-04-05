#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.brute-force.octile.runtime.gnuplot
#
# Started on 04/05/2024 10:26:10
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 15 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-15.brute-force.octile.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.5864963799999999
	2 0.58701636
	3 0.5881482100000001
	4 0.58850113
	5 0.58928891
	6 0.58929407
	7 0.5892349400000001
	8 0.58915427
	9 0.59008292
	10 0.58814052
	20 0.58779108
	30 0.58941305
	40 0.589043
	50 0.5896374799999999
	60 0.59032748
	70 0.5905780500000001
	80 0.59138128
	90 0.59291079
	100 0.59012833
	200 0.59152857
	300 0.5926701499999999
	400 0.5945853900000002
	500 0.5948068000000001
	600 0.59679441
	700 0.5972196599999999
	800 0.5977441099999998
	900 0.59957041
	1000 0.59589397
	2000 0.60505826
	3000 0.6146187399999999
	4000 0.62484781
	5000 0.6625558899999998
	6000 0.6800857599999999
	7000 0.6940573600000002
	8000 0.6854510699999998
	9000 0.6893575700000001
	10000 0.7180439899999999
end
	1 1.2525202
	2 1.2492269
	3 1.2538252
	4 1.2537868
	5 1.2555132
	6 1.2565279999999999
	7 1.2578163
	8 1.2590286
	9 1.2596658000000003
	10 1.2545908
	20 1.2566847
	30 1.2588382
	40 1.2608337
	50 1.2628905
	60 1.2645496
	70 1.2681693
	80 1.2709073
	90 1.2714804000000002
	100 1.2642930000000001
	200 1.2806817999999998
	300 1.2944616
	400 1.3064578999999998
	500 1.3174801000000003
	600 1.3302798999999998
	700 1.3434163000000001
	800 1.3544841999999997
	900 1.3660406000000003
	1000 1.3712978999999998
	2000 1.5167769999999998
	3000 1.6867566999999999
	4000 1.8829088
	5000 2.098333
	6000 2.3306626
	7000 2.5733488999999996
	8000 2.826508299999999
	9000 3.0816278000000006
	10000 3.3370638
end
	1 0.4652511
	2 0.85461529
	3 1.2130247
	4 1.5825305
	5 2.0133034
	6 2.4590549
	7 2.937078900000001
	8 3.4465281000000005
	9 3.8361277
	10 4.3218146
	20 9.5233646
	30 15.058637
	40 21.065682000000006
	50 27.502796
	60 34.160647
	70 51.155171999999986
	80 61.14207799999999
	90 72.120945
	100 83.26080900000002
end
