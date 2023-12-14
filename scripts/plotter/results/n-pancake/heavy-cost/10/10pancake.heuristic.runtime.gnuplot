#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
#
# results/n-pancake/heavy-cost/10/10pancake.heuristic.runtime.gnuplot
#
# Started on 12/07/2023 15:25:01
# Author: Carlos Linares López
set grid
set xlabel "k"
set ylabel "runtime"

set title "Runtime (seconds) - 10-Pancake heavy-cost"

set terminal png enhanced font "Ariel,10"
set output '10pancake.heuristic.runtime.png'

plot "-" title "BELA*"      with linesp, "-" title "K*"      with linesp, "-" title "mA*"      with linesp

	1 0.3907041436
	2 0.4069330848999999
	3 0.41668445670000004
	4 0.4263677359
	5 0.44164810520000003
	6 0.4570789893
	7 0.4618432872
	8 0.4655680913
	9 0.4721189623
	10 0.4749869636000001
	20 0.5296099571
	30 0.5595491980000001
	40 0.5834536360000001
	50 0.611012902
	60 0.623873738
	70 0.640229147
	80 0.6533266219999999
	90 0.66887354
	100 0.6728519629999999
end
	1 1.990331275
	2 2.470188488
	3 3.0391305079999995
	4 3.321329682
	5 3.4380584999999995
	6 3.915653061
	7 4.230301695999999
	8 4.551894305
	9 4.763345285
	10 4.854515903999999
	20 6.31761294
end
	1 0.3790411672
	2 0.7386003010000002
	3 1.0955422929999998
	4 1.4112493240000001
	5 1.7852129319999996
	6 2.1499644190000002
	7 2.488677899
	8 2.78836989
	9 3.1416978929999995
	10 3.504419976
	20 6.782060680999999
end
