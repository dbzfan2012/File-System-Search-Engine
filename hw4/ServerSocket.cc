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

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

extern "C" {
  #include "libhw1/CSE333.h"
}

namespace hw4 {

// Outputs the string containing printable representation of
// IP address and port in IPv4 or IPv6 abstract to new function
void assignClient(struct sockaddr *client_sock_addr,
                  std::string* const client_addr,
                  uint16_t* const client_port);

// Outputs strings representing the server's IP address and
// the server's DNS name or its IP address if there's no valid DNS name
void assignServer(int client_fd, int sock_family_,
                  std::string* const server_addr,
                  std::string* const server_dns_name);

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int* const listen_fd) {
  // Set the hints for the socket
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = ai_family;
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "INADDR_ANY"
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  // Get the address info from the given port
  std::string port_str = std::to_string(port_);
  struct addrinfo *result;
  int res = getaddrinfo(nullptr, port_str.c_str(), &hints, &result);

  if (res != 0) {
    return false;
  }

  // Listen and bind the socket
  int listen_fd_tmp = -1;
  for (struct addrinfo *rp = result; rp != nullptr; rp = rp->ai_next) {
    listen_fd_tmp = socket(rp->ai_family,
                       rp->ai_socktype,
                       rp->ai_protocol);
    if (listen_fd_tmp == -1) {
      listen_fd_tmp = -1;
      continue;
    }

    int optval = 1;
    setsockopt(listen_fd_tmp, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof(optval));

    if (bind(listen_fd_tmp, rp->ai_addr, rp->ai_addrlen) == 0) {
      sock_family_ = ai_family;
      break;
    }

    close(listen_fd_tmp);
    listen_fd_tmp = -1;
  }

  // Free the info and throw an error if necessary
  freeaddrinfo(result);
  if (listen_fd_tmp == -1) {
    return false;
  }
  if (listen(listen_fd_tmp, SOMAXCONN) != 0) {
    return false;
  }
  listen_sock_fd_ = listen_fd_tmp;
  *listen_fd = listen_fd_tmp;

  return true;
}

bool ServerSocket::Accept(int* const accepted_fd,
                          std::string* const client_addr,
                          uint16_t* const client_port,
                          std::string* const client_dns_name,
                          std::string* const server_addr,
                          std::string* const server_dns_name) const {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // Accepts the connection from a client
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    int client_fd = accept(listen_sock_fd_,
                          reinterpret_cast<struct sockaddr *>(&caddr),
                          &caddr_len);

    if (client_fd < 0) {
      if ((errno == EINTR) || (errno == EAGAIN)/* || (errno == EWOULDBLOCK)*/) {
        continue;
      }
      return false;
    }

    struct sockaddr *client_sock_addr =
                    reinterpret_cast<struct sockaddr *>(&caddr);

    // Outputs the file descriptor for new client connection
    *accepted_fd = client_fd;

    assignClient(client_sock_addr, client_addr, client_port);

    // Outputs String representing DNS name of client or
    // string representation of IP adress if no valid DNS name
    char hostname[1024];
    getnameinfo(client_sock_addr, caddr_len, hostname, 1024, nullptr, 0, 0);
    *client_dns_name = std::string(hostname);

    assignServer(client_fd, sock_family_, server_addr, server_dns_name);
    break;
  }
  return true;
}

void assignClient(struct sockaddr *client_sock_addr,
                  std::string* const client_addr,
                  uint16_t* const client_port) {
  if (client_sock_addr->sa_family == AF_INET) {
    char addr_string[INET_ADDRSTRLEN];
    struct sockaddr_in *in4 =
          reinterpret_cast<struct sockaddr_in *>(client_sock_addr);
    inet_ntop(AF_INET, &(in4->sin_addr), addr_string, INET_ADDRSTRLEN);
    *client_addr = std::string(addr_string);
    *client_port = ntohs(in4->sin_port);
  } else {
    char addr_string[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *in6 =
          reinterpret_cast<struct sockaddr_in6 *>(client_sock_addr);
    inet_ntop(AF_INET6, &(in6->sin6_addr), addr_string, INET6_ADDRSTRLEN);
    *client_addr = std::string(addr_string);
    *client_port = ntohs(in6->sin6_port);
  }
}

void assignServer(int client_fd, int sock_family_,
                  std::string* const server_addr,
                  std::string* const server_dns_name) {
  char hname[1024];
  hname[0] = '\0';
  if (sock_family_ == AF_INET) {
    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);
    char addrbuf[INET_ADDRSTRLEN];
    getsockname(client_fd, (struct sockaddr *) &server, &server_len);
    inet_ntop(AF_INET, &server.sin_addr, addrbuf, INET_ADDRSTRLEN);

    getnameinfo((const struct sockaddr *) &server,
                server_len, hname, 1024, nullptr, 0, 0);

    *server_addr = std::string(addrbuf);
    *server_dns_name = std::string(hname);
  } else {
    struct sockaddr_in6 server;
    socklen_t server_len = sizeof(server);
    char addrbuf[INET6_ADDRSTRLEN];
    getsockname(client_fd, (struct sockaddr *) &server, &server_len);
    inet_ntop(AF_INET6, &server.sin6_addr, addrbuf, INET6_ADDRSTRLEN);

    getnameinfo((const struct sockaddr *) &server,
                server_len, hname, 1024, nullptr, 0, 0);

    *server_addr = std::string(addrbuf);
    *server_dns_name = std::string(hname);
  }
}

}  // namespace hw4
