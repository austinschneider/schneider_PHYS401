set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel '$X/N$'
set title "HW08-1 N=64"

dx = 0.01
d(y) = ($0 == 0) ? (y1=y, 0) : (y2=y1, y1=y, (y1-y2)/dx)

plot \
'output_64.dat' u 1:((($13)/($1))/64) t 'Thermal fluctuations'
