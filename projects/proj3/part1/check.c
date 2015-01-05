// CS 61C Fall 2014 Project 3

#include <sys/types.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "calcDepthNaive.h"
#include "calcDepthOptimized.h"
#include "utils.h"

static bool testCalcDepth(int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	float* leftImage = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (leftImage == NULL)
	{
		allocationFailed();
	}
	float* rightImage = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (rightImage == NULL)
	{
		free(leftImage);
		allocationFailed();
	}

	fillRandomFloat(leftImage, imageWidth * imageHeight);
	fillRandomFloat(rightImage, imageWidth * imageHeight);

	float* depthNaive = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthNaive == NULL)
	{
		free(leftImage);
		free(rightImage);
		allocationFailed();
	}
	float* depthOptimized = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthOptimized == NULL)
	{
		free(leftImage);
		free(rightImage);
		free(depthNaive);
		allocationFailed();
	}

	calcDepthNaive(depthNaive, leftImage, rightImage, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement, NULL);
	calcDepthOptimized(depthOptimized, leftImage, rightImage, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);

	for (size_t i = 0; i < imageWidth * imageHeight; i++)
	{
		if (!floatsWithinTolerance(depthNaive[i], depthOptimized[i]))
		{
			free(leftImage);
			free(rightImage);
			free(depthNaive);
			free(depthOptimized);
			return false;
		}
	}
	free(leftImage);
	free(rightImage);
	free(depthNaive);
	free(depthOptimized);
	return true;
}

static bool testCalcDepthVerbose(int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	printf("Testing %ix%i image, feature width %i, feature height %i, maximum diplacement %i... ", imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);
	if (testCalcDepth(imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement))
	{
		printf("Test success!\r\n");
		return true;
	}
	else
	{
		printf("Error! Results of naive and optimized code do not match.\r\n");
		return false;
	}
}

int main(int argc, char **argv)
{
	int returnResult = 0;
	if (!testCalcDepthVerbose(3, 3, 2, 2, 1))
	{
		returnResult = - 1;
	}
	if (!testCalcDepthVerbose(5, 5, 2, 2, 1))
	{
		returnResult = -1;
	}
	if (!testCalcDepthVerbose(10, 10, 2, 2, 1))
	{
		returnResult = -1;
	}
	if (!testCalcDepthVerbose(10, 10, 5, 5, 2))
	{
		returnResult = -1;
	}
	if (!testCalcDepthVerbose(100, 100, 3, 3, 14))
	{
		returnResult = -1;
	}

	if (!testCalcDepthVerbose(1, 20, 3, 3, 14))
	{
		returnResult = -1;
	}
	if (!testCalcDepthVerbose(20, 1, 3, 3, 14))
	{
		returnResult = -1;
	}

	if (!testCalcDepthVerbose(10, 10, 2, 5, 1))
	{
		returnResult = -1;
	}
	if (!testCalcDepthVerbose(10, 10, 5, 2, 2))
	{
		returnResult = -1;
	}

	if (!testCalcDepthVerbose(21, 5, 9, 5, 15))
	{
		returnResult = -1;
	}

	return returnResult;
}
