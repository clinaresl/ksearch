#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NY/USA-road-d.NY.brute-force.expansions.gnuplot
#
# Started on 01/09/2024 14:34:30
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - NY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.brute-force.expansions.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 201040.29
	2 201042.0
	3 201043.13
	4 201043.78
	5 201045.28
	6 201045.85
	7 201046.67
	8 201047.22
	9 201048.65
	10 201048.88
	20 201052.58
	30 201055.07
	40 201057.05
	50 201058.83
	60 201060.1
	70 201061.12
	80 201062.14
	90 201063.23
	100 201064.19
	200 201070.25
	300 201074.12
	400 201076.8
	500 201078.84
	600 201080.53
	700 201082.37
	800 201083.54
	900 201084.94
	1000 201086.06
	2000 201093.83
	3000 201098.35
	4000 201101.57
	5000 201104.2
	6000 201106.29
	7000 201108.19
	8000 201109.79
	9000 201111.11
	10000 201112.45
end
	1 201043.0
	2 201574.97
	3 201843.97
	4 201894.49
	5 201967.77
	6 201990.67
	7 202023.3
	8 202092.88
	9 202191.54
	10 202197.63
	20 202502.07
	30 202536.33
	40 202629.06
	50 202722.64
	60 202898.03
	70 202916.38
	80 202940.52
	90 202999.93
	100 203005.45
	200 203101.44
	300 203211.94
	400 203285.35
	500 203312.15
	600 203326.49
	700 203336.79
	800 203338.37
	900 203362.95
	1000 203397.32
	2000 203458.87
	3000 203499.15
	4000 203595.55
	5000 203685.46
	6000 203715.65
	7000 203728.28
	8000 203804.8
	9000 203843.07
	10000 203880.66
end
	1 201040.29
	2 402081.9
	3 603123.44
	4 804164.29
	5 1005208.29
	6 1206249.3
	7 1407292.15
	8 1608334.21
	9 1809381.92
	10 2010421.96
end