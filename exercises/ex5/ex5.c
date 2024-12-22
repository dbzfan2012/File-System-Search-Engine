/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 5
 */

#include <stdlib.h>   // for EXIT_SUCCESS
#include <stdio.h>    // for printf

#include "NthPrime.h" // for NthPrime

// The length of the test array
#define LEN 5

int main(int argc, char **argv) {
  // Defines an array of test cases for primes, the answers should be:
  // The 1st prime number is 2, second is 3, tenth is 29, hundredth is 541
  // and the thousandth is 7919
  int16_t tests[] = {1, 2, 10, 100, 1000};
  for (int i = 0; i < LEN; i++) {
    int64_t prime = NthPrime(tests[i]);
    printf("Prime #%i is: %li\n", tests[i], prime);
  }
  return EXIT_SUCCESS;
}
