set output 'plot_c.tex'
set size 1.0, 2.0
set multiplot layout 2,1
#
set origin 0.0, 1.0
set size 1.0, 1.0
set yrange [1e-8:4]            # set range of y axis
set xrange [2:8]        # set range of x axis
set logscale y
set format y "%1.1e"
set xtics 1             # set x mayor tics
set mxtics 10           # set number of x minor tics
set ytics               # set y mayor tics
set mytics 10           # set number of y minor tics
set title "Single precision $S_{up}$ error"
unset key
plot \
'single_sup_err.dat' u 1:2 ls 1 title "Single precision $S_{up}$ error"

#
set origin 0.0, 0.0
set size 1.0, 1.0
set yrange [1e-8:1]            # set range of y axis
set xrange [2:8]        # set range of x axis
set logscale y
set format y "%1.1e"
set xtics 1             # set x mayor tics
set mxtics 10           # set number of x minor tics
set ytics               # set y mayor tics
set mytics 10           # set number of y minor tics
set title "Single precision $S_{down}$ error"
unset key
plot \
'single_sdown_err.dat' u 1:2 ls 1 title "Single precision $S_{down}$ error"