#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/roadmap/dimacs/COL/USA-road-d.COL.heuristic.runtime.gnuplot
#
# Started on 01/09/2024 14:36:34
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - COL Roadmap dimacs"

set terminal png enhanced font "Ariel,10"
set output 'USA-road-d.COL.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 0.527761794
	2 0.5306168640000001
	3 0.531069334
	4 0.5320315379999999
	5 0.5329159189999999
	6 0.532807159
	7 0.534845883
	8 0.535313645
	9 0.5371110679999999
	10 0.533119547
	20 0.538235849
	30 0.539781166
	40 0.5440621989999999
	50 0.547776301
	60 0.554678565
	70 0.5574647600000001
	80 0.5607808229999999
	90 0.563256912
	100 0.5670776099999999
	200 0.5967052049999999
	300 0.629391648
	400 0.659745559
	500 0.69106094
	600 0.7217086350000002
	700 0.7525520700000001
	800 0.7821316500000001
	900 0.81006203
	1000 0.8363045499999999
	2000 1.12022636
	3000 1.3947840399999996
	4000 1.66422351
	5000 1.9294232900000003
	6000 2.1971102899999995
	7000 2.46092015
	8000 2.7215069799999996
	9000 2.98132286
	10000 3.23851437
end
	1 0.8312837070000001
	2 0.832651584
	3 0.83462246
	4 0.8389166640000001
	5 0.8402375469999999
	6 0.8417024760000001
	7 0.842587907
	8 0.8434488359999999
	9 0.843466736
	10 0.840670078
	20 0.843482516
	30 0.846616773
	40 0.8483322900000002
	50 0.849973572
	60 0.8510038590000001
	70 0.8561521500000001
	80 0.8566427600000001
	90 0.859341513
	100 0.856885553
	200 0.873432614
	300 0.8912437330000001
	400 0.905318556
	500 0.9210567530000001
	600 0.935784619
	700 0.9521956629999999
	800 0.9666297730000001
	900 0.982882909
	1000 0.9931210140000001
	2000 1.15015633
	3000 1.31016072
	4000 1.4779168699999996
	5000 1.6460934199999997
	6000 1.81236914
	7000 1.9840355499999998
	8000 2.1588807799999996
	9000 2.331049089999999
	10000 2.5025476200000005
end
	1 0.4791423020000001
	2 0.7490943250000001
	3 1.02724583
	4 1.2916971000000002
	5 1.57040645
	6 1.8438125599999997
	7 2.1225022399999998
	8 2.4089912400000006
	9 2.7136777100000002
	10 3.0052211299999994
end