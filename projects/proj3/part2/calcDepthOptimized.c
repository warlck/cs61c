// CS 61C Fall 2014 Project 3

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

// include OpenMP
#if !defined(_MSC_VER)
#include <pthread.h>
#endif
#include <omp.h> 

#include "calcDepthOptimized.h"
#include "calcDepthNaive.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

//just added, change these top functions to not incorporate _mm_dp_ps, do more work
//also remember to copy caldDepthOptimized.c to proj2 folder in hive

/*
void squaredDifference1(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_dp_ps(vdiff, vdiff, 0b11110001));  //second instruction does dot product
} */


void squaredDifference4(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff, vdiff));  //second instruction does dot product
}

void squaredDifference8(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff1 = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	__m128 vdiff2 = _mm_sub_ps(_mm_loadu_ps(leftPtr+4), _mm_loadu_ps(rightPtr+4));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff1, vdiff1));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff2, vdiff2));
}

void squaredDifference12(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff1 = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	__m128 vdiff2 = _mm_sub_ps(_mm_loadu_ps(leftPtr+4), _mm_loadu_ps(rightPtr+4));
	__m128 vdiff3 = _mm_sub_ps(_mm_loadu_ps(leftPtr+8), _mm_loadu_ps(rightPtr+8));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff1, vdiff1));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff2, vdiff2));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff3, vdiff3));
}

void squaredDifference16(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff1 = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	__m128 vdiff2 = _mm_sub_ps(_mm_loadu_ps(leftPtr+4), _mm_loadu_ps(rightPtr+4));
	__m128 vdiff3 = _mm_sub_ps(_mm_loadu_ps(leftPtr+8), _mm_loadu_ps(rightPtr+8));
	__m128 vdiff4 = _mm_sub_ps(_mm_loadu_ps(leftPtr+12), _mm_loadu_ps(rightPtr+12));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff1, vdiff1));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff2, vdiff2));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff3, vdiff3));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff4, vdiff4));
}

void squaredDifference20(__m128 *vecSquaredDifference, float *leftPtr, float *rightPtr)
{
	__m128 vdiff1 = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
	__m128 vdiff2 = _mm_sub_ps(_mm_loadu_ps(leftPtr+4), _mm_loadu_ps(rightPtr+4));
	__m128 vdiff3 = _mm_sub_ps(_mm_loadu_ps(leftPtr+8), _mm_loadu_ps(rightPtr+8));
	__m128 vdiff4 = _mm_sub_ps(_mm_loadu_ps(leftPtr+12), _mm_loadu_ps(rightPtr+12));
	__m128 vdiff5 = _mm_sub_ps(_mm_loadu_ps(leftPtr+16), _mm_loadu_ps(rightPtr+16));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff1, vdiff1));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff2, vdiff2));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff3, vdiff3));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff4, vdiff4));
	*vecSquaredDifference = _mm_add_ps(*vecSquaredDifference, _mm_mul_ps(vdiff5, vdiff5));
}

void calcDepthGeneral(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	//Iterate over every pixel in the left image
	#pragma omp parallel for firstprivate(imageHeight, imageWidth, maximumDisplacement, featureWidth, featureHeight, left, right)  //could I add stop and tail here?
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			//Set depth map to zero if a complete feature does not fit around this pixel
			if ((y < featureHeight) || (y >= imageHeight - featureHeight) || (x < featureWidth) || (x >= imageWidth - featureWidth))  //modify conditional here
			{
				depth[y * imageWidth + x] = 0;
				continue;
			}

			//Initialize variables for searching for nearest feature
			float minimumSquaredDifference = -1;
			int minimumDy = 0;
			int minimumDx = 0;

			//Iterate over all features in the right image in the search space defined by the current pixel
			for (int dy = -maximumDisplacement; dy <= maximumDisplacement; dy++)  //first move this check up here by doing &&
			{
				if (y + dy - featureHeight < 0 || y + dy + featureHeight >= imageHeight)
				{
					continue;
				}
				for (int dx = -maximumDisplacement; dx <= maximumDisplacement; dx++) //change <= to != maximumDisplacemnt + 1, and repeat for
					//other ones !!!!!!!!!!!!!
				{
					//Don't consider this feature in the right image if it doesn't fit
					if (x + dx - featureWidth < 0 || x + dx + featureWidth >= imageWidth)
					{
						continue;
					}

					//Initialize squared distance between this current feature in the right image and the fixed feature in the left image
					//float squaredDifference = 0;
					float squaredDifference = 0;
					__m128 vecSquaredDifference = _mm_setzero_ps();

					//Iterate over every pixel in the feature in the right imgage
					for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
					{
						float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
						float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;

						int stop = -featureWidth+((2*featureWidth+1)/4)*4;

						for (int boxX = -featureWidth; boxX < stop; boxX+=4)
						{
							//Compute the squraed difference between the coordinates and add to the total squared distance for this feature
							__m128 diff = _mm_sub_ps(_mm_loadu_ps(leftPtr), _mm_loadu_ps(rightPtr));
							vecSquaredDifference = _mm_add_ps(vecSquaredDifference, _mm_dp_ps(diff, diff, 0b11111111));

							leftPtr += 4;
							rightPtr += 4;
						}
						
						//Tail case
						float diff1, diff2, diff3;
						int tail = (2*featureWidth+1)%4;
						//added stuff
					/*	int pad_size = (featureWidth + 2) * (featureHeight + 2);
						float padded[pad_size];  //added line
						memset(padded, 0.0, pad_size);
						for (int i = 0; i < featureHeight; i++) {
							for (int j = 0; j < featureWidth; j++) {
								padded[] = ;
							}
						} */
						switch (tail) //must pad with tail case
						{
							case 1:
								diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
								squaredDifference += diff1*diff1;
								break;
							case 2:
								diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
								diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
								squaredDifference += diff1*diff1 + diff2*diff2;
								break;
							case 3:
								diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
								diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
								diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
								squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
							default:
								break;
						}
					}

					float result[4];
					_mm_storeu_ps(result, vecSquaredDifference);
					squaredDifference += result[0];
					//printf("<%f, %f, %f, %f>\n", result[0], result[1], result[2], result[3]);

					//Check if current nearest feature
					if ((minimumSquaredDifference > squaredDifference) || ((minimumSquaredDifference == squaredDifference) && (dx*dx+dy*dy < minimumDx*minimumDx+minimumDy*minimumDy)) || (minimumSquaredDifference == -1))
					{
						//Set the minimum squared difference and minimum x and y displacements
						minimumSquaredDifference = squaredDifference;
						minimumDx = dx;
						minimumDy = dy;
					}
				}
			}

			//Update depth map
			if (minimumSquaredDifference != -1)  //modify here
			{
				if (maximumDisplacement == 0)
				{
					//depth[y * imageWidth + x] = displacementNaive(minimumDx, minimumDy);
					depth[y * imageWidth + x] = 0;
				}
				 else
				{
					depth[y * imageWidth + x] = displacementNaive(minimumDx, minimumDy);
				} 
			}
			else
			{
				depth[y * imageWidth + x] = 0;
			}
		}
	}
}

void calcDepthOptimized(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	//printf("Width = %d\n", ((2*featureWidth+1)/4)*4);
	//Iterate over every pixel in the left image

	int width = ((2*featureWidth+1)/4)*4;
	if (width > 16)
	{
		calcDepthGeneral(depth, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);
		return;
	}
	//was here before
	//int stop = -featureWidth+width;
	//int tail = (2*featureWidth+1)%4;

	#pragma omp parallel for firstprivate(imageHeight, imageWidth, maximumDisplacement, featureHeight, featureWidth, left, right) //took out stop and tail
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			//Set depth map to zero if a complete feature does not fit around this pixel
			if ((y < featureHeight) || (y >= imageHeight - featureHeight) || (x < featureWidth) || (x >= imageWidth - featureWidth))
			{
				depth[y * imageWidth + x] = 0;
				continue;
			}

			//Initialize variables for searching for nearest feature
			float minimumSquaredDifference = -1;
			int minimumDy = 0;
			int minimumDx = 0;

			//Iterate over all features in the right image in the search space defined by the current pixel
			for (int dy = -maximumDisplacement; dy <= maximumDisplacement; dy++)
			{
				if (y + dy - featureHeight < 0 || y + dy + featureHeight >= imageHeight)
				{
					continue;
				}
				for (int dx = -maximumDisplacement; dx <= maximumDisplacement; dx++)
				{
					//Don't consider this feature in the right image if it doesn't fit
					if (x + dx - featureWidth < 0 || x + dx + featureWidth >= imageWidth)
					{
						continue;
					}

					//Initialize squared distance between this current feature in the right image and the fixed feature in the left image
					//float squaredDifference = 0;
					float squaredDifference = 0;
					__m128 vecSquaredDifference = _mm_setzero_ps();
					//was here before
					//float *leftPtr, *rightPtr;
					//float diff1, diff2, diff3;
					//Iterate over every pixel in the feature in the right imgage
					if (width == 8)
					{
						for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
						{
							float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
							float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;
							float diff1, diff2, diff3;
							int stop = -featureWidth+width;
							int tail = (2*featureWidth+1)%4;

							squaredDifference8(&vecSquaredDifference, leftPtr, rightPtr);

							switch (tail)  //pad matrix here
							{
								case 1:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									squaredDifference += diff1*diff1;
									break;
								case 2:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									squaredDifference += diff1*diff1 + diff2*diff2;
									break;
								case 3:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
									squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
								default:
									break;
							}
						}
					}
					else if (width == 4)
					{
						for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
						{
							float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
							float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;
							float diff1, diff2, diff3;
							int stop = -featureWidth+width;
							int tail = (2*featureWidth+1)%4;

							squaredDifference4(&vecSquaredDifference, leftPtr, rightPtr);

							switch (tail)
							{
								case 1:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									squaredDifference += diff1*diff1;
									break;
								case 2:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									squaredDifference += diff1*diff1 + diff2*diff2;
									break;
								case 3:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
									squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
								default:
									break;
							}
						}
					}
					else if (width == 16)
					{
						for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
						{
							float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
							float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;
							float diff1, diff2, diff3;
							int stop = -featureWidth+width;
							int tail = (2*featureWidth+1)%4;

							squaredDifference16(&vecSquaredDifference, leftPtr, rightPtr);

							switch (tail)
							{
								case 1:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									squaredDifference += diff1*diff1;
									break;
								case 2:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									squaredDifference += diff1*diff1 + diff2*diff2;
									break;
								case 3:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
									squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
								default:
									break;
							}
						}
					}
					else if (width == 12)
					{
						for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
						{
							float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
							float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;
							float diff1, diff2, diff3;
							int stop = -featureWidth+width;
							int tail = (2*featureWidth+1)%4;
							
							squaredDifference12(&vecSquaredDifference, leftPtr, rightPtr);

							switch (tail)
							{
								case 1:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									squaredDifference += diff1*diff1;
									break;
								case 2:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									squaredDifference += diff1*diff1 + diff2*diff2;
									break;
								case 3:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
									squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
								default:
									break;
							}
						}
					}
					else if (width == 20)
					{
						for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
						{
							float *leftPtr = left+(y+boxY)*imageWidth+x-featureWidth;
							float *rightPtr = right+(y+dy+boxY)*imageWidth+x+dx-featureWidth;
							float diff1, diff2, diff3;
							int stop = -featureWidth+width;
							int tail = (2*featureWidth+1)%4;
							
							squaredDifference20(&vecSquaredDifference, leftPtr, rightPtr);

							switch (tail)
							{
								case 1:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									squaredDifference += diff1*diff1;
									break;
								case 2:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									squaredDifference += diff1*diff1 + diff2*diff2;
									break;
								case 3:
									diff1 = left[(y+boxY)*imageWidth+x+stop]-right[(y+dy+boxY)*imageWidth+x+dx+stop];
									diff2 = left[(y+boxY)*imageWidth+x+stop+1]-right[(y+dy+boxY)*imageWidth+x+dx+stop+1];
									diff3 = left[(y+boxY)*imageWidth+x+stop+2]-right[(y+dy+boxY)*imageWidth+x+dx+stop+2];
									squaredDifference += diff1*diff1 + diff2*diff2 + diff3*diff3;
								default:
									break;
							}
						}
					}

					float result[4];
					_mm_storeu_ps(result, vecSquaredDifference);
					squaredDifference += result[0] + result[1] + result[2] + result[3];
					//printf("<%f, %f, %f, %f>\n", result[0], result[1], result[2], result[3]);

					//Check if current nearest feature
					if ((minimumSquaredDifference > squaredDifference) || ((minimumSquaredDifference == squaredDifference) && (dx*dx+dy*dy < minimumDx*minimumDx+minimumDy*minimumDy)) || (minimumSquaredDifference == -1))
					{
						//Set the minimum squared difference and minimum x and y displacements
						minimumSquaredDifference = squaredDifference;
						minimumDx = dx;
						minimumDy = dy;
					}
				}
			}

			//Update depth map
			if (minimumSquaredDifference != -1)  //changed here
			{
				if (maximumDisplacement == 0)
				{
					depth[y * imageWidth + x] = 0;
				}
				else
				{
					depth[y * imageWidth + x] = displacementNaive(minimumDx, minimumDy);
				} 
			}
			else
			{
				depth[y * imageWidth + x] = 0;
			}
		}
	}
}
