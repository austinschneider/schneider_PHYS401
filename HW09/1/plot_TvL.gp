set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel 'L'
set title "HW09-1"
plot \
'out.txt' u 1:2 t '$$'
