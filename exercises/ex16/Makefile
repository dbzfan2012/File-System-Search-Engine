# Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
# Ethan Kogan
# dbzfan@uw.edu
# Exercise 16

CXX = g++
CPPFLAGS = -Wall -g -std=c++17

ex16: listen.o ex16.o
	$(CXX) $(CPPFLAGS) -o ex16 listen.o ex16.o

listen.o: listen.cc listen.h
	$(CXX) $(CPPFLAGS) -c $<

ex16.o: ex16.cc listen.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf ex16 *.o *~ *.dSYM