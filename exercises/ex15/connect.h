/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 15
 */

#ifndef CONNECT_H_
#define CONNECT_H_

// Attempts to open a socket with the given port and hostname
// Returns the socket if successful, -1 if failed
int GetSocket(unsigned short port, char *hostname);

// Gets the address info given a hostname and port
// Outputs the struct info and size through parameter
// Returns true if succeeded, false otherwise
bool LookupName(char* name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen);


#endif  // CONNECT_H_
