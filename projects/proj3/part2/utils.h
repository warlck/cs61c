// CS 61C Fall 2014 Project 3

#pragma once

typedef struct
{
	unsigned char *data;
	int width;
	int height;
} Image;

typedef unsigned char byte;

#define FLOATING_POINT_TOLERANCE 0.0001

// Call this function when a memory allocation fails: exits the program, returning -1.
void allocationFailed();

// Fills dst with n random floats between 0 and 1.
void fillRandomFloat(float* dst, size_t n);

// Checks for floating point equality.
bool floatsWithinTolerance(float a, float b);

// Converts a float array to a normalized char array.
void floatToChar(unsigned char* dst, const float* const src, size_t n, int maximumDisplacement);

// Converts a char array into a float array.
void charToFloat(float* dst, const unsigned char* const src, size_t n);

// Loads a bitmap file.
Image loadImage(char *filename);

// Prints a 2D float array.
void printFloatImage(const float *data, int width, int height);

// Prints a 2D char array.
void printImage(const unsigned char *data, int width, int height);

// Saves a grayscale bitmap image.
void saveImage(char *filename, const unsigned char *data, int width, int height);

// Saves a color bitmap image.
void saveImageWithDepth(char *filename, const unsigned char *data, const unsigned char *depth, int width, int height, int feature_width, int feature_height);

