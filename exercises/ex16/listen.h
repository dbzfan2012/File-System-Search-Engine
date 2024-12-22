/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 16
 */

#ifndef LISTEN_H_
#define LISTEN_H_

// Creates and returns a listening file descriptor given a
// port number and a socket. Returns the listener on success,
// otherwise returns -1
int Listen(char *portnum, int *sock_family);

#endif  // LISTEN_H_
