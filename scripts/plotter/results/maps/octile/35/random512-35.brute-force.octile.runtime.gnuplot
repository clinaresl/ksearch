#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/octile/35/random512-35.brute-force.octile.runtime.gnuplot
#
# Started on 04/07/2024 18:14:12
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 35 octile"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.brute-force.octile.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.26901763999999995
	2 0.26780182
	3 0.26721229999999996
	4 0.26534005
	5 0.26640068000000006
	6 0.26542943999999996
	7 0.26775177000000006
	8 0.26601207
	9 0.26519504
	10 0.2656173399999999
	20 0.26637067
	30 0.26644708
	40 0.26641915
	50 0.26718693
	60 0.26715913
	70 0.26755177
	80 0.26759839
	90 0.26816706000000007
	100 0.26728454999999995
	200 0.26836347
	300 0.2702913
	400 0.26865018999999996
	500 0.26804470999999996
	600 0.27051672
	700 0.27289454
	800 0.27330282000000006
	900 0.27380245
	1000 0.26966861
	2000 0.27509245
	3000 0.29491473
	4000 0.29986915
	5000 0.29916606
	6000 0.29593864999999997
	7000 0.30330545
	8000 0.30648137000000003
	9000 0.32485016
	10000 0.32166613999999993
end
	1 0.7516068199999999
	2 0.75061651
	3 0.7515276299999999
	4 0.75277807
	5 0.7553186900000002
	6 0.7563523499999999
	7 0.75630204
	8 0.7582521999999998
	9 0.7602311900000001
	10 0.7558031399999998
	20 0.7564407400000002
	30 0.75946488
	40 0.76060731
	50 0.7640561400000001
	60 0.7658311400000001
	70 0.7687959099999999
	80 0.7709305000000002
	90 0.7747037599999999
	100 0.7704011399999999
	200 0.7851469299999999
	300 0.7982030200000001
	400 0.81187711
	500 0.8258126999999997
	600 0.8387243400000002
	700 0.8517099200000001
	800 0.8675939500000002
	900 0.8829500500000003
	1000 0.89132795
	2000 1.0485303000000001
	3000 1.2329493
	4000 1.4459020999999999
	5000 1.6813384999999996
	6000 1.9387707000000003
	7000 2.2222919
	8000 2.5162549
	9000 2.8172327999999998
	10000 3.1160903
end
	1 0.28880276000000005
	2 0.5161031300000001
	3 0.7309017800000001
	4 0.93405744
	5 1.162848
	6 1.3775845999999998
	7 1.5924581999999998
	8 1.8256996
	9 2.0697755
	10 2.2961975999999997
	20 5.0891057
	30 8.200102400000002
	40 11.720961000000003
	50 15.318469000000002
	60 19.409183999999996
	70 29.203242999999997
	80 34.862164
	90 40.940185
	100 47.214286
end
