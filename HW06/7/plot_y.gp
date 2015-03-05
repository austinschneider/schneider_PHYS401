set autoscale
#set logscale x
set logscale y
set xlabel 'y'
set ylabel 'average'
set title "HW06-7"
plot \
'output.dat' u 1:2, 'output.dat' u 1:3, x with lines

