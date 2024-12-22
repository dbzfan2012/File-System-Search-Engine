/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 8
 */

#include <iostream>  // for stdout, EXIT_SUCCESS, EXIT_FAILURE

// Takes in a positive integer and prints out
// all of its factors to stdout
void PrintFactors(int factorize);

int main(int argc, char **argv) {
  // Input checking
  if (argc != 1) {
    std::cout << "Usage: ./ex8" << std::endl;
    return EXIT_FAILURE;
  }

  // Takes in a value from standard input, throws
  // an error if invalid
  int factorize = 0;
  std::cout << "Which positive integer would you like me to factorize? ";
  std::cin >> factorize;
  if (factorize <= 0) {
    std::cout << "Please input a positive integer" << std::endl;
    std::cout << "Usage: ./ex8" << std::endl;
    return EXIT_FAILURE;
  }

  // Checks if value hit the integer ceiling, prints
  // out values for it directly if so. Otherwise, runs method
  // to calculate and print all factors
  if (factorize == __INT_MAX__) {
    std::cout << "Integer ceiling reached, defaulting to integer ceiling of "
              << __INT_MAX__ << std::endl;
    std::cout << "1 " << __INT_MAX__ << std::endl;
  } else {
    PrintFactors(factorize);
  }
  return EXIT_SUCCESS;
}

void PrintFactors(int factorize) {
  int factor = 1;
  while (factor < factorize) {
    if (factorize % factor == 0) {
      std::cout << factor << " ";
    }
    factor++;
  }
  std::cout << factor << std::endl;
}
