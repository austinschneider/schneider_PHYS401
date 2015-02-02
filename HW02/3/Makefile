# set executable name
EXECNAME    = sqrt2

# default compiler settings
CC          =  g++
OPT         = -O2 
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

