/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#include "Polar.h"  // For polar

#include <string>   // For string
#include <sstream>  // For stringstream
#include <cmath>    // For pow, sqrt, cos

// Calculates the distance between two Polar points
// using Pythagoras's theorem
double Polar::Distance(Polar other) const {
  double square_sum = pow(radius_, 2) + pow(other.radius_, 2);
  double cos_product = 2 * radius_ * other.radius_ * cos(angle_ - other.angle_);
  return sqrt(square_sum - cos_product);
}

// Gets the string information from the point
std::string Polar::ToString() const {
  std::stringstream ss;
  ss << "(" << radius_ << ", " << angle_ << ")";
  return ss.str();
}
