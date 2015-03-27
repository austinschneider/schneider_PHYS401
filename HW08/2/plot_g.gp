set autoscale
#set logscale x
#set logscale y
set xlabel 'T'
set ylabel '$g$'
set title "HW08-2"

plot \
'output_256.dat' u 1:(0.5*(3-($20)/(($16)**2))) t '$N=256$', \
'output_576.dat' u 1:(0.5*(3-($20)/(($16)**2))) t '$N=576$', \
'output_1024.dat' u 1:(0.5*(3-($20)/(($16)**2))) t '$N=1024$'
