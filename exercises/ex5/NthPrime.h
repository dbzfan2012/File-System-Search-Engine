/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 4
 */

#ifndef EX5_NTHPRIME_H_
#define EX5_NTHPRIME_H_

#include <inttypes.h> // for int64_t etc.

// Finds and returns the nth prime number, returning -1 if
// an invalid input is given. For example:
//   NthPrime(1) --> 2
//   NthPrime(2) --> 3
//   NthPrime(3) --> 5
//   NthPrime(4) --> 7
//   etc.
// num must be > 0
int64_t NthPrime(int16_t num);
#endif  //EX5_NTHPRIME_H_
