/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 4
 */
#include "NthPrime.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// The length of the test array
#define LEN 5

int main(int argc, char **argv) {
    // Defines an array of test cases for primes, the answers should:
    // The 1st prime number is 2, second is 3, tenth is 29, hundredth is 541
    // and the thousandth is 7919
    int16_t tests[] = {1, 2, 10, 100, 1000};
    for (int i = 0; i < LEN; i++) {
        int64_t prime = NthPrime(tests[i]);
        printf("Prime #%i is: %li\n", tests[i], prime);
        if (prime == -1) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
