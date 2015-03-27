set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel '$C_V$'
set title "HW07-3 N=256"

dx = 0.01
d(y) = ($0 == 0) ? (y1=y, 0) : (y2=y1, y1=y, (y1-y2)/dx)

plot \
'output_256.dat' u 1:((($5)/($1)**2)/256) t 'Thermal fluctuations', \
'output_256.dat' u ($1-dx):(d($4)/256) t 'Numeric Derivative'
