/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 1
 */
#include <stdlib.h>
#include <stdio.h>

// Length of the array
#define LEN 12

// This method copies the elements of a given array into a new array
// and sorts them in ascending order as they are added
void copyAndSort(__int64_t original[], __int64_t new[], int len) {
    int curr;
    int prev;
    for (int i = 0; i < len; i++) {
        // Adds each element to the new array
        __int64_t current = *(original + i);
        *(new + i) = current;

        // Uses Insertion Sort to sort the array
        if (i > 0) {
            curr = i;
            prev = curr - 1;
            while (current < *(new + prev) && prev >= 0) {
                *(new + curr) = *(new + prev);
                *(new + prev) = current;
                prev--;
                curr--;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    __int64_t original[] = {3, 2, -5, 7, 17, 42, 6, 333, 7, 8, -8, 6};
    __int64_t new[LEN];

    // Copy the original array into a new array and sort it
    copyAndSort(original, new, LEN);

    // Print out all the elements of the new array
    for (int i = 0; i < LEN; i++) {
        printf("%li%s", new[i], " ");
    }
    printf("\n");
    return 0;
}
