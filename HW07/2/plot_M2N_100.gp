set autoscale
#set logscale x
#set logscale y
set xlabel 'Time'
set ylabel '$M^2/N$'
set title "HW07-2"
plot \
'output_100.dat' u 1:7 t '$N=100$'
