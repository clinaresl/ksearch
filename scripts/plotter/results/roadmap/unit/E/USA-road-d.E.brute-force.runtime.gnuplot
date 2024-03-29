#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/E/USA-road-d.E.brute-force.runtime.gnuplot
#
# Started on 01/09/2024 14:28:36
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - E Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.E.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 2.153203963
	2 2.1634684330000002
	3 2.168552948
	4 2.170417816
	5 2.175447456
	6 2.1750113410000003
	7 2.1781011820000002
	8 2.181263086
	9 2.1816909719999997
	10 2.169599489
	20 2.203617577
	30 2.211049111
	40 2.21345186
	50 2.2174789279999993
	60 2.218875471
	70 2.220851067
	80 2.224637806
	90 2.2271823729999998
	100 2.217912874
	200 2.2239486829999997
	300 2.228946345
	400 2.235679304
	500 2.240412479
	600 2.247205725
	700 2.249927368
	800 2.259614848
	900 2.26393354
	1000 2.25807197
	2000 2.29807533
	3000 2.33923549
	4000 2.38349068
	5000 2.42775316
	6000 2.4708236
	7000 2.5129895099999997
	8000 2.55763982
	9000 2.5999727200000002
	10000 2.6458332799999997
end
	1 4.793039950000001
	2 4.7934461200000005
	3 4.80649544
	4 4.80940461
	5 4.81464622
	6 4.81646434
	7 4.82392522
	8 4.8269676100000005
	9 4.828908340000001
	10 4.807821130000001
	20 4.81712417
	30 4.824621560000001
	40 4.83227923
	50 4.84190804
	60 4.84821766
	70 4.85464303
	80 4.85791148
	90 4.86478868
	100 4.843759520000001
	200 4.87751963
	300 4.90292552
	400 4.9429326
	500 4.96112223
	600 4.9923798
	700 5.020764649999999
	800 5.070001800000001
	900 5.0823445199999995
	1000 5.07488777
	2000 5.35751475
	3000 5.6535587199999995
	4000 5.964052860000001
	5000 6.279152870000001
	6000 6.592025430000002
	7000 6.91073355
	8000 7.232239470000001
	9000 7.5508355400000005
	10000 7.871160480000001
end
	1 2.05452404
	2 3.40680593
	3 4.964902479999999
	4 6.286253330000001
	5 8.00005546
	6 9.39031305
	7 10.952789829999999
	8 12.376995679999999
	9 14.323060120000001
	10 15.854185710000001
end