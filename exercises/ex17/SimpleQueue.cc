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

#include "SimpleQueue.h"

SimpleQueue::SimpleQueue() {
  size = 0;
  front = nullptr;
  end = nullptr;
  pthread_mutex_init(&queue_lock, NULL);
}

SimpleQueue::~SimpleQueue() {
  pthread_mutex_lock(&queue_lock);
  while (front != nullptr) {
    node *next = front->next;
    delete front;
    front = next;
  }
  pthread_mutex_unlock(&queue_lock);
  pthread_mutex_destroy(&queue_lock);
}

void SimpleQueue::Enqueue(string item) {
  pthread_mutex_lock(&queue_lock);
  node *new_node = new node();
  new_node->next = nullptr;
  new_node->item = item;
  if (end != nullptr) {
    end->next = new_node;
  } else  {
    front = new_node;
  }
  end = new_node;
  size++;
  pthread_mutex_unlock(&queue_lock);
}

bool SimpleQueue::Dequeue(string *result) {
  pthread_mutex_lock(&queue_lock);
  if (size == 0) {
    pthread_mutex_unlock(&queue_lock);
    return false;
  }
  *result = front->item;
  node *next = front->next;
  delete front;
  if (end == front) {
    end = front = next;
  } else {
    front = next;
  }
  size--;
  pthread_mutex_unlock(&queue_lock);
  return true;
}

int SimpleQueue::Size() const {
  return size;
}

bool SimpleQueue::IsEmpty() const {
  return size == 0;
}
