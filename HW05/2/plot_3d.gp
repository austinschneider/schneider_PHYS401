set terminal epslatex color
set output 'plot_3d.tex'
set multiplot

splot 'twister_3d.dat' w points pt 0
