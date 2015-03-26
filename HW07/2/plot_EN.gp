set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel 'E/N'
set title "HW07-1"
plot \
'output_10.dat' u 1:2:3 t '$N=10$' with yerrorbars, \
'output_100.dat' u 1:2:3 t '$N=100$' with yerrorbars, \
'output_1000.dat' u 1:2:3 t '$N=1000$' with yerrorbars, \
(exp(-1/x) - exp(1/x))/(exp(-1/x) + exp(1/x)) t 'expected' with lines
