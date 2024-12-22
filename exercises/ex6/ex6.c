/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 6
 */

#include <stdlib.h>   // for EXIT_SUCCESS
#include <stdio.h>    // for printf

int main(int argc, char **argv) {
  FILE *file;
  char c;

  // Take the filename from command line argument
  if (argc != 2) {
    fprintf(stderr, "usage: ./ex6 (file)\n");
    return EXIT_FAILURE;
  }

  // Opens the input file, throws error if not found
  file = fopen(argv[1], "rb");
  if (file == NULL) {
    fprintf(stderr, "%s -- ", argv[1]);
    perror("fopen for read failed");
    return EXIT_FAILURE;
  }

  // Gets the number of bytes and moves cursor to end of file
  fseek(file, 0, SEEK_END);
  int size = ftell(file);

  // Reads each byte backwards and prints it out
  int idx = 1;
  while (size > 0) {
    fseek(file, -idx, SEEK_END);
    fread(&c, 1, 1, file);
    fprintf(stderr, "%c", c);
    size--;
    idx++;
  }

  // Test to see if we encountered an error while reading.
  if (ferror(file)) {
    perror("fread failed");
    return EXIT_FAILURE;
  }
  fclose(file);

  return EXIT_SUCCESS;
}
