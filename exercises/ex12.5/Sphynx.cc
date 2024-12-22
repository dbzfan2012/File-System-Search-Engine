/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 12.5
 */

#include "Sphynx.h"
#include "Cat.h"

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

string Sphynx::getCatInfo() const {
  stringstream ss;
  ss <<  "Name: " << get_name() << ", Age: " << get_age()
     << ", Fur Color: " << get_fur_color() << ", Message: " << message_;
  return ss.str();
}
