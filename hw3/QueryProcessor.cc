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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include <map>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;
using std::map;

namespace hw3 {

// Checks if the given map values match what's in the doc id list. If yes,
// then the rank is updated and returns true. If no, then returns false
bool IsMatch(const DocID_t doc_id, QueryProcessor::QueryResult* result,
                                   list<DocIDElementHeader> doc_id_list);

// Merges the results of two index searches, combining both into
// the passed in initial map
void MergeIndices(map<DocID_t, QueryProcessor::QueryResult>* initial,
                  map<DocID_t, QueryProcessor::QueryResult> previous);

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader* [array_len_];
  itr_array_ = new IndexTableReader* [array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int     rank;    // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;
  map<DocID_t, QueryProcessor::QueryResult> final_map;

  auto previous = final_map;
  bool previous_error = false;
  for (int i = 0; i < array_len_; i++) {
    // Gets the current file's index and doc table readers
    map<DocID_t, QueryProcessor::QueryResult> local_result;
    IndexTableReader* itr = itr_array_[i];
    DocTableReader* dtr = dtr_array_[i];

    DocIDTableReader* docid_reader = itr->LookupWord(query[0]);
    if (docid_reader == nullptr) {
      if (previous.size() == 0) { previous_error = true; }
      delete docid_reader;
      continue;
    }
    // check if returns nullptr
    list<DocIDElementHeader> id_list = docid_reader->GetDocIDList();

    // Adds first word to list
    for (DocIDElementHeader header : id_list) {
      // Get the header's doc id and rank
      DocID_t doc_id = header.doc_id;
      int32_t rank = header.num_positions;

      // Gets the file name based on the doc id
      string doc_name;
      dtr->LookupDocID(doc_id, &doc_name);

      QueryResult result;
      result.rank = rank;
      result.document_name = doc_name;

      local_result.insert({doc_id, result});
    }

    bool invalid = false;
    // Loop through words, remove files from final_result
    // that don't match with every word
    for (size_t idx = 1; idx < query.size(); idx++) {
      string word = query[idx];

      // If the current word is not in the index, then the query
      // was not found at all
      DocIDTableReader* new_docid_reader = itr->LookupWord(word);
      if (new_docid_reader == nullptr) {
        invalid = true;
        delete new_docid_reader;
        break;
      }

      list<DocIDElementHeader> doc_id_list = new_docid_reader->GetDocIDList();
      map<DocID_t, QueryProcessor::QueryResult>::iterator iter =
                                                 local_result.begin();

      // Loops through the words docID list and checks if it
      // matches with the previous words' results
      // Increments rank if matched
      while (iter != local_result.end()) {
        if (!IsMatch(iter->first, &(iter->second), doc_id_list)) {
          auto tmp = iter;
          tmp++;
          local_result.erase(iter);
          iter = tmp;
        } else {
          iter++;
        }
        // If the matches results are none, leaves the loop
        if (local_result.size() == 0) { break; }
      }
      delete new_docid_reader;
    }

    // Current index is invalid, continues
    if (invalid) {
      if (previous.size() == 0) { previous_error = true; }
      delete docid_reader;
      continue;
    }

    // Merges the current index results with the previous results
    if (i > 0 && !previous_error) {
      MergeIndices(&local_result, previous);
    }
    previous_error = false;
    previous = local_result;
    delete docid_reader;
  }

  final_map = previous;
  // Moves values in map to final_result vector
  map<DocID_t, QueryProcessor::QueryResult>::iterator iter = final_map.begin();
  while (iter != final_map.end()) {
    final_result.push_back(iter->second);
    iter++;
  }

  // One query word done, what about more?
  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

bool IsMatch(const DocID_t doc_id, QueryProcessor::QueryResult* result,
                                   list<DocIDElementHeader> doc_id_list) {
  // Iterates through list and checks for matches
  list<DocIDElementHeader>::iterator iter;
  for (iter = doc_id_list.begin(); iter != doc_id_list.end(); iter++) {
    if (iter->doc_id == doc_id) {
      result->rank += iter->num_positions;  // increments rank
      return true;
    }
  }
  return false;  // no match found
}

void MergeIndices(map<DocID_t, QueryProcessor::QueryResult>* initial,
                  map<DocID_t, QueryProcessor::QueryResult> previous) {
  // Add elements in previous into initial
  map<DocID_t, QueryProcessor::QueryResult>::iterator iter = previous.begin();
  while (iter != previous.end()) {
    auto init_element = initial->find(iter->first);

    if (init_element != initial->end()) {
      // Update item in initial element by iter->second.rank
      init_element->second.rank += iter->second.rank;  // increments the rank
      auto tmp_rank = init_element->first;  // creates temporary rank
      auto tmp_result = init_element->second;  // creates temporary rank
      initial->erase(init_element->first);  // removes from map
      initial->insert(
        {tmp_rank, tmp_result});  // adds new rank to map
    } else {
      // Adds to the map
      initial->insert({iter->first, iter->second});
    }
    iter++;
  }
}

}  // namespace hw3
