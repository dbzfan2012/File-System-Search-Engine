/*
 * Copyright ©2024 Hannah c. Tang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef SIMPLEQUEUE_H_
#define SIMPLEQUEUE_H_

#include <string>

using std::string;

// A simple FIFO queue class that stores strings
// Your task: Make SimpleQueue thread safe!
class SimpleQueue {
 public:
  SimpleQueue();

  ~SimpleQueue();

  // Enqueues the given item
  void Enqueue(string item);

  // Dequeues the item at the front of the queue
  // and stores it at the location pointed to by result.
  // Returns true if there was an item to return and
  // false if the queue was empty.
  bool Dequeue(string *result);

  // Returns the size of the queue
  int Size() const;

  // Returns true if the queue if empty, false otherwise
  bool IsEmpty() const;

 private:
  struct node {
    string item;
    node *next;
  } *front, *end;
  int size;
  mutable pthread_mutex_t queue_lock;
};  // class SimpleQueue

#endif  // SIMPLEQUEUE_H_
