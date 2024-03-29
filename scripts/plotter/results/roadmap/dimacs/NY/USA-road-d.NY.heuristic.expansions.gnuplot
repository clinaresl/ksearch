#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/NY/USA-road-d.NY.heuristic.expansions.gnuplot
#
# Started on 01/09/2024 14:40:12
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - NY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NY.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 191475.0
	2 191476.51
	3 191477.83
	4 191478.49
	5 191480.07
	6 191480.45
	7 191481.28
	8 191481.86
	9 191483.18
	10 191483.51
	20 191487.52
	30 191490.37
	40 191492.65
	50 191494.4
	60 191495.65
	70 191497.01
	80 191498.42
	90 191499.48
	100 191500.49
	200 191507.03
	300 191511.22
	400 191514.2
	500 191516.71
	600 191518.61
	700 191520.69
	800 191522.2
	900 191523.69
	1000 191524.84
	2000 191533.32
	3000 191538.64
	4000 191542.16
	5000 191545.3
	6000 191547.66
	7000 191549.73
	8000 191551.54
	9000 191553.37
	10000 191554.93
end
	1 191477.63
	2 192057.48
	3 192354.9
	4 192408.23
	5 192492.87
	6 192514.18
	7 192555.7
	8 192637.29
	9 192745.16
	10 192751.91
	20 193055.58
	30 193093.95
	40 193207.0
	50 193319.87
	60 193483.2
	70 193503.26
	80 193539.55
	90 193591.8
	100 193598.08
	200 193688.39
	300 193804.41
	400 193890.82
	500 193921.88
	600 193930.54
	700 193950.5
	800 193952.04
	900 193975.22
	1000 194004.06
	2000 194094.18
	3000 194121.02
	4000 194245.98
	5000 194336.39
	6000 194354.65
	7000 194366.23
	8000 194443.18
	9000 194488.05
	10000 194531.88
end
	1 191475.0
	2 382950.82
	3 574428.6
	4 765905.48
	5 957385.03
	6 1148861.06
	7 1340342.38
	8 1531816.71
	9 1723301.56
	10 1914776.61
end