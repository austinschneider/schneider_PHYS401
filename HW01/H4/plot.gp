set output 'plot.tex'
set size 1.0, 1.0
set multiplot layout 1,1
#
set origin 0.0, 0.0
set size 1.0, 1.0
set yrange [0:50]    		# set range of y axis
set xrange [0:80000000]		# set range of x axis
set format y "%1.2f"
set xtics 10000000     		# set x mayor tics
set mxtics 10			# set number of x minor tics
set ytics			# set y mayor tics
set mytics 10  			# set number of y minor tics
set title "Timing for jackknife mean calculation"
unset key
plot \
'timing.dat' u 1:2 ls 1 title "timing"
