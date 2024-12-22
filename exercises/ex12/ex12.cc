/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 12
 */

#include <iostream>       // for stdout, EXIT_SUCCESS, EXIT_FAILURE
#include <bits/stdc++.h>  // for map

int main(int argc, char **argv) {
  // Input checking
  if (argc != 2) {
    std::cout << "Usage: ./ex12 file_path" << std::endl;
    return EXIT_FAILURE;
  }

  // Reads file
  std::string file_path = argv[1];
  std::ifstream indata;
  indata.open(file_path);
  if (!indata) {
    std::cerr << "The file could not be opened" << std::endl;
    return EXIT_FAILURE;
  }

  // Reads through file and adds the words and
  // number of occurrences to the map
  std::string data;
  std::map<std::string, int> map;
  while (!indata.eof()) {
    indata >> data;
    if (map.find(data) == map.end()) {
      map[data] = 0;
    }
    map[data] += 1;
  }

  // Prints out map pairs from a sorted vector
  std::map<std::string, int>::iterator iter;
  for (iter = map.begin(); iter != map.end(); iter++) {
    std::cout << iter->first << " " << iter->second << std::endl;
  }

  indata.close();
}
