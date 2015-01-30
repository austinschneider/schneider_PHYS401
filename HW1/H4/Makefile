# plot file name
NAME = figure

# set variables
MACRO 	= plot.gp
SETTINGS = settings.gp
TEX	= gnuplot

# make eps
paper: *.gp

	echo "\documentclass{article}" 		>  $(TEX).tex
	echo "\usepackage{graphics}"		>> $(TEX).tex
	echo "\usepackage{nopageno}" 		>> $(TEX).tex
	echo "\usepackage[usenames]{color}" 	>> $(TEX).tex
	echo "\uegin{document}" 		>> $(TEX).tex
	echo "\begin{center}" 			>> $(TEX).tex
	echo "\input{./plot.tex}" 		>> $(TEX).tex
	echo "\end{center}" 			>> $(TEX).tex
	echo "\end{document}" 			>> $(TEX).tex
	perl -pi -e 's/uegin/begin/g' $(TEX).tex

	gnuplot -e "load 'settings.gp'" plot.gp
	latex 	$(TEX).tex
	dvips	$(TEX).dvi -o $(TEX).ps
	ps2pdf14  $(TEX).ps
	mv      $(TEX).pdf $(NAME).pdf
	rm	*.eps
	rm *.tex
	rm $(TEX).*

# clean up!
clean:

	rm -rf $(NAME).pdf
 
