set autoscale
#set logscale x
#set logscale y
set xlabel 'Generation'
set ylabel 'Fitness'
set title "HW09-2"
unset key
plot \
'fitness.dat' u 1:2
