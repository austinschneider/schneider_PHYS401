# set executable name
EXECNAME    = anh_oscillator

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

# clean up
clean:
	rm -rf *.o $(EXECNAME)

