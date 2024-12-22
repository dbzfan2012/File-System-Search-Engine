/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 10
 */
#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

namespace vector333 {

// This class contains information about a vector in 3D space
class Vector {
 public:
    // Constructors
    Vector();                               // Default Constructor
    Vector(const double x, const double y,  // Constructor
           const double z);
    Vector(const Vector& copyme);           // Copy Constructor
    ~Vector();                              // Destructor

    // Update and equality operators for vectors
    Vector &operator=(const Vector &v);
    Vector &operator+=(const Vector &v);
    Vector &operator-=(const Vector &v);

 private:
    double* coordinates_;

  // Following operators given the friend permissions to access vector array

  // Overloaded operators for vectors
  friend Vector operator-(const Vector &v, const Vector &u);
  friend Vector operator+(const Vector &v, const Vector &u);

  // Dot product vector operator
  friend double operator*(const Vector &v, const Vector &u);

  // Scalar product vector operator, ensures it is commutative
  friend Vector operator*(const Vector &v, const double k);
  friend Vector operator*(const double k, const Vector &v);

  // Overloaded output stream operator for a vector
  friend std::ostream &operator<<(std::ostream &out, const Vector &v);
};
}  // namespace vector333

#endif  // VECTOR_H_
