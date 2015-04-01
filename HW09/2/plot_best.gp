set autoscale
#set logscale x
#set logscale y
set xlabel 'Site'
set ylabel 'Sum'
set title "HW09-2"
unset key
set boxwidth 0.5
set style fill solid
plot \
'best.dat' u 1:2:xtic(1) with boxes
