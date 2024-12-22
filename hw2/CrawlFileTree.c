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

#include "./CrawlFileTree.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "libhw1/CSE333.h"
#include "./DocTable.h"
#include "./FileParser.h"

//////////////////////////////////////////////////////////////////////////////
// Internal helper functions and constants
//////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_DIR_ENTRIES 512   // max items (files + subdirs) in a directory
#define MAX_PATHNAME_LENGTH 1024  // max len of a directory item's path + name

struct entry_st {
  char* path_name;
  bool is_dir;
};

// Return the relative ordering of two strings, according to the signature
// required by "man 3 qsort".
int alphasort(const void* v1, const void* v2) {
  struct entry_st* e1 = (struct entry_st*) v1,
    *e2 = (struct entry_st*) v2;
  return strncmp(e1->path_name, e2->path_name, MAX_PATHNAME_LENGTH);
}

// Recursively descend into the passed-in directory, looking for files and
// subdirectories.  Any encountered files are processed via HandleFile(); any
// subdirectories are recursively handled by HandleDir().
static void HandleDir(char* dir_path, DIR* d,
                      DocTable** doc_table, MemIndex** index);

// Read and parse the specified file, then inject it into the MemIndex.
static void HandleFile(char* file_path, DocTable** doc_table, MemIndex** index);


//////////////////////////////////////////////////////////////////////////////
// Externally-exported functions
//////////////////////////////////////////////////////////////////////////////

bool CrawlFileTree(char* root_dir, DocTable** doc_table, MemIndex** index) {
  struct stat root_stat;
  DIR* rd;

  // Verify we got some valid args.
  if (root_dir == NULL || doc_table == NULL || index == NULL) {
    return false;
  }

  // Verify that rootdir is a directory.
  if (stat((char*) root_dir, &root_stat) == -1) {
    // We got some kind of error stat'ing the file. Give up
    // and return an error.
    return false;
  }
  if (!S_ISDIR(root_stat.st_mode)) {
    // It isn't a directory, so give up.
    return false;
  }

  // Try to open the directory using opendir().  If we fail, (e.g., we don't
  // have permissions on the directory), return a failure. ("man 3 opendir")
  rd = opendir(root_dir);
  if (rd == NULL) {
    return false;
  }

  // Since we're able to open the directory, allocate our objects.
  *doc_table = DocTable_Allocate();
  Verify333(*doc_table != NULL);
  *index = MemIndex_Allocate();
  Verify333(*index != NULL);

  // Begin the recursive handling of the directory.
  HandleDir(root_dir, rd, doc_table, index);

  // All done.  Release and/or transfer ownership of resources.
  Verify333(closedir(rd) == 0);
  return true;
}


//////////////////////////////////////////////////////////////////////////////
// Internal helper functions
//////////////////////////////////////////////////////////////////////////////

static void HandleDir(char* dir_path, DIR* d, DocTable** doc_table,
                      MemIndex** index) {
  // We make two passes through the directory.  The first gets the list of
  // all the metadata necessary to process its entries; the second iterates
  // does the actual recursive descent.
  int entries_capacity = 16;
  struct entry_st* entries = (struct entry_st*)
      malloc(sizeof(struct entry_st) * entries_capacity);
  Verify333(entries != NULL);

  int i;
  int path_name_len;
  struct dirent* dirent;
  struct stat st;

  int num_entries;

  // First pass, to populate the "entries" list of item metadata.
  for (i = 0; (dirent = readdir(d)) != NULL; i++) {

    // Checks if "." or ".." directories, decrements index and continues if so
    if (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, "..")) {
      i--;
      continue;
    }

    //
    // Record the name and directory status.
    //

    // Resize the entries array if it's too small.
    if (i == entries_capacity) {
      entries_capacity *= 2;
      entries = (struct entry_st*)
        realloc(entries, sizeof(struct entry_st) * entries_capacity);
      Verify333(entries != NULL);
    }

    path_name_len = strlen(dir_path) + 1 + strlen(dirent->d_name) + 1;
    entries[i].path_name = (char*) malloc(path_name_len * sizeof(char));
    Verify333(entries[i].path_name != NULL);
    if (dir_path[strlen(dir_path)-1] == '/') {
      // No need to add an additional '/'.
      snprintf(entries[i].path_name, path_name_len,
               "%s%s", dir_path, dirent->d_name);
    } else {
      // We do need to add an additional '/'.
      snprintf(entries[i].path_name, path_name_len,
               "%s/%s", dir_path, dirent->d_name);
    }

    if (stat(entries[i].path_name, &st) == 0) {
      if (S_ISREG(st.st_mode)) {
        // Handled as a file
        entries[i].is_dir = false;
      } else if (S_ISDIR(st.st_mode)) {
        // Handled as a directory
        entries[i].is_dir = true;
      }
    }
  }  // end iteration over directory contents ("first pass").

  // Sort the directory's metadata alphabetically.
  num_entries = i;
  qsort(entries, num_entries, sizeof(struct entry_st), &alphasort);

  // Second pass, processing the now-sorted directory metadata.
  for (i = 0; i < num_entries; i++) {
    if (!entries[i].is_dir) {
      HandleFile(entries[i].path_name, doc_table, index);
    } else {
      DIR* sub_dir = opendir(entries[i].path_name);
      if (sub_dir != NULL) {
        HandleDir(entries[i].path_name, sub_dir, doc_table, index);
        closedir(sub_dir);
      }
    }

    // Free the memory we'd allocated for the entries.
    free(entries[i].path_name);
  }
  free(entries);
}

static void HandleFile(char* file_path, DocTable** doc_table,
                        MemIndex** index) {
  int file_len = 0;
  HashTable* tab = NULL;
  DocID_t doc_id;
  HTIterator* it;

  char* buf = ReadFileToString(file_path, &file_len);
  tab = ParseIntoWordPositionsTable(buf);
  if (tab == NULL) {
    free(tab);
    return;
  }

  doc_id = DocTable_Add(*doc_table, file_path);

  // Loop through the newly-built hash table.
  it = HTIterator_Allocate(tab);
  Verify333(it != NULL);
  while (HTIterator_IsValid(it)) {
    WordPositions* wp;
    HTKeyValue_t kv;

    HTIterator_Remove(it, &kv);
    wp = (WordPositions*) kv.value;

    MemIndex_AddPostingList(*index, wp->word, doc_id, wp->positions);

    free(wp);
  }
  HTIterator_Free(it);

  FreeWordPositionsTable(tab);
}
