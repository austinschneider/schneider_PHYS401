# set executable name
EXECNAME    = ballistic

# default compiler settings
CC          =  g++
OPT         = -O2 -g 
LDFLAGS     = -lm

SRC         = *.cc
OBJS        = $*(SRC).o

# compilation for runs
all:
	$(CC) $(OPT) $(SRC) -o $(EXECNAME) $(LDFLAGS) 
	/bin/rm -rf *.o

# clean up
clean:
	rm -rf *.o $(EXECNAME)

