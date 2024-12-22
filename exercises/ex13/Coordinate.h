/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <string>  // For string

// Abstract class for a type of Coordinate
class Coordinate {
 public:
  // Gets coordinate string information, to be implemented elsewhere
  virtual std::string ToString() const = 0;
};

#endif  // COORDINATE_H_
