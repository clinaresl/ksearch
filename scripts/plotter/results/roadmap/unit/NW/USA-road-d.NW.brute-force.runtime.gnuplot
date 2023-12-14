#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/unit/NW/USA-road-d.NW.brute-force.runtime.gnuplot
#
# Started on 12/13/2023 18:52:45
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - NW Roadmap unit"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.NW.brute-force.runtime.png'

plot "-" title "BELA0"      with linesp, "-" title "K0"      with linesp, "-" title "mDijkstra"      with linesp

	1 0.6773847299999999
	2 0.68960674
	3 0.6919972899999999
	4 0.6921315899999999
	5 0.69458977
	6 0.6947947600000001
	7 0.69632194
	8 0.6969521799999999
	9 0.6972004900000001
	10 0.69128281
	20 0.6930569900000001
	30 0.69443776
	40 0.69504062
	50 0.6949788800000002
	60 0.69873234
	70 0.69816675
	80 0.69950755
	90 0.70196498
	100 0.6973923900000001
	200 0.6998766199999998
	300 0.7059241700000001
	400 0.7081018
	500 0.7112237499999999
	600 0.71500879
	700 0.71830052
	800 0.71951821
	900 0.72346376
	1000 0.7209817299999999
	2000 0.7459481800000001
	3000 0.77332876
	4000 0.8000067200000001
	5000 0.8298090600000001
	6000 0.85659872
	7000 0.88445132
	8000 0.9119080900000001
	9000 0.94168539
	10000 0.96660958
end
	1 1.47806355
	2 1.48465512
	3 1.4858911600000002
	4 1.4881237999999997
	5 1.4885143000000003
	6 1.4893970700000003
	7 1.4934558599999996
	8 1.4928833499999998
	9 1.49257674
	10 1.4892853699999997
	20 1.49394595
	30 1.4956109800000001
	40 1.49919864
	50 1.4981984600000002
	60 1.5036096399999999
	70 1.5044475
	80 1.5071723399999999
	90 1.5094709999999998
	100 1.5052016400000001
	200 1.51919048
	300 1.5350785999999996
	400 1.5489953600000002
	500 1.5637228
	600 1.5813816200000002
	700 1.5951616299999998
	800 1.61112286
	900 1.6295127500000004
	1000 1.6356017
	2000 1.7965939300000002
	3000 1.95924391
	4000 2.12413104
	5000 2.2884735099999998
	6000 2.45645303
	7000 2.6231500899999998
	8000 2.7870301300000007
	9000 2.9594926599999996
	10000 3.12595388
end
	1 0.6209834230000001
	2 1.00253606
	3 1.4208504800000004
	4 1.7878654799999998
	5 2.25508399
	6 2.6480148299999997
	7 3.08252887
	8 3.49187316
	9 4.07478519
	10 4.51832746
end
