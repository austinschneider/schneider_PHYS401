set autoscale
#set logscale x
#set logscale y
set xlabel '$L^{1/v}(T-T_c)$'
set ylabel '$g$'
set title "HW08-2"

plot \
'output_256.dat' u (16*($1-2.269)):(0.5*(3-($20)/(($16)**2))) t '$N=256$', \
'output_576.dat' u (24*($1-2.269)):(0.5*(3-($20)/(($16)**2))) t '$N=576$', \
'output_1024.dat' u (32*($1-2.269)):(0.5*(3-($20)/(($16)**2))) t '$N=1024$'
