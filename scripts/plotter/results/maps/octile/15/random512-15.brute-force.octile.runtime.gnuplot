#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/15/random512-15.brute-force.octile.runtime.gnuplot
#
# Started on 12/06/2023 14:51:33
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
	1 1.2972015
	2 1.2948861999999999
	3 1.298045
	4 1.2973465
	5 1.3010566
	6 1.3003132
	7 1.3022611
	8 1.3025266
	9 1.3063820000000004
	10 1.2980950000000002
	20 1.3005119000000003
	30 1.3037760000000003
	40 1.3071366999999998
	50 1.3061676
	60 1.3094045
	70 1.3125856000000002
	80 1.3144457
	90 1.3169452
	100 1.3108867000000002
	200 1.3239960000000002
	300 1.3377344
	400 1.3512877999999997
	500 1.3610003
	600 1.3752852999999998
	700 1.3841649999999999
	800 1.4024003999999999
	900 1.4145634
	1000 1.427426
end
	1 0.3620200900000001
	2 0.6564682
	3 0.9330766300000001
	4 1.2142398
end
