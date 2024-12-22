/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#include "Cartesian.h"  // For cartesian

#include <string>       // For string
#include <sstream>      // For stringstream
#include <cmath>        // For pow, sqrt

// Calculates the distance between two Cartesian points
// using Pythagoras's theorem
double Cartesian::Distance(Cartesian other) const {
  double x_dist_sq = pow(other.x_ - x_, 2);
  double y_dist_sq = pow(other.y_ - y_, 2);
  return sqrt(x_dist_sq + y_dist_sq);
}

// Gets the string information from the point
std::string Cartesian::ToString() const {
  std::stringstream ss;
  ss << "(" << x_ << ", " << y_ << ")";
  return ss.str();
}
