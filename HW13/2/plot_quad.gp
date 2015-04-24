q(x) = y0 + m*x + n*(x**2)
set fit errorscaling
set xlabel 'X'
set ylabel 'Y'
set title "HW13-2 quadratic fit"
fit q(x) 'L2B.dat' using 1:2:3 yerr via y0, m, n
plot 'L2B.dat' using 1:2:3 with errorbars, q(x)
