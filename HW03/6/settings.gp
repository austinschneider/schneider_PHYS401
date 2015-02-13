set terminal epslatex color	# needed to TeX the strings
#set multiplot

# set details about key
#set key at graph 0.25, graph 0.91 width -0
#set key Left reverse 	
#set key samplen 1.5		# set length of error bars in key
#set key box linestyle 1 lc rgb "#000000"

# set size of plot
#unset key
#set size ratio 0.72
#set lmargin at screen 0
#set rmargin at screen 0.72
#set bmargin at screen 0
#set tmargin at screen 0.72

# set plot ranges/axes/ticks
#set format y "%3.0f"
#set format x "$%1.0t$"
#set yrange [1e-8:4]    		# set range of y axis
#set xrange [2:8]		# set range of x axis
#set logscale y
#set xtics 1     		# set x mayor tics
#set mxtics 10			# set number of x minor tics
#set ytics   			# set y mayor tics
#set mytics 10  			# set number of y minor tics
set grid back lc rgb "#000000"	# set visual grid on plot

# set axes labels
#set xlabel 't'		         	# set label x axis
#set ylabel 'E' 				# set label y axis

# styles for symbol plotting
set style line  1 lt 1 lw 1 pt  7 ps 0.5 lc rgb "#FF0000"
set style line  2 lt 1 lw 1 pt  5 ps 1.8 lc rgb "#2B65EC"
set style line  3 lt 1 lw 1 pt  9 ps 1.8 lc rgb "#347235"
set style line  4 lt 1 lw 1 pt 13 ps 1.8 lc rgb "#571B7E"
set style line  5 lt 1 lw 1 pt 15 ps 1.8 lc rgb "#F88017"
set style line  6 lt 1 lw 1 pt  7 ps 1.8 lc rgb "#B048B5"

# styles for lines
set style line 11 lt 1 lw 1 lc rgb "#FF0000"
set style line 12 lt 1 lw 1 lc rgb "#2B65EC"
set style line 13 lt 1 lw 1 lc rgb "#347235"
set style line 14 lt 1 lw 1 lc rgb "#571B7E"
set style line 15 lt 1 lw 1 lc rgb "#F88017"

#plot \
#'single_precision_data.dat'   u 1:2 title "Single precision error"

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
