 # Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 # Ethan Kogan
 # dbzfan@uw.edu
 # Exercise 14.5 

CXX = g++
CPPFLAGS = -Wall -g -std=c++11
PROGS = ex14_5

# default target attempts to build all executables
all: $(PROGS)

ex14_5: ex14_5.o
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

ex14_5.o: ex14_5.cc
	$(CXX) $(CPPFLAGS) -c $< -I./libhw1

# phony target - remove generated files and backups
clean:
	rm -rf *.o *~ *.dSYM $(PROGS)