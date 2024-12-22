/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 4
 */

#include <stdbool.h> // for bool
#include <stdio.h>   // for printf
#include <stdlib.h>  // for exit, EXIT_SUCCESS

#include "NthPrime.h" // for NthPrime declaration

// The only even prime value
#define EVEN_PRIME 2
// The value to return if an invalid input is given
#define ERROR -1

// Returns true if the given value is prime,
// false otherwise
static bool isPrime(int64_t value);

int64_t NthPrime(int16_t num) {
  // An error value is returned with invalid input
  if (num <= 0) {
    fprintf(stderr, "ERROR: argument to NthPrime is less than 1!");
    exit(EXIT_FAILURE);
  }
  // The only even prime number is returned
  if (num == 1) {
    return EVEN_PRIME;
  }
  int64_t prime = 1;
  while (num > 1) {
    if (prime == INT64_MAX) {
    fprintf(stderr, "ERROR: hit the largest int64_t!\n");
    exit(EXIT_FAILURE);
    }
    prime += 2;
    if (isPrime(prime)) {
      num--;
    }
  }
  return prime;
}

static bool isPrime(int64_t value) {
  // Numbers under 2 are not prime
  if (value <= 1) {
    return false;
  }
  for (int i = 2; i < value; i++) {
    if (value % i == 0) {
      return false;
    }
  }
  return true;
}
