/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 10
 */
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Vector.h"

int main(int argc, char **argv) {
  // test constructors and stream output
  vector333::Vector* v1 = new vector333::Vector();
  std::cout << "default Vector, should be (0,0,0): " << *v1 << std::endl;
  vector333::Vector* v2 = new vector333::Vector(1, 2, 3);
  std::cout << "Vector with initial values, should be (1,2,3): "
            << *v2 << std::endl;
  vector333::Vector* v3 = new vector333::Vector(*v2);
  std::cout << "Vector from copy constructor, should be (1,2,3): "
            << *v3 << std::endl;

  // test assignment (including chaining)
  vector333::Vector* v4 = new vector333::Vector(3.1, -2.5, 2.7);
  *v1 = *v3 = *v4;
  std::cout << "Vector assignment, should have three copies"
            << " of (3.1,-2.5,2.7):\n";
  std::cout << "  " << *v1 << "  " << *v3 << "  " << *v4 << std::endl;

  // test updating assignment (including chaining)
  std::cout << "Updating assignment, should have two copies"
            << " of (4.1,-0.5,5.7):\n";
  *v1 = *v3 += *v2;
  std::cout << " " << *v1 << " = " <<  *v3 << std::endl;
  std::cout << "Updating assignment, should have two copies"
            << " of (3.1,-2.5,2.7):\n";
  *v1 = *v3 -= *v2;
  std::cout << " " <<  *v1 << " = " <<  *v3 << std::endl;

  // element-wise arithmetic
  std::cout << "Arithmetic:" << std::endl;
  std::cout << "  " << *v1 << " + " << *v2 << " = " << *v1+*v2 << std::endl;
  std::cout << "  " << *v1 << " - " << *v2 << " = " << *v1-*v2 << std::endl;

  // dot product
  std::cout << "Dot product: (a,b,c) * (x,y,z) = (ax+by+cz)" << std::endl;
  std::cout << "  " << *v2 << " * " << *v2 << " = " << *v2**v2 << std::endl;

  // scalar product
  std::cout << "Scalar product: (a,b,c) * k = (ak,bk,ck)" << std::endl;
  std::cout << "  "       << *v1 << " * 2 = " << *v1*2 << std::endl;
  std::cout << "  2 * " << *v1 <<       " = " << 2**v1 << std::endl;

  delete v1;
  delete v2;
  delete v3;
  delete v4;
  return EXIT_SUCCESS;
}
