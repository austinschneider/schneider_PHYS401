set autoscale
set logscale x
set logscale y
set xlabel 'samples'
set ylabel 'error'
set title "HW06-1 pi estimation"
plot \
'output.dat' u 1:2, 1/sqrt(x) with lines

