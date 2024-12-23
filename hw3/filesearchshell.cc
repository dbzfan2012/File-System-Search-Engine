/*
 * Copyright ©2024 Hannah C. Tang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <cstdlib>    // for EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>   // for std::cout, std::cerr, etc.
#include <sstream>    // for stringstream
#include <algorithm>  // for transform

#include "./QueryProcessor.h"

using std::cerr;
using std::endl;

// Error usage message for the client to see
// Arguments:
// - prog_name: Name of the program
static void Usage(char* prog_name);

// Gets the queries and number of queries from standard input
// Takes in the full user-input query, splits it into tokens,
// and adds them to an array of strings outputted through queries
// Returns the number of total queries
static vector<string> GetQueries(string query);

// Loops through user input with the given processed queries
// to output matching documents and rank
static void ProcessQueries(hw3::QueryProcessor* qp);

int main(int argc, char** argv) {
  if (argc < 2) {
    Usage(argv[0]);
  }

  // Gets all indices into a list
  std::list<string> idx_list;
  for (int i = 1; i < argc; i++) {
    idx_list.push_back(argv[i]);
  }

  // Processes the queries
  hw3::QueryProcessor qp(idx_list);
  ProcessQueries(&qp);

  return EXIT_SUCCESS;
}

static void ProcessQueries(hw3::QueryProcessor* qp) {
  while (1) {
    std::string query;
    std::vector<string> queries;
    std::cout << "Enter query:" << endl;
    getline(std::cin, query);

    // Checks for ctrl-d input
    if (feof(stdin)) {
      break;
    }

    // Gets the user's query
    queries = GetQueries(query);
    vector<hw3::QueryProcessor::QueryResult> res = qp->ProcessQuery(queries);

    // Outputs "no results" if no matches found
    if (res.size() == 0) {
      std::cout << "  [no results]" << endl;
    }

    // Outputs the files and ranks
    for (auto result : res) {
      std::cout << "  " << result.document_name
                << " (" << result.rank << ")" << endl;
    }
  }
}

static void Usage(char* prog_name) {
  cerr << "Usage: " << prog_name << " [index files+]" << endl;
  exit(EXIT_FAILURE);
}

static vector<string> GetQueries(std::string query) {
  std::stringstream ss(query);
  std::vector<string> queries;
  std::string word;

  // Loops through the words in the stringstream, adding them
  // to the queries list and making them lowercase
  while (ss >> word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    queries.push_back(word);
  }
  return queries;
}
