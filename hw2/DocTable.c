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
#include "./DocTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "libhw1/HashTable.h"

#define HASHTABLE_INITIAL_NUM_BUCKETS 2

// This structure represents a DocTable; it contains two hash tables, one
// mapping from document id to document name, and one mapping from
// document name to document id.
struct doctable_st {
  HashTable* id_to_name;  // mapping document id to document name
  HashTable* name_to_id;  // mapping document name to document id
  DocID_t    max_id;      // max doc ID allocated so far
};

DocTable* DocTable_Allocate(void) {
  DocTable* dt = (DocTable*) malloc(sizeof(DocTable));
  Verify333(dt != NULL);

  dt->id_to_name = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->name_to_id = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->max_id = 1;  // we reserve max_id = 0 for the invalid docID

  return dt;
}

void DocTable_Free(DocTable* table) {
  Verify333(table != NULL);

  HashTable_Free(table->id_to_name, &free);
  HashTable_Free(table->name_to_id, &free);

  free(table);
}

int DocTable_NumDocs(DocTable* table) {
  Verify333(table != NULL);
  return HashTable_NumElements(table->id_to_name);
}

DocID_t DocTable_Add(DocTable* table, char* doc_name) {
  char* doc_copy;
  DocID_t* doc_id;
  HTKeyValue_t kv, old_kv;

  Verify333(table != NULL);

  kv.key = FNVHash64((unsigned char*) doc_name, strlen(doc_name));
  // Finds the key in the table, returns docID if already exists
  if (HashTable_Find(table->name_to_id, kv.key, &old_kv)) {
    return *((DocID_t*) old_kv.value);
  }

  // Allocates string and copies doc name to it
  doc_copy = (char*)malloc(sizeof(char) * (strlen(doc_name) + 1));
  snprintf(doc_copy, strlen(doc_name) + 1, "%s", doc_name);
  doc_id = (DocID_t*)malloc(sizeof(DocID_t));
  kv.value = doc_id;

  *doc_id = table->max_id;
  table->max_id++;

  HTKeyValue_t id_to_name;
  id_to_name.key = *doc_id;
  id_to_name.value = doc_copy;
  HashTable_Insert(table->id_to_name, id_to_name, &id_to_name);

  HTKeyValue_t name_to_id;
  name_to_id.key = FNVHash64((unsigned char*) doc_copy, strlen(doc_copy));
  name_to_id.value = doc_id;
  HashTable_Insert(table->name_to_id, name_to_id, &name_to_id);

  return *doc_id;
}

DocID_t DocTable_GetDocID(DocTable* table, char* doc_name) {
  HTKey_t key;
  HTKeyValue_t kv;
  DocID_t* res;

  Verify333(table != NULL);
  Verify333(doc_name != NULL);

  key = FNVHash64((unsigned char*) doc_name, strlen(doc_name));
  if (HashTable_Find(table->name_to_id, key, &kv)) {
    res = kv.value;
    return *res;
  }
  return INVALID_DOCID;
}

char* DocTable_GetDocName(DocTable* table, DocID_t doc_id) {
  HTKeyValue_t kv;

  Verify333(table != NULL);
  Verify333(doc_id != INVALID_DOCID);

  if (HashTable_Find(table->id_to_name, doc_id, &kv)) {
    return kv.value;
  }
  return NULL;
}

HashTable* DT_GetIDToNameTable(DocTable* table) {
  Verify333(table != NULL);
  return table->id_to_name;
}

HashTable* DT_GetNameToIDTable(DocTable* table) {
  Verify333(table != NULL);
  return table->name_to_id;
}
