/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 9
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

    // Getter functions for x, y, and z coordinates
    double get_x() const {return x_;}
    double get_y() const {return y_;}
    double get_z() const {return z_;}

    // Update and equality operators for vectors
    Vector &operator=(const Vector &v);
    Vector &operator+=(const Vector &v);
    Vector &operator-=(const Vector &v);

 private:
    double x_, y_, z_;
};

  // Overloaded operators for vectors
  Vector operator-(const Vector &v, const Vector &u);
  Vector operator+(const Vector &v, const Vector &u);

  // Dot product vector operator
  double operator*(const Vector &v, const Vector &u);

  // Scalar product vector operator, ensures it is commutative
  Vector operator*(const Vector &v, const double k);
  Vector operator*(const double k, const Vector &v);

  // Overloaded output stream operator for a vector
  std::ostream &operator<<(std::ostream &out, const Vector &v);
}  // namespace vector333

#endif  // VECTOR_H_
