/*
 * Copyright ©2024 Hannah C. Tang.  All rights reserved.  Permission is
 * hereby granted to the students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fcntl.h>  // for O_RDONLY
#include <memory>   // for unique_ptr

extern "C" {
  #include "libhw2/FileParser.h"
}

#include "./HttpUtils.h"
#include "./FileReader.h"

using std::string;

namespace hw4 {

bool FileReader::ReadFile(string* const contents) {
  string full_file = basedir_ + "/" + fname_;

  // Read the file into memory, and store the file contents in the
  // output parameter "contents."  Be careful to handle binary data
  // correctly; i.e., you probably want to use the two-argument
  // constructor to std::string (the one that includes a length as a
  // second argument).
  //
  // You might find ::ReadFileToString() from HW2 useful here. Remember that
  // this function uses malloc to allocate memory, so you'll need to use
  // free() to free up that memory after copying to the string output
  // parameter.
  //
  // Alternatively, you can use a unique_ptr with a malloc/free
  // deleter to automatically manage this for you; see the comment in
  // HttpUtils.h above the MallocDeleter class for details.

  // STEP 1:

  int size;
  // Check if the path is safe to prevent directory traversal
  if (!IsPathSafe(basedir_, full_file)) {
    return false;
  }

  // Read the file, return false if failed
  char *read = HW2_FILEPARSER_H_::ReadFileToString(full_file.c_str(), &size);
  if (read == NULL) {
    return false;
  }
  free(read);

  // A unique pointer with MallocDeleter to free the contents read to the file
  std::unique_ptr<char, MallocDeleter<char>>
      str(HW2_FILEPARSER_H_::ReadFileToString(full_file.c_str(), &size));
  *contents = std::string(str.get(), size);

  return true;
}

}  // namespace hw4
