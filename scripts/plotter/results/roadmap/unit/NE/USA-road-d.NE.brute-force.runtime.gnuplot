#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/NE/USA-road-d.NE.brute-force.runtime.gnuplot
#
# Started on 01/09/2024 14:29:21
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - NE Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NE.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.9666545290000002
	2 0.9871813849999999
	3 0.9925787740000002
	4 0.9943346359999999
	5 0.994572049
	6 0.996064626
	7 0.9981134479999999
	8 0.9982925090000002
	9 1.000401339
	10 0.9928641180000001
	20 0.9932255529999999
	30 0.9953127279999999
	40 0.99652085
	50 0.9978512090000001
	60 1.000171615
	70 1.000369022
	80 1.00217415
	90 1.0045860889999998
	100 0.9991998250000002
	200 1.0028495200000003
	300 1.0058507499999998
	400 1.011094536
	500 1.0140590109999998
	600 1.01674639
	700 1.0196381900000002
	800 1.02415425
	900 1.02727081
	1000 1.02230395
	2000 1.04762637
	3000 1.0742990099999998
	4000 1.09531155
	5000 1.12890908
	6000 1.1582435199999999
	7000 1.18659447
	8000 1.21411018
	9000 1.24039028
	10000 1.2726920099999999
end
	1 2.17945943
	2 2.1840055799999996
	3 2.18553676
	4 2.1892804299999997
	5 2.1915403400000004
	6 2.1970635
	7 2.20005231
	8 2.19664545
	9 2.20140616
	10 2.19388153
	20 2.1955698499999996
	30 2.2039079800000003
	40 2.20301572
	50 2.20999492
	60 2.2079250399999997
	70 2.21280703
	80 2.2167067199999995
	90 2.2194578500000004
	100 2.21098128
	200 2.22604633
	300 2.2431989400000005
	400 2.2671936400000003
	500 2.2881368699999998
	600 2.30140479
	700 2.32077664
	800 2.33925544
	900 2.3572171099999997
	1000 2.3617239999999997
	2000 2.5307399999999998
	3000 2.7143058300000003
	4000 2.9002355900000003
	5000 3.0858710599999997
	6000 3.2714005900000007
	7000 3.4593012699999997
	8000 3.65021728
	9000 3.83638072
	10000 4.02454977
end
	1 0.8630585080000001
	2 1.4277585799999999
	3 2.0643519099999996
	4 2.62385208
	5 3.3382863099999995
	6 3.94396454
	7 4.608582559999999
	8 5.22994088
	9 6.09428055
	10 6.764823700000001
end