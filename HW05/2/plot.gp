set output 'plot.tex'
set size 1.0, 1.0
set multiplot layout 1,1

#
set autoscale
set logscale x
set logscale y
set title "Twister"
plot \
'twister.dat' u 1:2, 1/sqrt(x) with lines

#
set autoscale
set logscale x
set logscale y
set title "Helmut"
plot \
'helmut.dat' u 1:2, 1/sqrt(x) with lines


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
