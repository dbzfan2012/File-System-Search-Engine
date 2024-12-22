# Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
# Ethan Kogan
# dbzfan@uw.edu
# Exercise 5

CC = gcc
CFLAGS += -Wall -g -std=c17
OBJS = ex5.o NthPrime.o

# Compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile.
all: ex5

ex5: $(OBJS)
	$(CC) $(CFLAGS) -o ex5 $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	/bin/rm -f *.o *~ ex5

