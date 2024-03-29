#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-puzzle/heavy-cost/8/8puzzle.heuristic.runtime.gnuplot
#
# Started on 12/09/2023 19:56:38
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 8-Puzzle heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '8puzzle.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 0.00231703655
	2 0.00250109054
	3 0.0026109543800000006
	4 0.0025835470900000003
	5 0.0026839190400000006
	6 0.0028266049400000005
	7 0.00292366839
	8 0.00302568054
	9 0.0030857550900000004
	10 0.0032184436100000002
	20 0.0036265302400000005
	30 0.00400957821
	40 0.0042657326
	50 0.0046029121
	60 0.004789950970000001
	70 0.005087928899999999
	80 0.0054307222
	90 0.005731540799999999
	100 0.006090719400000001
	200 0.008411206899999999
	300 0.0105429343
	400 0.0126929181
	500 0.014455665500000001
	600 0.0156922069
	700 0.0166884678
	800 0.017381002
	900 0.017863371000000003
	1000 0.018721220000000004
	2000 0.032771854
	3000 0.046475786
	4000 0.060639607
	5000 0.075514587
	6000 0.08982174000000001
	7000 0.104227282
	8000 0.118646062
	9000 0.132940879
	10000 0.14827696999999998
end
	1 0.00585789715
	2 0.01092694234
	3 0.013924290579999998
	4 0.01541867402
	5 0.017051104640000002
	6 0.017716832699999997
	7 0.0181633216
	8 0.0189141539
	9 0.019704264200000002
	10 0.0202587443
	20 0.022066041100000003
	30 0.024129769699999998
	40 0.0253339981
	50 0.02680085010000001
	60 0.0276281777
	70 0.028071227200000003
	80 0.028752677299999994
	90 0.029415085799999997
	100 0.028861228899999997
	200 0.0346934707
	300 0.0365192308
	400 0.039034110200000006
	500 0.04176875929999999
	600 0.044879975
	700 0.04782079499999999
	800 0.05007230600000002
	900 0.051941032
	1000 0.051790108000000015
	2000 0.07140163399999999
	3000 0.089602136
	4000 0.10719579599999998
	5000 0.12587142099999998
	6000 0.142609677
	7000 0.15981333
	8000 0.17989604
	9000 0.19828456
	10000 0.21418344
end
	1 0.00231631174
	2 0.00372782801
	3 0.00519412516
	4 0.00637075258
	5 0.0075761502
	6 0.0088464368
	7 0.009856394979999999
	8 0.011014659380000003
	9 0.01234736518
	10 0.013334338129999998
	20 0.02353750231
	30 0.0334508829
	40 0.0436796221
	50 0.0531744847
	60 0.0630189056
	70 0.0782406745
	80 0.09199943199999999
	90 0.1052073974
	100 0.1177612639
	200 0.27125035799999997
	300 0.44428743
	400 0.6401437419999999
	500 0.8635064570000003
	600 1.118288361
	700 1.4034950849999999
	800 1.732914777
	900 2.07975924
	1000 2.48676201
end