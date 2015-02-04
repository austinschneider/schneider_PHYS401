set xlabel 't'                  # set label x axis
set ylabel 'E'              # set label y axis


set output 'plot_energy.tex'
set size 1.0, 1.0
set multiplot layout 1,1
#
set yrange [0:100]    		# set range of y axis
set xrange [0:2]		# set range of x axis
set format y "%1.5f"
set format x "%1.5f"
set xtics 1     		# set x mayor tics
set mxtics 10			# set number of x minor tics
set ytics   			# set y mayor tics
set mytics 10  			# set number of y minor tics
set title "RK4"
unset key
plot \
'orbit_rk4.dat' u 1:4 title "ballistic"

#
set yrange [0:100]    		# set range of y axis
set xrange [0:2]		# set range of x axis
set format y "%1.5f"
set format x "%1.5f"
set xtics 1     		# set x mayor tics
set mxtics 10			# set number of x minor tics
set ytics   			# set y mayor tics
set mytics 10  			# set number of y minor tics
set title "Velocity Verlet"
unset key
plot \
'orbit_verlet.dat' u 1:4 title "ballistic"


#########################################################
# symbols
#########################################################
#               12              empty diamond
#                6              emtpy circle
#                4              empty square
#                8              empty triangle up
#               10              empty triangle down
#               14              empty pentagon
#               13              filled diamond
#                7              filled circle
#                5              filled square
#                9              filled triangle up
#               11              filled triangle down
#               15              filled pentagon
#########################################################
# colors
#########################################################
#		#FF0000		red
#		#347235		dark green
#		#2B65EC		blue
#		#000000		black
#		#571B7E		dark purple
#		#F88017		dark orange
#		#B048B5		dark pink
#		#413839		gray 30 (darkest)
#		#595454		gray 40
#		#747170		gray 50
#########################################################
# line styles
#########################################################
#		0		dotted
#		1		solid
#		3		dashed
#########################################################
