/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 13
 */

#include "Cartesian.h"  // For cartesian
#include "Polar.h"      // For polar

#include <iostream>     // For cout, cerr
#include <cmath>        // For round

int main() {
  // Create two cartesian points
  Cartesian c_p1(3.0, 5.0);
  Cartesian c_p2(-5.7, 12.1);

  // Output the points to standard out and the distance between them
  std::cout << "cartesian p1 is " << c_p1.ToString() << std::endl;
  std::cout << "cartesian p2 is " << c_p2.ToString() << std::endl;

  // The distance between (3.0, 5.0) and (-5.7, 12.1) should be:
  // ~11.2294
  double c_distance = c_p1.Distance(c_p2);
  double round_c_distance = std::round(c_distance * 10000.0) / 10000.0;
  std::cout << round_c_distance << std::endl;
  if (round_c_distance != 11.2294) {
    std::cerr << "The calculated cartesian point distance was incorrect"
              << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Distance between cartesian p1 and cartesian p2 is "
            << c_distance << std::endl;

  // Create two polar coordinates
  Polar p_p1(10, 3.14);
  Polar p_p2(4, 0.86);

  // Output the polar coordinates to standard out
  // and the distance between them
  std::cout << "polar p1 is " << p_p1.ToString() << std::endl;
  std::cout << "polar p2 is " << p_p2.ToString() << std::endl;

  // The distance between (10, 3.14) and (4, 0.86) should be:
  // ~12.9653
  double p_distance = p_p1.Distance(p_p2);
  double round_p_distance = std::round(p_distance * 10000.0) / 10000.0;
  if (round_p_distance != 12.9653) {
    std::cerr << "The calculated polar coordinate distance was incorrect"
              << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Distance between polar p1 and polar p2 is "
            << p_distance << std::endl;
}
