/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 11
 */

#include <iostream>       // for stdout, EXIT_SUCCESS, EXIT_FAILURE
#include <bits/stdc++.h>  // for vector, sort
#include "read_value.h"   // for ReadValue

int main(int argc, char **argv) {
  // Input checking
  if (argc != 1) {
    std::cout << "Usage: ./ex11" << std::endl;
    return EXIT_FAILURE;
  }

  // Reads 6 values from standard input through the
  // ReadValue template. Throws an error if an incorrect
  // input is given
  std::cout << "Enter 6 doubles:" << std::endl;
  std::vector<double> values;
  for (int i = 0; i < 6; i++) {
    double value = ReadValue<double>(std::cin);
    values.push_back(value);
  }

  // Sorts the values and prints it to standard output
  std::sort(values.begin(), values.end());
  std::cout << "Your sorted doubles are:" << std::endl;
  for (int i = 0; i < 6; i++) {
    std::cout << values.at(i) << std::endl;
  }
}
