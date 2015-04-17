set autoscale
set logscale x
set logscale y
set xlabel 'samples'
set ylabel 'error'
set title "HW06-4 10d Integration"
plot \
'output.dat' u 1:2, 1/sqrt(x) with lines

