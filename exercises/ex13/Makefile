# Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
# Ethan Kogan
# dbzfan@uw.edu
# Exercise 13

CXX = g++
OBJS = Cartesian.o Polar.o ex13.o
EXE  = ex13
HEADERS = Cartesian.h Coordinate.h Polar.h
CPPFLAGS = -Wall -g -std=c++17

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM