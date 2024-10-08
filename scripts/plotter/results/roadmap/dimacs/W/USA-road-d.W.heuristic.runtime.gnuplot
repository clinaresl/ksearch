#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/W/USA-road-d.W.heuristic.runtime.gnuplot
#
# Started on 01/09/2024 14:40:26
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - W Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.W.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp

	1 4.85966322
	2 4.925626779999999
	3 4.9341151000000005
	4 4.95969899
	5 4.9636596200000005
	6 4.95727982
	7 5.008615550000001
	8 4.9893243
	9 5.011933439999998
	10 5.005324869999999
	20 5.02913447
	30 5.05732376
	40 5.012321560000001
	50 5.05059093
	60 5.040488109999999
	70 5.04490546
	80 5.05928207
	90 5.05443105
	100 5.041790710000001
	200 5.10877528
	300 5.212858729999999
	400 5.28241559
	500 5.3489472000000005
	600 5.40887294
	700 5.48849553
	800 5.58859299
	900 5.6504354
	1000 5.71759327
	2000 6.348195130000001
	3000 7.00034848
	4000 7.583100050000001
	5000 8.213638699999999
	6000 8.8463379
	7000 9.406979600000001
	8000 10.0101536
	9000 10.5568906
	10000 11.1590568
end
	1 8.77778113
	2 8.87332347
	3 8.871403260000001
	4 8.9299694
	5 9.009804950000001
	6 9.04315364
	7 9.031351500000001
	8 9.01389442
	9 9.00826816
	10 8.91122891
	20 8.955617960000001
	30 8.954921720000002
	40 8.97189388
	50 8.96940502
	60 9.028086029999999
	70 9.071630319999999
	80 9.148389489999998
	90 9.1000812
	100 9.001063570000001
	200 9.11945677
	300 9.12631841
	400 9.1229377
	500 9.178260620000001
	600 9.20484746
	700 9.233973240000001
	800 9.36449387
	900 9.38463454
	1000 9.36427548
	2000 9.67332437
	3000 10.133002920000001
	4000 10.526392160000002
	5000 10.764076529999997
	6000 11.09774655
	7000 11.604665400000002
	8000 12.029726700000001
	9000 12.452970700000002
	10000 12.845587099999998
end
