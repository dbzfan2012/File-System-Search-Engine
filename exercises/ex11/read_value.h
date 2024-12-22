/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 11
 */

#ifndef READ_VALUE_H_
#define READ_VALUE_H_

// A template for reading in a datatype T from standard
// input and returning it. An error is thrown if the given
// data doesn't match the specified type
template <class T>
T ReadValue(std::istream& in) {
  T value;
  in >> value;
  if (!in) {
    std::cerr << "Invalid input" << std::endl;
    exit(EXIT_FAILURE);
  }
  return value;
}

#endif  // READ_VALUE_H_
