 # Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 # Ethan Kogan
 # dbzfan@uw.edu
 # Exercise 9
 

CXX = g++
CPPFLAGS = -Wall -g -std=c++11
PROGS = ex9

# default target attempts to build all executables
all: $(PROGS)

ex9: ex9.o Vector.o
	$(CXX) $(CPPFLAGS) -o $@ $^

ex9.o: ex9.cc Vector.h
	$(CXX) $(CPPFLAGS) -c $<

Vector.o: Vector.cc Vector.h
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf *.o *~ *.dSYM $(PROGS)
