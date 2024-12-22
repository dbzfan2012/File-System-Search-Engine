/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 15
 */

#include <assert.h>  // for assert
#include <netdb.h>   // for sockets

#include <string.h>  // for strerror, memset
#include <iostream>  // for cerr, endl

#include "connect.h"

int GetSocket(unsigned short port, char *hostname) {
  struct sockaddr_storage addr;
  size_t addrlen;
  if (!LookupName(hostname, port, &addr, &addrlen)) {
    std::cerr << "Usage: ex15 [hostname] [port number] [filename]" << std::endl;
    return -1;
  }

  // Create the socket
  int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    std::cerr << "socket() failed: " << strerror(errno) << std::endl;
    return -1;
  }

  // Connect the socket to the remote host
  int connect_res = connect(socket_fd,
                    reinterpret_cast<sockaddr *>(&addr),
                    addrlen);

  if (connect_res == -1) {
    std::cerr << "connect() failed: " << strerror(errno) << std::endl;
    return -1;
  }

  return socket_fd;
}

bool LookupName(char* name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen) {
  struct addrinfo hints, *results;
  int retval;

  // Sets hints to all 0s, then sets specific desired settings
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Do the lookup by invoking getaddrinfo()
  if ((retval = getaddrinfo(name, nullptr, &hints, &results)) != 0) {
    std::cerr << "getaddrinfo failed: ";
    std::cerr << gai_strerror(retval) << std::endl;
    return false;
  }

  // Set the port in the first result
  if (results->ai_family == AF_INET) {
    struct sockaddr_in *ipv4addr =
          reinterpret_cast<struct sockaddr_in *>(results->ai_addr);
    ipv4addr->sin_port = htons(port);
  } else if (results->ai_family == AF_INET6) {
    struct sockaddr_in6 *ipv6addr =
          reinterpret_cast<struct sockaddr_in6 *>(results->ai_addr);
    ipv6addr->sin6_port = htons(port);
  } else {
    std::cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
    std::cerr << std::endl;
    freeaddrinfo(results);
    return false;
  }

  // Return the first result
  assert(results != nullptr);
  memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
  *ret_addrlen = results->ai_addrlen;

  // Clean up
  freeaddrinfo(results);
  return true;
}
