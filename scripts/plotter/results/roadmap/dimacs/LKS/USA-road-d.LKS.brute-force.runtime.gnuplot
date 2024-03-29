#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/LKS/USA-road-d.LKS.brute-force.runtime.gnuplot
#
# Started on 01/09/2024 14:32:52
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - LKS Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.LKS.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 2.2661724899999998
	2 2.32629468
	3 2.33893047
	4 2.34289453
	5 2.34558535
	6 2.33766813
	7 2.33056164
	8 2.3490396
	9 2.3541325800000004
	10 2.3335129400000003
	20 2.34947307
	30 2.3612699800000003
	40 2.36944465
	50 2.3769702600000002
	60 2.3821074700000002
	70 2.38902074
	80 2.39424372
	90 2.39790451
	100 2.39737215
	200 2.4712906199999995
	300 2.49415954
	400 2.5451460100000003
	500 2.5906120300000004
	600 2.63424851
	700 2.6805085099999997
	800 2.7256871600000006
	900 2.7686157600000003
	1000 2.804349780000001
	2000 3.2230778300000007
	3000 3.61620827
	4000 4.017057219999999
	5000 4.4086156899999995
	6000 4.79431463
	7000 5.150987659999999
	8000 5.539832119999999
	9000 5.923110690000001
	10000 6.327689400000001
end
	1 4.1443552299999995
	2 4.17412515
	3 4.1803254
	4 4.20845363
	5 4.200062689999999
	6 4.2019189699999995
	7 4.2026069900000005
	8 4.212146290000001
	9 4.21288096
	10 4.1970841100000005
	20 4.21919583
	30 4.25804922
	40 4.26585549
	50 4.27361548
	60 4.27636898
	70 4.288147090000001
	80 4.296147039999999
	90 4.301396319999999
	100 4.28291813
	200 4.33411905
	300 4.381594840000001
	400 4.433453879999999
	500 4.48502994
	600 4.55704072
	700 4.607172050000001
	800 4.654904650000001
	900 4.7004978799999995
	1000 4.708927930000001
	2000 5.17778603
	3000 5.6377893100000005
	4000 6.13083305
	5000 6.65125222
	6000 7.19761301
	7000 7.739107229999999
	8000 8.2080786
	9000 8.73762395
	10000 9.291175590000002
end
	1 1.9950925759999998
	2 3.0790997899999994
	3 4.18481593
	4 5.19644996
	5 6.418193019999999
	6 7.554996760000002
	7 8.777123360000001
	8 9.986837699999999
	9 11.419725070000002
	10 12.708664899999999
end