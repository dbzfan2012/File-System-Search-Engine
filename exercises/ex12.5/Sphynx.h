/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 12.5
 */

#ifndef SPHYNX_H_
#define SPHYNX_H_

#include <string>
#include "Cat.h"

class Sphynx : public Cat {
 public:
  Sphynx(const std::string &n, int a, const std::string &fc,
         const std::string &m)
    : Cat(n, a, fc), message_(m) {}

  std::string getCatInfo() const;

 protected:
  std::string name_;
  int age_;
  std::string fur_color_;
  std::string message_;
};

#endif  // SPHYNX_H_
