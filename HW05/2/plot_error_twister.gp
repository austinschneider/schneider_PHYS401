set size 1.0, 1.0
set multiplot layout 1,1

#
set autoscale
set logscale x
set logscale y
set title "Twister"
plot \
'twister.dat' u 1:2, 1/sqrt(x) with lines

