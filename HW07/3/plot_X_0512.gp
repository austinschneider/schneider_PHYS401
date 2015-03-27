set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel '$X$'
set title "HW07-3 N=512"

dx = 0.01
d(y) = ($0 == 0) ? (y1=y, 0) : (y2=y1, y1=y, (y1-y2)/dx)

plot \
'output_512.dat' u 1:((($13)/($1))/512) t 'Thermal fluctuations', \
'output_512.dat' u ($1-dx):(d($12)/512) t 'Numeric Derivative'
