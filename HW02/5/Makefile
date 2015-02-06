NAME = figure

# set variables
MACRO   = plot.gp
SETTINGS = settings.gp
TEX = gnuplot

# set executable name
EXECNAME    = 2d_oscillator

# default compiler settings
CC          =  g++
OPT         = -O3 
LDFLAGS     = -lm

SRC         = *.cc
OBJS        = $*(SRC).o

# compilation for runs
all:
	$(CC) $(OPT) $(SRC) -o $(EXECNAME) $(LDFLAGS) 
	/bin/rm -rf *.o

# make eps
paper:
	
	echo "\documentclass{article}"      >  $(TEX).tex
	echo "\usepackage{graphics}"        >> $(TEX).tex
	echo "\usepackage{nopageno}"        >> $(TEX).tex
	echo "\usepackage[usenames]{color}"     >> $(TEX).tex
	echo "\uegin{document}"             >> $(TEX).tex
	echo "\uegin{center}"               >> $(TEX).tex
	echo "\input{./plot_traj.tex}"       >> $(TEX).tex
	echo "\pagebreak"                  >> $(TEX).tex
	echo "\input{./plot_ex.tex}"       >> $(TEX).tex
	echo "\end{center}"             >> $(TEX).tex
	echo "\end{document}"           >> $(TEX).tex
	perl -pi -e 's/uegin/begin/g' $(TEX).tex
	gnuplot -e "load 'settings.gp'" plot_traj.gp
	gnuplot -e "load 'settings.gp'" plot_ex.gp
	latex   $(TEX).tex
	dvips   $(TEX).dvi -o $(TEX).ps
	ps2pdf14  $(TEX).ps
	mv      $(TEX).pdf $(NAME).pdf
	rm *.eps
	rm *.tex
	rm $(TEX).*


# clean up
clean:
	rm -rf $(NAME).pdf
	rm -rf *.o $(EXECNAME)

