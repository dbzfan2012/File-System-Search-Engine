/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 9
 */
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Vector.h"

int main(int argc, char **argv) {
  // Create test vectors
  vector333::Vector v(10, 0, -20), u(-1, 2, -9), x(13, 0, 0), z(-5.5, 2, 7.5);

  // Test that << operand formats vector correctly
  // Output of default should be "(0,0,0)"
  vector333::Vector test_default;
  std::stringstream input;
  input << test_default;
  std::string output = input.str();
  if (output.compare("(0,0,0)") != 0) {
    fprintf(stderr, "Stream output operator works incorrectly\n");
    return EXIT_FAILURE;
  }

  // Test values were assigned correctly, and that getters work
  // v should be (10, 0, -20)
  if (v.get_x() != 10 || v.get_y() != 0 || v.get_z() != -20) {
    fprintf(stderr, "Values not correctly assigned to vectors\n");
    return EXIT_FAILURE;
  }

  // Outputs the currently unedited vectors
  std::cout << "Constructed vector v with values " << v << std::endl;
  std::cout << "Constructed vector u with values " << u << std::endl;
  std::cout << "Constructed vector x with values " << x << std::endl;
  std::cout << "Constructed vector z with values " << z << std::endl;

  // Test default constructor
  vector333::Vector zero;
  if (zero.get_x() != 0 || zero.get_y() != 0 || zero.get_z() != 0) {
    fprintf(stderr, "Values not correctly assigned to default vector\n");
    return EXIT_FAILURE;
  }
  std::cout << "Constructed default vector with values " << zero << std::endl;

  // Test that assignment operator works
  u = v;
  vector333::Vector new_vector = x;
  if (u.get_x() != v.get_x() || u.get_y() != v.get_y() ||
      u.get_z() != v.get_z() ||
      new_vector.get_x() != x.get_x() || new_vector.get_y() != x.get_y() ||
      new_vector.get_z() != x.get_z()) {
    fprintf(stderr, "Assignment operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "Vector u was assigned to v, it is equal to " << u << std::endl;
  std::cout << "Vector new_vector was assigned to x, it is equal to "
            << new_vector << std::endl;

  // Test that addition update assignment works
  // x is (13, 0, 0), u is (10, 0, -20)
  // after x += u, x is (23, 0, -20)
  x += u;
  if (x.get_x() != 23 || x.get_y() != 0 || x.get_z() != -20) {
    fprintf(stderr, "Addition update operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "Vector x after x += u is " << x << std::endl;

  // Test that subtraction assignment works
  // x is (23, 0, -20), z is (-5.5, 2, 7.5)
  // after x -= z, x is (28.5, -2, -27.5)
  x -= z;
  if (x.get_x() != 28.5 || x.get_y() != -2 || x.get_z() != -27.5) {
    fprintf(stderr, "Subtraction update operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "Vector x after x -= z is " << x << std::endl;

  // Test that addition operator works
  // x is (28.5, -2, -27.5)
  // after x + x, add is (57, -4, -55)
  vector333::Vector add = x + x;
  if (add.get_x() != 57 || add.get_y() != -4 || add.get_z() != -55) {
    fprintf(stderr, "Addition operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "Vector add is equal to x + x, which is " << add << std::endl;

  // Test that subtraction operator works
  // x is (28.5, -2, -27.5), v is (10, 0, -20)
  // after x - v, sub is (18.5, -2, -7.5)
  vector333::Vector sub = x - v;
  if (sub.get_x() != 18.5 || sub.get_y() != -2 || sub.get_z() != -7.5) {
    fprintf(stderr, "Subtraction operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "Vector sub is equal to x - v, which is " << sub << std::endl;

  // Test that dot product operator works
  // u is (10, 0, -20), z is (-5.5, 2, 7.5)
  // dot is 10 * -5.5 + 0 * 2 + -20 * 7.5 = -205
  double dot = u * z;
  if (dot != -205) {
    fprintf(stderr, "Dot product operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "The dot product of u and v is " << dot << std::endl;

  // Test that multiplication operator is overloaded,
  // and works with scalar multiplication
  // z is (-5.5, 2, 7.5)
  // scaled is (-55, 20, 75)
  vector333::Vector scaled = 10 * z;
  vector333::Vector scaled_2 = z * 10;
  if (scaled.get_x() != -55 || scaled.get_y() != 20 || scaled.get_z() != 75 ||
      scaled_2.get_x() != -55 || scaled_2.get_y() != 20 ||
      scaled_2.get_z() != 75) {
    fprintf(stderr, "Scalar operator works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "The vector z multiplied by a scalar of 10 is "
            << scaled << std::endl;

  // Test copy constructor
  vector333::Vector to_copy(10, 20, 30);
  vector333::Vector copied(to_copy);
  if (copied.get_x() != to_copy.get_x() || copied.get_y() != to_copy.get_y() ||
      copied.get_z() != to_copy.get_z()) {
    fprintf(stderr, "Copy constructor works incorrectly\n");
    return EXIT_FAILURE;
  }
  std::cout << "The copied vector of " << to_copy << " is "
            << copied << std::endl;

  return EXIT_SUCCESS;
}
