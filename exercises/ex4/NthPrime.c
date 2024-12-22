/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 4
 */
#include "NthPrime.h"
#include <stdbool.h>

// The only even prime value
#define EVEN_PRIME 2
// The value to return if an invalid input is given
#define ERROR -1

// Declares the isPrime function
bool isPrime(int64_t value);

// Finds and returns the nth prime number, returning -1 if
// an invalid input is given
int64_t NthPrime(int16_t num) {
    // An error value is returned with invalid input
    if (num <= 0) {
        return ERROR;
    }
    // The only even prime number is returned
    if (num == 1) {
        return EVEN_PRIME;
    }
    int64_t prime = 1;
    while (num > 1) {
        prime += 2;
        if (isPrime(prime)) {
            num--;
        }
    }
    return prime;
}

// The isPrime function checks to see if an individual
// number is prime or not
bool isPrime(int64_t value) {
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
