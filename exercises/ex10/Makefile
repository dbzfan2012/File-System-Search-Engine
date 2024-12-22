 # Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 # Ethan Kogan
 # dbzfan@uw.edu
 # Exercise 10
 

CXX = g++
CPPFLAGS = -Wall -g -std=c++11
PROGS = ex10

# default target attempts to build all executables
all: $(PROGS)

ex10: ex10.o Vector.o
	$(CXX) $(CPPFLAGS) -o $@ $^

ex10.o: ex10.cc Vector.h
	$(CXX) $(CPPFLAGS) -c $<

Vector.o: Vector.cc Vector.h
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf *.o *~ *.dSYM $(PROGS)
