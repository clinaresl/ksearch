#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/BAY/USA-road-d.BAY.heuristic.expansions.gnuplot
#
# Started on 01/09/2024 14:35:49
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "expansions"

set title "#Expansions - BAY Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.BAY.heuristic.expansions.png'

plot "-" title "BELA*"      with linesp, "-" title "mA*"      with linesp

	1 194967.9
	2 194970.12
	3 194972.38
	4 194973.51
	5 194975.37
	6 194976.08
	7 194977.45
	8 194978.15
	9 194979.62
	10 194980.22
	20 194985.47
	30 194989.58
	40 194992.62
	50 194994.91
	60 194996.76
	70 194998.06
	80 194999.54
	90 195000.82
	100 195001.82
	200 195009.05
	300 195013.59
	400 195017.19
	500 195019.96
	600 195022.46
	700 195024.17
	800 195025.85
	900 195027.6
	1000 195028.84
	2000 195037.7
	3000 195043.42
	4000 195047.57
	5000 195050.65
	6000 195053.19
	7000 195055.48
	8000 195057.55
	9000 195059.24
	10000 195060.48
end
	1 194967.9
	2 389939.2
	3 584911.24
	4 779882.48
	5 974856.67
	6 1169819.08
	7 1364793.22
	8 1559762.99
	9 1754739.27
	10 1949710.03
end
