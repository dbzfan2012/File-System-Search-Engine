# Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
# Ethan Kogan
# dbzfan@uw.edu
# Exercise 15

CXX = g++
CPPFLAGS = -Wall -g -std=c++17

ex15: connect.o ex15.o
	$(CXX) $(CPPFLAGS) -o ex15 connect.o ex15.o

connect.o: connect.cc connect.h
	$(CXX) $(CPPFLAGS) -c $<

ex15.o: ex15.cc connect.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf ex15 *.o *~ *.dSYM