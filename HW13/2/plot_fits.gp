l(x) = a + b*x
q(x) = c + d*x + e*(x**2)
set fit errorscaling
set xlabel 'X'
set ylabel 'Y'
set title "HW13-2 fits"
fit l(x) 'L2B.dat' using 1:2:3 yerr via a, b
fit q(x) 'L2B.dat' using 1:2:3 yerr via c, d, e
plot 'L2B.dat' using 1:2:3 with errorbars, l(x), q(x)
