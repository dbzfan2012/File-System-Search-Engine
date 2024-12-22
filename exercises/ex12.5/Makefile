# Copyright ©2024 Yiqing Wang.  All rights reserved.  Permission is
# hereby granted to students registered for University of Washington
# CSE 333 for use solely during Spring Quarter 2024 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

CXX = g++
OBJS = Cat.o Sphynx.o ex12_5.o
EXE  = ex12_5
HEADERS = Cat.h Sphynx.h
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