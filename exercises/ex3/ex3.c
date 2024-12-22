/*
 * Copyright ©2024 Ethan Kogan. All rights reserved. Please don't steal, thanks.
 * Ethan Kogan
 * dbzfan@uw.edu
 * Exercise 3
 */
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

// The length of the array used for testing
#define LEN 20

// Defines the struct which holds the coordinate
// values for a point in 3D space
typedef struct point3D_st {
    uint16_t x, y, z;
} Point3D;

// Declares the function AllocatePoint3D
Point3D* AllocatePoint3D(uint16_t x, uint16_t y, uint16_t z);

// Declares the function FreePoints3D
void FreePoints3D(Point3D* points[], int len);

// Declares the function FillPoints
void FillPoints(Point3D* points[], int len);

// Declares the function TestPoints
bool TestPoints(Point3D* points[], int len);

int main(int argc, char **argv) {
    // Defines the array holding the 3D points
    // and gives each arbitrary test values
    Point3D* points[LEN];
    // Fills the array with 3D points for testing
    FillPoints(points, LEN);
    // Prints out all the 3D points and assigns an error
    // if the printed values were incorrect
    bool error = TestPoints(points, LEN);
    // Frees the allocated points
    FreePoints3D(points, LEN);
    // Exits unsuccessfully if an error occured during testing
    if (error) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// Allocates a new Point3D object with the given coordinates
Point3D* AllocatePoint3D(uint16_t x, uint16_t y, uint16_t z) {
    Point3D* new_point = (Point3D*) malloc(sizeof(Point3D));
    new_point->x = x;
    new_point->y = y;
    new_point->z = z;
    return new_point;
}

// Frees all the allocated points in a given array of 3D points
void FreePoints3D(Point3D* points[], int len) {
    for (int i = 0; i < len; i++) {
        free(points[i]);
    }
}

// Given an empty array of 3D points and its length,
// adds different points until the array is full
void FillPoints(Point3D* points[], int len) {
    int x = 0;
    int y = 0;
    int z = 0;
    int idx = 0;
    while (len > 0) {
        points[idx] = AllocatePoint3D(x, y, z);
        if (idx % 3 == 0) {
            x++;
        } else if (idx % 3 == 1) {
            y++;
        } else {
            z++;
        }
        idx++;
        len--;
    }
}

// Prints out all the points in given list and tests them
// against the expected values, returning false if the values
// were incorrect
bool TestPoints(Point3D* points[], int len) {
    int x = 0;
    int y = 0;
    int z = 0;
    int idx = 0;
    while (len > 0) {
        printf("The coordinates are (%i, %i, %i)\n", \
               points[idx]->x, points[idx]->y, points[idx]->z);
        if (points[idx]->x != x || points[idx]->y != y || points[idx]->z != z) {
            printf("Incorrect points acquired! Should've been (%i, %i, %i), " \
                   "but was (%i, %i, %i)\n", x, y, z, \
                    points[idx]->x, points[idx]->y, points[idx]->z);
            return false;
        }
        if (idx % 3 == 0) {
            x++;
        } else if (idx % 3 == 1) {
            y++;
        } else {
            z++;
        }
        idx++;
        len--;
    }
    return true;
}
