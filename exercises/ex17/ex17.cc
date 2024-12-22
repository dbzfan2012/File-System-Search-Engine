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

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>

#include "SimpleQueue.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;

const int NUM_PIES = 6;                 // # pies created by each producer
static SimpleQueue queue;               // queue of pies
static unsigned int seed = time(NULL);  // initialize random sleep time
static pthread_mutex_t write_lock;      // mutex for cout

// A thread wrapper for running producer
void *thread_start_producer(void *arg);

// A thread wrapper for running consumer
void *thread_start_consumer(void *arg);

// Thread safe print that prints the given str on a line
void thread_safe_print(string str) {
  pthread_mutex_lock(&write_lock);
  // Only one thread can hold the lock at a time, making it safe to
  // use cout. If we didn't lock before using cout, the order of things
  // put into the stream could be mixed up.
  cout << str << endl;
  pthread_mutex_unlock(&write_lock);
}

// Produces NUM_PIES pies of the given type
// You should NOT modify this method at all
void producer(string pie_type) {
  for (int i = 0; i < NUM_PIES; i++) {
    queue.Enqueue(pie_type);
    thread_safe_print(pie_type + " pie ready!");
    int sleep_time = rand_r(&seed) % 500 + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
}

// Eats 2 * NUM_PIES pies
// You should NOT modify this method at all
void consumer() {
  for (int i = 0; i < NUM_PIES * 2; i++) {
    bool successful = false;
    string pie_type;
    while (!successful) {
      while (queue.IsEmpty()) {
        // Sleep for a bit and then check again
        int sleep_time = rand_r(&seed) % 800 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
      successful = queue.Dequeue(&pie_type);
    }
    thread_safe_print(pie_type + " pie eaten!");
  }
}

int main(int argc, char **argv) {
  pthread_mutex_init(&write_lock, NULL);
  // Your task: Make the two producers and the single consumer
  // all run concurrently (hint: use pthreads)
  pthread_t apple_producer_thr;
  pthread_t blackberry_producer_thr;
  pthread_t consumer_thr;

  // Starts the apple producer thread
  string apple = "Apple";
  if (pthread_create(&apple_producer_thr, nullptr, &thread_start_producer,
                                  reinterpret_cast<void *>(&apple)) != 0) {
    return EXIT_FAILURE;
  }

  // Starts the blackberry producer thread
  string blackberry = "Blackberry";
  if (pthread_create(&blackberry_producer_thr, nullptr, &thread_start_producer,
                                  reinterpret_cast<void *>(&blackberry)) != 0) {
    return EXIT_FAILURE;
  }

  // Starts the consumer thread
  if (pthread_create(&consumer_thr, nullptr, &thread_start_consumer,
                                                    nullptr) != 0) {
    return EXIT_FAILURE;
  }

  // Join all the threads
  pthread_join(consumer_thr, nullptr);
  pthread_join(blackberry_producer_thr, nullptr);
  pthread_join(apple_producer_thr, nullptr);

  pthread_mutex_destroy(&write_lock);
  return EXIT_SUCCESS;
}

void *thread_start_producer(void *arg) {
  string *pie_type = reinterpret_cast<string *>(arg);
  producer(*pie_type);
  return nullptr;
}

void *thread_start_consumer(void *arg) {
  consumer();
  return nullptr;
}
