/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {

    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;

}

int calc_ESD(unsigned char *left, unsigned char *right, int currPix1, int currPix2, int image_width, int image_height, int feature_width, int feature_height){
	int currColumn1 = currPix1 % image_width;  //currPix1
	int currRow1 = currPix1 % image_height;
	int currColumn2 = currPix2 % image_width;
	int currRow2 = currPix2 % image_width;
	int currTotal = 0;

	for (int column = 0; column < feature_height; column++){
		for (int row = 0; row < feature_width; row++){
			if ((currColumn1 + column - 2 >= image_width)|| (currColumn1 + column - 2 < 0)){
				if ((currColumn2 + column - 2 >= image_width)){
					continue;
				} else if (currColumn2 + column - 2 < 0){
					continue;
				}
				if (currRow2 + row - 2 >= image_height){
					continue;
				} else if (currRow2 + row - 2 < 0){
					continue;
				} else {				
				currTotal = currTotal + ((0 - right[currPix2]) * (0 - right[currPix2]));
				continue;
				}
			}
			if ((currColumn2 + column - 2 >= image_width) || (currColumn2 + column - 2 < 0)){
				if (currRow1 + row - 2 >= image_height){
					continue;
				} else if (currRow1 + row - 2 < 0){
					continue;
				} else {					
				currTotal = currTotal + (left[currPix1] - 0) * (left[currPix1] - 0);
				} //just added this curly brace was not here before /////////
			} else if (currColumn2 + column - 2 < 0){
				currTotal = currTotal + (left[currPix1] - 0) * (left[currPix1] - 0);
			}
			if (currRow1 + row - 2 >= image_height || (currRow1 + row - 2 < 0)){
				if ((currRow1 + row - 2 >= image_height)){
					continue;
				} else if (currRow1 + row - 2 < 0){
					continue;
				}
				currTotal = currTotal + ((0 - right[currPix2]) * (0 - right[currPix2]));
				continue;
			}			
			if (currRow2 + row - 2 >= image_height){
				currTotal = currTotal + (left[currPix1] - 0) * (left[currPix1] - 0);
				continue;
			} else if (currRow2 + row - 2 < 0){
				currTotal = currTotal + (left[currPix1] - 0) * (left[currPix1] - 0);
				continue;
			}				
			currPix1 += row - 2;
			currPix2 += row - 2;
			currTotal = currTotal + ((left[currPix1] - right[currPix2]) * (left[currPix1] - right[currPix2]));
		}
		currPix1 += image_width * (column - 2);
		currPix2 += image_width * (column - 2);
	}
	return currTotal;
}




void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {

    /* YOUR CODE HERE */
    ///////// Iterating through searchspace ///////////
	for (int pixel = 0; pixel < image_width * image_height - 1; pixel++) {
		calc_ESD(left, right, pixel, pixel, image_width, image_height, feature_width, feature_height);  //8 parameters
	}
}

