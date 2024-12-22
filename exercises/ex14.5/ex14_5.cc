/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 14.5
 */

#include <iostream>  // for cout, endl
#include <map>       // for map
#include <string>    // for string
#include <vector>    // for vector
#include <sstream>   // for stringstream

// The size that the map should be
#define MAP_SIZE 5

// This function takes in a filled map of strings to vectors of strings,
// and prints them to standard out in an organized format
void print_contents(std::map<std::string, std::vector<std::string>> stl_map);

// This function takes in a reference to a map of strings to
// vectors of strings and an amount, and fills up the map with
// this amount number of key/value pairs
void fill_map(std::map<std::string, std::vector<std::string>> *stl_map,
                                                           int amount);

int main(int argc, char** argv) {
  std::map<std::string, std::vector<std::string>> stl_map;

  // Fill up the map
  fill_map(&stl_map, MAP_SIZE);

  // Print out its contents
  print_contents(stl_map);
  return EXIT_SUCCESS;
}

void fill_map(std::map<std::string, std::vector<std::string>> *stl_map,
                                                           int amount) {
  // Loops through, adding vectors to the map
  for (int i = 0; i < amount; i++) {
    std::vector<std::string> vec;

    // Loops through, filling up the vector
    for (int j = 0; j < amount; j++) {
      std::stringstream value;
      value << "Value " << j;
      vec.push_back(value.str());
    }

    // Assigns a key to the map and adds the vector as its value
    std::stringstream key;
    key << "Key " << i;
    (*stl_map).insert({key.str(), vec});
  }
}

void print_contents(std::map<std::string, std::vector<std::string>> stl_map) {
  // Begins iteration over the map
  auto iter = stl_map.begin();
  while (iter != stl_map.end()) {
    std::vector<std::string> curr_vec = iter->second;

    // Adds the vector's contents into an organized stringstream
    std::stringstream vector;
    vector << std::endl << "<";
    for (size_t i = 0; i < curr_vec.size() - 1; i++) {
      vector << curr_vec.at(i) << ", ";
    }
    vector << curr_vec.back() << ">" << std::endl;

    // Prints out the contents of this key/value pair
    std::cout << "Key is: " << iter->first
              << ", Value is: " << vector.str() << std::endl;
    iter++;
  }
}
