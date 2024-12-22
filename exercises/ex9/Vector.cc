/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 9
 */
#include <iostream>
#include <sstream>
#include <string>

#include "Vector.h"

namespace vector333 {

Vector::Vector() {
  this->x_ = 0.0;
  this->y_ = 0.0;
  this->z_ = 0.0;
}

Vector::Vector(const double x, const double y, const double z) {
  this->x_ = x;
  this->y_ = y;
  this->z_ = z;
}

Vector::Vector(const Vector &copyme) {
  this->x_ = copyme.x_;
  this->y_ = copyme.y_;
  this->z_ = copyme.z_;
}

Vector::~Vector() {}

Vector &Vector::operator=(const Vector &v) {
  if (this != &v) {
    this->x_ = v.x_;
    this->y_ = v.y_;
    this->z_ = v.z_;
  }
  return *this;
}

Vector &Vector::operator+=(const Vector &v) {
  this->x_ += v.x_;
  this->y_ += v.y_;
  this->z_ += v.z_;
  return *this;
}

Vector &Vector::operator-=(const Vector &v) {
  this->x_ -= v.x_;
  this->y_ -= v.y_;
  this->z_ -= v.z_;
  return *this;
}

Vector operator-(const Vector &v, const Vector &u) {
  Vector tmp(v.get_x() - u.get_x(), v.get_y() - u.get_y(),
              v.get_z() - u.get_z());
  return tmp;
}

Vector operator+(const Vector &v, const Vector &u) {
  Vector tmp(v.get_x() + u.get_x(), v.get_y() + u.get_y(),
              v.get_z() + u.get_z());
  return tmp;
}

double operator*(const Vector &v, const Vector &u) {
  double value = (v.get_x() * u.get_x()) + (v.get_y() * u.get_y()) +
                 (v.get_z() * u.get_z());
  return value;
}

Vector operator*(const Vector &v, const double k) {
  Vector tmp(k * v.get_x(), k * v.get_y(), k * v.get_z());
  return tmp;
}

Vector operator*(const double k, const Vector &v) {
  Vector tmp(k * v.get_x(), k * v.get_y(), k * v.get_z());
  return tmp;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
  out << "(" << v.get_x() << "," << v.get_y() << "," << v.get_z() << ")";
  return out;
}

}  // namespace vector333
