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

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE  // For getline

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);

// Processes the queries through the index and doctable created
// by crawling through the given directory. Loops through
// the given queries and prints out the search results
static void ProcessQueries(DocTable* dt, MemIndex* mi);

// Gets the queries and number of queries from standard input
// Takes in the full user-input query, splits it into tokens,
// and adds them to an array of strings outputted through queries
// Returns the number of total queries
static int GetQueries(char* query, char*** queries);

// Frees the various queries allocated through GetQueries
static void FreeQueries(char* query, char** queries, int query_len);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  MemIndex* index;
  DocTable* doc_table;
  char* file_path = argv[1];
  printf("Indexing '%s'\n", file_path);
  if (!CrawlFileTree(file_path, &doc_table, &index)) {
    fprintf(stderr, "Path '%s' is not indexable\n", file_path);
    Usage();
  }

  // Processes the queries within the directory
  ProcessQueries(doc_table, index);

  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable* dt, MemIndex* mi) {
  while (true) {
    char* query = NULL;
    char** queries;
    int query_len;
    size_t len = 0;

    // Gets the user input
    printf("enter query:\n");
    getline(&query, &len, stdin);

    // Checks for ctrl-d input, frees resources, then shuts down
    if (feof(stdin)) {
      printf("shutting down...\n");
      free(query);
      MemIndex_Free(mi);
      DocTable_Free(dt);
      break;
    }

    // Splits the queries from the user input and gets number of queries
    query_len = GetQueries(query, &queries);

    // Searches through the index with the given queries,
    // continues if no result found
    LinkedList* index_search = MemIndex_Search(mi, queries, query_len);
    if (index_search == NULL) {
      FreeQueries(query, queries, query_len);
      continue;
    }

    // Loops through the index, printing the matching files
    // sorted by rank in descending order
    LLIterator* iter = LLIterator_Allocate(index_search);
    while (LLIterator_IsValid(iter)) {
      SearchResult* search;
      LLIterator_Get(iter, (LLPayload_t*) &search);

      char* file = DocTable_GetDocName(dt, search->doc_id);
      printf("  %s (%i)\n", file, search->rank);
      LLIterator_Next(iter);
    }

    LinkedList_Free(index_search, &free);
    LLIterator_Free(iter);
    FreeQueries(query, queries, query_len);
  }
}

static int GetQueries(char* query, char*** queries) {
  char* token;
  char* rest = query;
  LinkedList* word_list = LinkedList_Allocate();

  // First pass: takes the given user input, splits into words by
  // whitespace, sets everything to lowercase, and removes \n from input
  // Appends these words to a linked list
  while ((token = strtok_r(rest, " ", &rest))) {
    for (int i = 0; i < strlen(token); i++) {
      if (token[i] == '\n') {
        token[i] = '\0';
      } else {
        token[i] = tolower(token[i]);
      }
    }
    LinkedList_Append(word_list, token);
  }

  // Allocates an array of strings based on the number of total queries
  int query_len = LinkedList_NumElements(word_list);
  char** new_queries = (char**)malloc(sizeof(char*) * query_len);

  // Loops and removes each element from the linked list, allocating
  // space for it and adding it to the allocated array of strings
  for (int i = 0; i < query_len; i++) {
    char* word;
    LinkedList_Pop(word_list, (LLPayload_t*) &word);
    new_queries[i] = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    snprintf(new_queries[i], strlen(word) + 1, "%s", word);
  }

  // Takes care of freeing the list as well as function outputs
  LinkedList_Free(word_list, &free);
  *queries = new_queries;
  return query_len;
}

static void FreeQueries(char* query, char** queries, int query_len) {
  // Loops through array and frees each of its strings
  for (int i = 0; i < query_len; i++) {
    free(queries[i]);
  }
  free(queries);
  free(query);
}
