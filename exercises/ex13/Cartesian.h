/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#ifndef CARTESIAN_H_
#define CARTESIAN_H_

#include "Coordinate.h"  // For coordinate

#include <string>        // For string

// Represents a point in a 2D cartesian plane
class Cartesian : public Coordinate {
 public:
  Cartesian(double x, double y) : x_(x), y_(y) { }

  // Calculates distance between two points
  double Distance(Cartesian other) const;

  // Gets the string information from the point
  std::string ToString() const;

 protected:
  double x_, y_;
};

#endif  // CARTESIAN_H_
