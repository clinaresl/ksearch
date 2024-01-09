#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/FLA/USA-road-d.FLA.brute-force.runtime.gnuplot
#
# Started on 01/09/2024 14:32:25
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - FLA Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.FLA.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 1.052499005
	2 1.07232562
	3 1.073183318
	4 1.075740395
	5 1.0741933010000002
	6 1.0755477919999998
	7 1.077760346
	8 1.077262777
	9 1.072080139
	10 1.0690833499999999
	20 1.07320994
	30 1.076477077
	40 1.0801546590000002
	50 1.0845243459999998
	60 1.087486567
	70 1.090177899
	80 1.095308678
	90 1.0991892239999996
	100 1.099625495
	200 1.130593842
	300 1.162628434
	400 1.193370309
	500 1.2212933149999998
	600 1.251000896
	700 1.2805925360000001
	800 1.3097678209999999
	900 1.3374837269999997
	1000 1.363926622
	2000 1.6428748299999998
	3000 1.91404155
	4000 2.1796098600000002
	5000 2.4484783399999994
	6000 2.70514852
	7000 2.9659199999999992
	8000 3.22349088
	9000 3.4863477599999997
	10000 3.7473777500000005
end
	1 1.872609252
	2 1.8935646490000004
	3 1.8950594859999998
	4 1.899756458
	5 1.904882754
	6 1.9076228270000002
	7 1.9089981880000002
	8 1.911784193
	9 1.9126762270000004
	10 1.9041203800000002
	20 1.9087188739999998
	30 1.912711324
	40 1.917657795
	50 1.923169221
	60 1.923366745
	70 1.9277525569999998
	80 1.9437664989999999
	90 1.9333761680000001
	100 1.9285796739999999
	200 1.9530511959999999
	300 1.977905595
	400 2.00197271
	500 2.02752358
	600 2.04866771
	700 2.07237344
	800 2.09534603
	900 2.1203617799999996
	1000 2.13114694
	2000 2.3654134300000003
	3000 2.60448236
	4000 2.8519693199999994
	5000 3.1261405800000004
	6000 3.3627949500000005
	7000 3.6338099100000005
	8000 3.9048573899999997
	9000 4.1743140499999996
	10000 4.434842720000001
end
	1 0.932793318
	2 1.461659412
	3 1.973218475
	4 2.45995009
	5 3.01019425
	6 3.54088284
	7 4.09686353
	8 4.65816658
	9 5.33611988
	10 5.90964507
end
