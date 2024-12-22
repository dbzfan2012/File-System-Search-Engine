/*
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 0
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Calculates the value of pi based on the n-th element of Nilakantha infinite series.
double calculate_pi(int n) {
  double pi = 3.0;
  while (n > 0) {
    double current_add = 4.0 / ((double) ((2.0 * n) * (2.0 * n + 1.0) * (2.0 * n + 2.0)));
    if (n % 2 == 0) {
      current_add *= -1;
    }
    pi += current_add;
    n--;
  }
  return pi;
}

// Takes in the number of times to run the Nilakantha series as the first command line 
// argument and prints out the value of pi to 20 decimal places.
// Prints out a warning message if more than 1 argument is given, but proceeds anyway.
// If the given argument is less than 0, overflows to less than 0, or is of an invalid data type,
// an error message is printed.
int main (int argc, char* argv[]) {
  bool error = false;
  double pi = 0.0;
  if (argc > 1) {
	  // Checks whether more than 1 argument was given, prints a warning if so.
	  if (argc > 2) {
        printf("Please only provide one argument next time, continuing anyway...\n");
      }

  	  char* endptr = argv[2] - 1;
	  int n = (int) strtol(argv[1], &endptr, 10);

	  // Checks whether the given first argument is a string instead of a number,
	  // marks it as errored if so.
	  if (*endptr != 0) {
	    printf("Please input a valid integer...\n");
		error = true;
	  }
		
	  // isNegative checks if the value of n is negative regardless of overflow,
	  // marks it as errored if so.
	  bool isNegative = n < 0 || (int) argv[1][0] == 45;
	  if (isNegative) {
	    printf("Your argument is invalid, please try again...\n");
		error = true;
	  }
	  if (!error) {
	    pi += calculate_pi(n);
	  }
  }
  if (!error) {
    if (pi == 3.0 || (int) pi == 0.0) {
      printf("Our estimate of Pi is %0.1f\n", pi);
    } else {
      printf("Our estimate of Pi is %.20f\n", pi);
    }
  }
  return 0;
}
