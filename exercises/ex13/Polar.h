/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#ifndef POLAR_H_
#define POLAR_H_

#include "Coordinate.h"  // For coordinate

#include <string>        // For string

// Represents a polar coordinate in a 2D cartesian plane
class Polar : public Coordinate {
 public:
  Polar(double radius, double angle) : radius_(radius), angle_(angle) { }

  // Calculates distance between two polar coordinates
  double Distance(Polar other) const;

  // Gets the string information from the polar coordinates
  std::string ToString() const;

 protected:
  double radius_, angle_;
};

#endif  // POLAR_H_
