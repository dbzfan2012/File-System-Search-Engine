/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 15
 */

// The max buf size
#define BUF 4096

#include <unistd.h>   // for open, close, read, write
#include <string.h>   // for strerror
#include <iostream>   // for sscanf, exting, files
#include <fcntl.h>    // for O_RDONLY

#include "connect.h"  // for GetSocket, Usage

// Prints to standard error how the executable
// should be used, then exits
void Usage();

// Reads from a local file with and outputs its text
// through the local variable readbuf. Returns the
// amount read from the file
int ReadFile(int32_t file, unsigned char *readbuf);

// Given an open socket, a buffer with text, and the amount
// initially read from the buffer, writes the contents of the
// buffer to the socket
int WriteSocket(int socket_fd, unsigned char *readbuf, int res);

int main(int argc, char** argv) {
  // Input checking
  if (argc != 4) {
    Usage();
  }

  // Reads the port number
  unsigned short port = 0;
  if (sscanf(argv[2], "%hu", &port) != 1) {
    Usage();
  }

  // Gets the socket
  int socket_fd = GetSocket(port, argv[1]);
  if (socket_fd == -1) {
    return EXIT_FAILURE;
  }

  // Reads the text at the file path into a buffer
  char *file_path = argv[3];
  unsigned char readbuf[BUF];

  int32_t file = open(file_path, O_RDONLY);
  if (file == -1) {
    std::cerr << "file open error" << std::endl;
    return EXIT_FAILURE;
  }

  while (1) {
    // Reads into readbuf
    int res = ReadFile(file, readbuf);
    if (res <= 0) {
      break;
    }

    // Writes the buffer's contents into the socket
    int res2 = WriteSocket(socket_fd, readbuf, res);
    if (res2 != res) {
      break;
    }
  }
  

  // Clean up
  close(socket_fd);
  close(file);
  return EXIT_SUCCESS;
}

void Usage() {
  std::cerr << "Usage: ex15 [hostname] [port number] [filename]" << std::endl;
  exit(EXIT_FAILURE);
}

int ReadFile(int32_t file, unsigned char *readbuf) {
  int res;

  // Loops through the file, reading into readbuf
  while (1) {
    res = read(file, readbuf, BUF);
    if (res == -1) {
      if (errno == EINTR) {
        continue;
      }
    }
    break;
  }
  return res;
}

int WriteSocket(int socket_fd, unsigned char *readbuf, int res) {
  // Loops through the buffer, writing into the socket
  int written = 0;
  while (1) {
    int wres = write(socket_fd, readbuf, res);
    if (wres == 0) {
      break;
    }
    if (wres == -1) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }
    if (written >= res) {
      break;
    }
    written += wres;
  }
  return written;
}
