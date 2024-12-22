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

// Feature test macro enabling strdup (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include "./FileParser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "./MemIndex.h"


///////////////////////////////////////////////////////////////////////////////
// Constants and declarations of internal helper functions

#define ASCII_UPPER_BOUND 0x7F

// Frees a WordPositions.positions's payload, which is just a
// DocPositionOffset_t.
static void NoOpFree(LLPayload_t payload) { }

// Frees a WordPositions struct.
static void FreeWordPositions(HTValue_t payload) {
  WordPositions* pos = (WordPositions*) payload;
  LinkedList_Free(pos->positions, &NoOpFree);
  free(pos->word);
  free(pos);
}

// Add a normalized word and its byte offset into the WordPositions HashTable.
static void AddWordPosition(HashTable* tab, char* word,
                            DocPositionOffset_t pos);

// Parse the passed-in string into normalized words and insert into a HashTable
// of WordPositions structures.
static void InsertContent(HashTable* tab, char* content);


///////////////////////////////////////////////////////////////////////////////
// Publically-exported functions

char* ReadFileToString(const char* file_name, int* size) {
  struct stat file_stat;
  char* buf;
  int result, fd;
  ssize_t num_read;
  size_t left_to_read;

  if (stat(file_name, &file_stat) == -1) {
    perror("File read failed");
    return NULL;
  }

  if (!S_ISREG(file_stat.st_mode)) {
    perror("File is not a 'regular file'");
    return NULL;
  }

  fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    perror("File failed to open");
    return NULL;
  }

  num_read = file_stat.st_size;
  buf = (char*)malloc((num_read + 1)*sizeof(char));

  left_to_read = num_read;
  while (left_to_read > 0) {
    result = read(fd, buf + (num_read - left_to_read), left_to_read);
    if (result == -1) {
      if (errno != EINTR) {
        perror("File failed to finish reading");
        return NULL;
      }
      continue;
    } else if (result == 0) {
      break;
    }
    left_to_read -= result;
  }

  close(fd);
  *size = file_stat.st_size - left_to_read;

  // Null-terminate the string.
  buf[*size] = '\0';
  return buf;
}

HashTable* ParseIntoWordPositionsTable(char* file_contents) {
  HashTable* tab;
  int i, file_len;

  if (file_contents == NULL) {
    return NULL;
  }

  file_len = strlen(file_contents);
  if (file_len == 0) {
    return NULL;
  }

  for (i = 0; i < file_len; i++) {
    if (file_contents[i] == '\0' ||
        (unsigned char) file_contents[i] > ASCII_UPPER_BOUND) {
      free(file_contents);
      return NULL;
    }
  }

  tab = HashTable_Allocate(32);
  Verify333(tab != NULL);

  // Loop through the file, splitting it into words and inserting a record for
  // each word.
  InsertContent(tab, file_contents);

  // If we found no words, return NULL instead of a zero-sized hashtable.
  if (HashTable_NumElements(tab) == 0) {
    HashTable_Free(tab, &FreeWordPositions);
    tab = NULL;
  }

  free(file_contents);
  return tab;
}

void FreeWordPositionsTable(HashTable* table) {
  HashTable_Free(table, &FreeWordPositions);
}


///////////////////////////////////////////////////////////////////////////////
// Internal helper functions

static void InsertContent(HashTable* tab, char* content) {
  char* cur_ptr = content;
  char* word_start = content;

  bool isWord = false;
  int offset = 0;
  int len = strlen(content);
  for (int i = 0; i < len; i++) {
    if (isalpha(*cur_ptr) && !isWord) {
      // Starts the word if it's a letter
      word_start = cur_ptr = content + i;
      isWord = true;
      offset = i;
    } else if (!isalpha(*cur_ptr) && isWord) {
      // Ends the word upon reaching non-letter
      isWord = false;
      *cur_ptr = '\0';
      AddWordPosition(tab, word_start, offset);
    }
    // Lowercases the current character if it's in a word
    // and increments the pointer
    if (isWord) { *cur_ptr = tolower(*cur_ptr); }
    cur_ptr++;
  }
}

static void AddWordPosition(HashTable* tab, char* word,
                            DocPositionOffset_t pos) {
  HTKey_t hash_key;
  HTKeyValue_t kv;
  WordPositions* wp;

  // Hash the string.
  hash_key = FNVHash64((unsigned char*) word, strlen(word));

  // Have we already encountered this word within this file?  If so, it's
  // already in the hashtable.
  if (HashTable_Find(tab, hash_key, &kv)) {
    wp = (WordPositions*) kv.value;

    // Ensure we don't have hash collisions (two different words that hash to
    // the same key, which is very unlikely).
    Verify333(strcmp(wp->word, word) == 0);

    LinkedList_Append(wp->positions, (LLPayload_t) (int64_t) pos);
  } else {
    wp = (WordPositions*)malloc(sizeof(WordPositions));
    wp->positions = LinkedList_Allocate();

    // Allocates the word and copies input to it
    wp->word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    snprintf(wp->word, strlen(word) + 1, "%s", word);

    // Adds the position to the positions table and the word to the hash table
    LinkedList_Append(wp->positions, (LLPayload_t) (int64_t) pos);

    kv.key = hash_key;
    kv.value = wp;

    HashTable_Insert(tab, kv, &kv);
  }
}
