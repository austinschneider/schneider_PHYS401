set autoscale
#set logscale x
#set logscale y
set xlabel 'Time'
set ylabel 'E/N'
set title "HW07-2"
plot \
'output_100.dat' u 1:3 t '$N=100$'
