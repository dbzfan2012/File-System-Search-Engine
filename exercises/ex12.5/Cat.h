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

#ifndef CAT_H_
#define CAT_H_

#include <string>

class Cat {
 public:
  Cat(const std::string &n, int a, const std::string &fc)
    : name_(n), age_(a), fur_color_(fc) { }

  std::string getCatInfo() const;
  int get_age() const;
  std::string get_name() const;
  std::string get_fur_color() const;

 protected:
  std::string name_;
  int age_;
  std::string fur_color_;
};

#endif  // CAT_H_
