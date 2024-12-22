/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 7
 */

#include <stdio.h>     // for sscanf, fprintf, perror
#include <stdlib.h>    // for malloc, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdint.h>    // for int32_t
#include <fcntl.h>     // for open, O_RDONLY
#include <unistd.h>    // for read, close
#include <string.h>    // for strlen, strcpy, strcat
#include <dirent.h>    // for opendir, readdir, closedir
#include <errno.h>     // for EINTR

// Prints out the given error message with perror and exits
void error(char message[]);

// Combines the directory path and file name into one allocated file path
char* create_path(char directory[], char file[]);

// Opens the file at the given filepath and writes all its bytes to stdout
void ReadFile(char file_path[]);

int main(int argc, char** argv) {
  DIR *dir;
  struct dirent *dir_files;

  // Input checking (only one argument)
  if (argc != 2) {
    fprintf(stderr, "Usage: ./ex7 directory_path\n");
    return EXIT_FAILURE;
  }

  // Opens the directory, throws error if unsuccessful
  if ((dir = opendir(argv[1])) == NULL) {
    fprintf(stderr, "Usage: ./ex7 directory_path\n");
    error("Open directory failed");
  }

  // Identifies every file in the directory, opens and reads the .txt files
  while ((dir_files = readdir(dir)) != NULL) {
    if (strstr(dir_files->d_name, ".txt") != NULL) {
      char *file_path = create_path(argv[1], dir_files->d_name);
      ReadFile(file_path);
      free(file_path);
    }
  }

  // Closes the directory, throws error if unsuccessful
  if (closedir(dir) == -1) {
    fprintf(stderr, "Usage: ./ex7 directory_path\n");
    error("Close directory failed");
  }
  return EXIT_SUCCESS;
}

void error(char message[]) {
  perror(message);
  exit(EXIT_FAILURE);
}

char* create_path(char directory[], char file[]) {
  // Calculates the total length of the file path
  int total_len = strlen(directory) + strlen(file) + strlen("/") + 1;

  // Allocates the file path and throws an error if failed
  char *file_path = (char *) malloc(total_len * sizeof(char));
  if (file_path == NULL) {
    free(file_path);
    error("allocate failed");
  }

  // Appends file elements to allocated file path, throws an error if failed
  if (snprintf(file_path, total_len, "%s%s%s", directory, "/", file) < 0) {
    free(file_path);
    error("string append failed");
  }
  return file_path;
}

void ReadFile(char file_path[]) {
  int32_t file, len, bytes_left, result;

  // Open input file
  file = open(file_path, O_RDONLY);
  if (file == -1) {
    free(file_path);
    error("open failed");
  }

  // Finds the length of the file in bytes,
  // adds 1 for the null terminator
  len = lseek(file, 0, SEEK_END) + 1;
  lseek(file, 0, SEEK_SET);

  // Allocates a buffer and writes all the bytes read to it,
  // throwing an error if unsuccessful
  char *buf = (char *)malloc(len*sizeof(char));
  bytes_left = len;
  while (bytes_left > 0) {
    result = read(file, buf + (len - bytes_left), bytes_left);
    if (result == -1) {
      if (errno != EINTR) {
        error("read failed");
      }
      continue;
    } else if (result == 0) {
      break;
    }
    bytes_left -= result;
  }
  buf[len - bytes_left] = '\0';

  // Writes the buffer to stdout, throws error if unsuccessful
  if (fwrite(buf, 1, len, stdout) < len) {
    error("fwrite failed");
  }

  // Frees the buffer and closes the file, throwing
  // an error if unsuccessful
  free(buf);
  if (close(file) == -1) {
    error("Failed to close file");
  }
}
