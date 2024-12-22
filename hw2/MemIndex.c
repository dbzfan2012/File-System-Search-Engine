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

#include "./MemIndex.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "libhw1/HashTable.h"
#include "libhw1/LinkedList.h"


///////////////////////////////////////////////////////////////////////////////
// Internal-only helpers
//
// These functions use the MI_ prefix instead of the MemIndex_ prefix,
// to indicate that they should not be considered part of the MemIndex's
// public API.

// Comparator usable by LinkedList_Sort(), which implements an increasing
// order by rank over SearchResult's.  If the caller is interested in a
// decreasing sort order, they should invert the return value.
static int MI_SearchResultComparator(LLPayload_t e1, LLPayload_t e2) {
  SearchResult* sr1 = (SearchResult*) e1;
  SearchResult* sr2 = (SearchResult*) e2;

  if (sr1->rank > sr2->rank) {
    return 1;
  } else if (sr1->rank < sr2->rank) {
    return -1;
  } else {
    return 0;
  }
}

// Deallocator usable by LinkedList_Free(), which frees a list of
// of DocPositionOffset_t.  Since these offsets are stored inline (ie, not
// malloc'ed), there is nothing to do in this function.
static void MI_NoOpFree(LLPayload_t ptr) { }

// Deallocator usable by HashTable_Free(), which frees a LinkedList of
// DocPositionOffset_t (ie, our posting list).  We use these LinkedLists
// in our WordPostings.
static void MI_PostingsFree(HTValue_t ptr) {
  LinkedList* list = (LinkedList*) ptr;
  LinkedList_Free(list, &MI_NoOpFree);
}

// Deallocator used by HashTable_Free(), which frees a WordPostings.  A
// MemIndex consists of a HashTable of WordPostings (ie, this is the top-level
// structure).
static void MI_ValueFree(HTValue_t ptr) {
  WordPostings* wp = (WordPostings*) ptr;

  free(wp->word);
  HashTable_Free(wp->postings, &MI_PostingsFree);

  free(wp);
}

///////////////////////////////////////////////////////////////////////////////
// MemIndex implementation

MemIndex* MemIndex_Allocate(void) {
  // Happily, HashTables dynamically resize themselves, so we can start by
  // allocating a small hashtable.
  HashTable* index = HashTable_Allocate(16);
  Verify333(index != NULL);
  return index;
}

void MemIndex_Free(MemIndex* index) {
  HashTable_Free(index, &MI_ValueFree);
}

int MemIndex_NumWords(MemIndex* index) {
  return HashTable_NumElements(index);
}

void MemIndex_AddPostingList(MemIndex* index, char* word, DocID_t doc_id,
                             LinkedList* postings) {
  HTKey_t key = FNVHash64((unsigned char*) word, strlen(word));
  HTKeyValue_t mi_kv, postings_kv, unused;
  WordPostings* wp;
  if (!HashTable_Find(index, key, &mi_kv)) {
    wp = (WordPostings*)malloc(sizeof(WordPostings));
    wp->word = word;

    // Allocates the postings table to 16, will dynamically
    // increase as table load factor increases
    wp->postings = HashTable_Allocate(16);
    mi_kv.key = key;
    mi_kv.value = wp;
    HashTable_Insert(index, mi_kv, &mi_kv);

  } else {
    wp = (WordPostings*) mi_kv.value;

    // Ensure we don't have hash collisions (two different words that hash to
    // the same key, which is very unlikely).
    Verify333(strcmp(wp->word, word) == 0);

    free(word);
  }

  // Verify that this document is not already in the posting list.
  Verify333(!HashTable_Find(wp->postings, doc_id, &postings_kv));

  postings_kv.key = doc_id;
  postings_kv.value = postings;
  HashTable_Insert(wp->postings, postings_kv, &unused);
}

LinkedList* MemIndex_Search(MemIndex* index, char* query[], int query_len) {
  LinkedList* ret_list;
  HTKeyValue_t kv;
  WordPostings* wp;
  HTKey_t key;
  int i;

  // If the user provided us with an empty search query, return NULL
  // to indicate failure.
  if (query_len == 0) {
    return NULL;
  }

  key = FNVHash64((unsigned char*) query[0], strlen(query[0]));
  if (!HashTable_Find(index, key, &kv)) {
    return NULL;
  }

  wp = (WordPostings*) kv.value;
  HTIterator* iter = HTIterator_Allocate(wp->postings);
  ret_list = LinkedList_Allocate();

  // Iterates through word postings, adding all matching
  // documents to the return list
  while (HTIterator_IsValid(iter)) {
    SearchResult* search = (SearchResult*)malloc(sizeof(SearchResult));
    Verify333(HTIterator_Get(iter, &kv));
    search->doc_id = kv.key;
    search->rank = LinkedList_NumElements(kv.value);
    LinkedList_Append(ret_list, search);
    HTIterator_Next(iter);
  }

  HTIterator_Free(iter);

  if (query_len == 1) {
    LinkedList_Sort(ret_list, false, &MI_SearchResultComparator);
    return ret_list;
  }

  for (i = 1; i < query_len; i++) {
    LLIterator* ll_it;
    int j, num_docs;

    key = FNVHash64((unsigned char*) query[i], strlen(query[i]));
    if (!HashTable_Find(index, key, &kv)) {
      LinkedList_Free(ret_list, &free);
      return NULL;
    }

    wp = (WordPostings*) kv.value;
    ll_it = LLIterator_Allocate(ret_list);
    Verify333(ll_it != NULL);
    num_docs = LinkedList_NumElements(ret_list);

    // Iterates through the return list, removing non-matching documents
    // when query is not found
    for (j = 0; j < num_docs; j++) {
      SearchResult* prev;
      LLIterator_Get(ll_it, (LLPayload_t) &prev);
      if (!HashTable_Find(wp->postings, prev->doc_id, &kv)) {
        LLIterator_Remove(ll_it, &free);
      } else {
        prev->rank += LinkedList_NumElements(kv.value);
        LLIterator_Next(ll_it);
      }
    }
    LLIterator_Free(ll_it);

    // If there are no documents left in our result list, free retlist and return NULL.
    if (LinkedList_NumElements(ret_list) == 0) {
      LinkedList_Free(ret_list, (LLPayloadFreeFnPtr)free);
      return NULL;
    }
  }

  // Sort the result list by rank and return it to the caller.
  LinkedList_Sort(ret_list, false, &MI_SearchResultComparator);
  return ret_list;
}