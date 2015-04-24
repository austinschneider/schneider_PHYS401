l(x) = y0 + m*x
set fit errorscaling
set xlabel 'X'
set ylabel 'Y'
set title "HW13-2 linear fit"
fit l(x) 'L2B.dat' using 1:2:3 yerr via y0, m
plot 'L2B.dat' using 1:2:3 with errorbars, l(x)
