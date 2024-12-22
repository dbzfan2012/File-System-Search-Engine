/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 10
 */
#include <iostream>
#include <sstream>
#include <string>

#include "Vector.h"

namespace vector333 {

Vector::Vector() {
  coordinates_ = new double[3];
  coordinates_[0] = 0;
  coordinates_[1] = 0;
  coordinates_[2] = 0;
}

Vector::Vector(const double x, const double y, const double z) {
  coordinates_ = new double[3];
  coordinates_[0] = x;
  coordinates_[1] = y;
  coordinates_[2] = z;
}

Vector::Vector(const Vector &copyme) {
  coordinates_ = new double[3];
  this->coordinates_[0] = copyme.coordinates_[0];
  this->coordinates_[1] = copyme.coordinates_[1];
  this->coordinates_[2] = copyme.coordinates_[2];
}

Vector::~Vector() {
  delete [] this->coordinates_;
}

Vector &Vector::operator=(const Vector &v) {
  if (this != &v) {
    this->coordinates_[0] = v.coordinates_[0];
    this->coordinates_[1] = v.coordinates_[1];
    this->coordinates_[2] = v.coordinates_[2];
  }
  return *this;
}

Vector &Vector::operator+=(const Vector &v) {
  this->coordinates_[0] += v.coordinates_[0];
  this->coordinates_[1] += v.coordinates_[1];
  this->coordinates_[2] += v.coordinates_[2];
  return *this;
}

Vector &Vector::operator-=(const Vector &v) {
  this->coordinates_[0] -= v.coordinates_[0];
  this->coordinates_[1] -= v.coordinates_[1];
  this->coordinates_[2] -= v.coordinates_[2];
  return *this;
}

Vector operator-(const Vector &v, const Vector &u) {
  Vector tmp(v.coordinates_[0] - u.coordinates_[0],
             v.coordinates_[1] - u.coordinates_[1],
             v.coordinates_[2] - u.coordinates_[2]);
  return tmp;
}

Vector operator+(const Vector &v, const Vector &u) {
  Vector tmp(v.coordinates_[0] + u.coordinates_[0],
             v.coordinates_[1] + u.coordinates_[1],
             v.coordinates_[2] + u.coordinates_[2]);
  return tmp;
}

double operator*(const Vector &v, const Vector &u) {
  double value = (v.coordinates_[0] * u.coordinates_[0]) +
                 (v.coordinates_[1] * u.coordinates_[1]) +
                 (v.coordinates_[2] * u.coordinates_[2]);
  return value;
}

Vector operator*(const Vector &v, const double k) {
  Vector tmp(v.coordinates_[0] * k,
             v.coordinates_[1] * k,
             v.coordinates_[2] * k);
  return tmp;
}

Vector operator*(const double k, const Vector &v) {
  Vector tmp(k * v.coordinates_[0],
             k * v.coordinates_[1],
             k * v.coordinates_[2]);
  return tmp;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
  out << "(" << v.coordinates_[0] << "," << v.coordinates_[1]
      << "," << v.coordinates_[2] << ")";
  return out;
}

}  // namespace vector333
