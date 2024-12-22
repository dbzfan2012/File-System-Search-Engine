/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 16
 */

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "listen.h"

// Prints to standard error how the executable
// should be used, then exits
void Usage(char *progname);

// Handles the client side of the program, listens for input
// and prints it all to standard out, then returns. Prints an
// error if something went wrong
void HandleClient(int c_fd, struct sockaddr *addr, size_t addrlen,
                  int sock_family);

int main(int argc, char **argv) {
  // Expect the port number as a command line argument.
  if (argc != 2) {
    Usage(argv[0]);
  }

  int sock_family;
  int listen_fd = Listen(argv[1], &sock_family);
  if (listen_fd <= 0) {
    // We failed to bind/listen to a socket.  Quit with failure.
    std::cerr << "Couldn't bind to any addresses." << std::endl;
    return EXIT_FAILURE;
  }

  // Accept a connection from the client and take in data from it
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    int client_fd = accept(listen_fd,
                           reinterpret_cast<struct sockaddr *>(&caddr),
                           &caddr_len);
    if (client_fd < 0) {
      if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK))
        continue;
      std::cerr << "Failure on accept: " << strerror(errno) << std::endl;
      close(client_fd);
      break;
    }

    HandleClient(client_fd,
                 reinterpret_cast<struct sockaddr *>(&caddr),
                 caddr_len,
                 sock_family);
    close(client_fd);
    break;
  }

  // Close up shop.
  close(listen_fd);
  return EXIT_SUCCESS;
}

void Usage(char *progname) {
  std::cerr << "usage: " << progname << " port" << std::endl;
  exit(EXIT_FAILURE);
}

void HandleClient(int c_fd, struct sockaddr *addr, size_t addrlen,
                  int sock_family) {
  // Reads the data sent by the client to standard out
  while (1) {
    char clientbuf[1024];
    ssize_t res = read(c_fd, clientbuf, 1023);
    if (res == 0) {
      break;
    }

    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;

      std::cout << " [Error on client socket: ";
      std::cout << strerror(errno) << "]" << std::endl;
      break;
    }
    clientbuf[res] = '\0';
    std::cout << clientbuf;
  }

  close(c_fd);
}
