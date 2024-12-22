/*
 * Copyright ©2024 Yiqing Wang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "Cat.h"

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

string Cat::getCatInfo() const {
  stringstream ss;
  ss <<  "Name: " << name_ << ", Age: " << age_
     << ", Fur Color: " << fur_color_;
  return ss.str();
}

int Cat::get_age() const {
  return age_;
}

std::string Cat::get_name() const {
  return name_;
}

std::string Cat::get_fur_color() const {
  return fur_color_;
}
