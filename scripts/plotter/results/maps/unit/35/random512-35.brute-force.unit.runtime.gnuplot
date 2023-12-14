#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/35/random512-35.brute-force.unit.runtime.gnuplot
#
# Started on 12/06/2023 14:52:54
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 35 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-35.brute-force.unit.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.17956101
	2 0.17875619999999998
	3 0.17904594999999998
	4 0.17932773999999999
	5 0.18023156999999998
	6 0.18090516999999998
	7 0.18241266
	8 0.18209075
	9 0.18371493000000005
	10 0.1794629
	20 0.17998851999999999
	30 0.18019411000000002
	40 0.18105501999999998
	50 0.18216659000000004
	60 0.18389583
	70 0.18456603000000002
	80 0.18596175000000004
	90 0.18668035
	100 0.18189355999999995
	200 0.18359096
	300 0.18594007999999995
	400 0.18745393000000005
	500 0.18998713000000003
	600 0.19233260999999996
	700 0.19338611
	800 0.19461204000000007
	900 0.19507255999999998
	1000 0.19181479999999998
	2000 0.20435785999999997
	3000 0.23427681
	4000 0.24046494999999996
	5000 0.26613772
	6000 0.283581
	7000 0.30314118999999995
	8000 0.32285705
	9000 0.3435399400000001
	10000 0.34087803
end
	1 0.40076153000000003
	2 0.39909759000000006
	3 0.40001071999999993
	4 0.4017291599999999
	5 0.40336701
	6 0.40395160999999985
	7 0.40478130999999995
	8 0.4059753299999999
	9 0.40653742000000004
	10 0.4044196899999999
	20 0.40701449999999995
	30 0.40976785
	40 0.41136571999999993
	50 0.41506103999999994
	60 0.41815097
	70 0.42148592
	80 0.42326565000000005
	90 0.42574054999999994
	100 0.4251606800000001
	200 0.45020159
	300 0.47310952
	400 0.4941237600000001
	500 0.51551892
	600 0.53809975
	700 0.5603999799999999
	800 0.58347112
	900 0.6076339099999999
	1000 0.63203502
end
	1 0.16124407000000002
	2 0.27689057
	3 0.39079911
	4 0.49871617
end
