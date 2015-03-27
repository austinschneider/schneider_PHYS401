SHELL := /bin/bash
NAME = figure

# set variables
MACRO   = plot.gp
SETTINGS = settings.gp
TEX = gnuplot

# set executable name
EXECNAME    = main

# default compiler settings
CC          =  g++
OPT         = -O3
LDFLAGS     = -lm

SRC         = *.cc ../../Global/*.cc
OBJS        = $*(SRC).o

# compilation for runs
all:
	$(CC) -g $(OPT) $(SRC) -o $(EXECNAME) $(LDFLAGS)
	#/bin/rm -rf *.o

# make pdf
paper:
	source ../../Global/functions.sh && make_tex figure

# clean up
clean:
	rm -rf $(NAME).pdf
	rm -rf *.o $(EXECNAME)
