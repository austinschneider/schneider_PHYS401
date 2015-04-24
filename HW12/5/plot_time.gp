set autoscale
set logscale x
set logscale y
set xlabel 'Number of cores'
set ylabel 'Time (s)'
set title "HW-12 $x^3$ Integration"
plot \
'timing.dat' u 0:1

