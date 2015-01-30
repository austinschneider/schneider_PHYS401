set multiplot layout 2,1 title "Test thing"
#
set yrange [1e-8:4]    		# set range of y axis
set xrange [2:8]		# set range of x axis
set logscale y
set format y "%1.1e"
set xtics 1     		# set x mayor tics
set mxtics 10			# set number of x minor tics
set ytics   			# set y mayor tics
set mytics 10  			# set number of y minor tics
set title "Single precision error ($S_{up} - S_{down}$)"
plot \
'single_precision_data.dat'   u 1:2 title "Single precision error"
#
set yrange [1e-17:1e-11]    		# set range of y axis
set xrange [2:8]		# set range of x axis
set logscale y
set format y "%1.1e"
set xtics 1     		# set x mayor tics
set mxtics 10			# set number of x minor tics
set ytics   			# set y mayor tics
set mytics 10  			# set number of y minor tics
set title "Double precision error ($S_{up} - S_{down}$)"
plot \
'double_precision_data.dat'   u 1:2 title "Single precision error"


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
