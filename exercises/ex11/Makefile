 # Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 # Ethan Kogan
 # dbzfan@uw.edu
 # Exercise 11

CXX = g++
CPPFLAGS = -Wall -g -std=c++11
PROGS = ex11

# default target attempts to build all executables
all: $(PROGS)

ex11: ex11.o
	$(CXX) $(CPPFLAGS) -o $@ $^

ex11.o: ex11.cc read_value.h
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf *.o *~ *.dSYM $(PROGS)
