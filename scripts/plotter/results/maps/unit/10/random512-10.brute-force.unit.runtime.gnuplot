#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/maps/unit/10/random512-10.brute-force.unit.runtime.gnuplot
#
# Started on 04/07/2024 18:12:05
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - Maps 10 unit"

set terminal png enhanced font "Ariel,10"
set output 'random512-10.brute-force.unit.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.22281392
	2 0.22232122
	3 0.22253257999999998
	4 0.22457851
	5 0.22279424000000003
	6 0.22327641
	7 0.22482437
	8 0.22663674000000003
	9 0.22788603999999996
	10 0.22289611
	20 0.22266135
	30 0.22367735999999996
	40 0.22385704999999997
	50 0.22602916
	60 0.22798783000000003
	70 0.22937282
	80 0.23025232000000004
	90 0.23093001000000002
	100 0.22447368
	200 0.22575879999999998
	300 0.22672380999999997
	400 0.22920661999999997
	500 0.23095918000000001
	600 0.23232802000000002
	700 0.23396803000000002
	800 0.23667931
	900 0.23600930999999994
	1000 0.23022493
	2000 0.24006002
	3000 0.26500421999999996
	4000 0.2737089
	5000 0.28443849
	6000 0.29682228
	7000 0.31683900000000004
	8000 0.33038749
	9000 0.3314570099999999
	10000 0.34042091999999996
end
	1 0.72489798
	2 0.7231428399999998
	3 0.72433723
	4 0.7266129899999999
	5 0.7261978599999999
	6 0.72708766
	7 0.7304459299999999
	8 0.73269376
	9 0.73204572
	10 0.7294489299999999
	20 0.7320205499999998
	30 0.73293487
	40 0.7359951600000001
	50 0.7396253
	60 0.7419469900000002
	70 0.7450760000000001
	80 0.7478960499999999
	90 0.75058081
	100 0.7486760100000001
	200 0.7695069199999999
	300 0.7977346400000002
	400 0.8294989799999999
	500 0.86541223
	600 0.9055054
	700 0.94575137
	800 0.9688849099999999
	900 0.9887787700000001
	1000 1.01147222
end
	1 0.260876
	2 0.47500627
	3 0.68397075
	4 0.8793689599999999
	5 1.0893256000000002
	6 1.3183766999999997
	7 1.5415716999999998
	8 1.7704893
	9 2.0192176
	10 2.1459258
	20 4.7340782
	30 7.7656035
	40 10.955314000000001
	50 14.526777000000004
	60 18.232988999999996
	70 26.299200000000006
	80 31.666823
	90 37.199225999999996
	100 42.93573799999999
end
