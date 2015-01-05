// CS 61C Fall 2014 Project 3

#pragma once

float displacementNaive(int dx, int dy);
void calcDepthNaive(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement, size_t* floatOps);
