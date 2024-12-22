/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 2
 */
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// Reads off a given number of bytes from a given address in memory
void PrintBytes(void* mem_addr, int num_bytes) {
    char* read_addr = (char*) mem_addr;
    uint8_t byte = 0;
    printf("The %i%s%p%s", \
           num_bytes, \
           " bytes starting at ", \
           mem_addr, " are: ");

    // Loops through each byte in the address the given number of times
    // and prints them out
    for (int i = 0; i < num_bytes; i++) {
        byte = (uint8_t) *(read_addr + i);
        printf("%02" PRIx8 " ", byte);
    }
    printf("\n");
}

int main(int argc, char **argv) {
  char     char_val = '0';
  int32_t  int_val = 1;
  float    float_val = 1.0;
  double   double_val  = 1.0;

  typedef struct {
    char     char_val;
    int32_t  int_val;
    float    float_val;
    double   double_val;
  } Ex2Struct;

  Ex2Struct struct_val = { '0', 1, 1.0, 1.0 };

  PrintBytes(&char_val, sizeof(char));
  PrintBytes(&int_val, sizeof(int32_t));
  PrintBytes(&float_val, sizeof(float));
  PrintBytes(&double_val, sizeof(double));
  PrintBytes(&struct_val, sizeof(struct_val));

  return EXIT_SUCCESS;
}
