#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/CAL/USA-road-d.CAL.heuristic.mem.gnuplot
#
# Started on 01/09/2024 14:36:11
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "mem"

set title "Memory (Mbytes) - CAL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.CAL.heuristic.mem.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 2253.83
	2 2329.8
	3 2365.86
	4 2380.34
	5 2404.88
	6 2410.8
	7 2410.8
	8 2410.8
	9 2400.36
	10 2413.8
	20 2434.7
	30 2421.8
	40 2421.8
	50 2421.8
	60 2417.04
	70 2405.66
	80 2407.23
	90 2410.09
	100 2408.22
	200 2405.37
	300 2403.66
	400 2403.89
	500 2404.62
	600 2412.04
	700 2412.27
	800 2412.55
	900 2413.13
	1000 2407.33
	2000 2442.33
	3000 2495.63
	4000 2514.04
	5000 2519.25
	6000 2644.48
	7000 2753.75
	8000 2721.18
	9000 2741.43
	10000 2745.31
end
	1 3769.24
	2 4079.48
	3 4082.08
	4 4084.08
	5 4122.0
	6 4149.56
	7 4151.56
	8 4153.56
	9 4155.96
	10 4158.56
	20 4171.36
	30 4182.12
	40 4186.72
	50 4191.12
	60 4196.12
	70 4205.63
	80 4270.2
	90 4289.12
	100 4201.79
	200 4230.79
	300 4259.16
	400 4297.79
	500 4333.42
	600 4326.75
	700 4341.91
	800 4340.27
	900 4339.16
	1000 4406.0
	2000 4553.51
	3000 4699.8
	4000 4906.79
	5000 5117.49
	6000 5326.14
	7000 5528.95
	8000 5748.99
	9000 5939.06
	10000 6100.91
end
	1 1940.62
	2 2613.33
	3 3630.33
	4 4621.38
	5 5619.87
	6 6215.2
	7 7063.26
	8 7845.46
	9 9029.25
	10 9820.07
end