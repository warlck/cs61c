// CS 61C Fall 2014 Project 3

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include "calcDepthNaive.h"
#include "calcDepthOptimized.h"
#include "utils.h"

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
// Intel Xeon E5620 (2.40 GHz)
#define CLOCK_FREQUENCY ((double)2.4)

#if defined(_MSC_VER)
#pragma intrinsic(__rdtsc)
#else
static __inline__ uint64_t __rdtsc()
{
	uint32_t hi, lo;
	__asm__ volatile
		(
		"rdtsc"
		: "=a"(lo), "=d"(hi)
		);
	return (((uint64_t)hi) << 32) | ((uint64_t)lo);
}
#endif

static __inline__ double timeElapsedInNanoseconds(uint64_t startTSC, uint64_t endTSC)
{
	return (double)(endTSC - startTSC) / CLOCK_FREQUENCY;
}

static bool benchmarkMatrix(int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	float* left = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (left == NULL)
	{
		allocationFailed();
	}
	float* right = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (right == NULL)
	{
		free(left);
		allocationFailed();
	}

	fillRandomFloat(left, imageWidth * imageHeight);
	fillRandomFloat(right, imageWidth * imageHeight);

	float* depthNaive = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthNaive == NULL)
	{
		free(left);
		free(right);
		allocationFailed();
	}
	float* depthOptimized = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthOptimized == NULL)
	{
		free(left);
		free(right);
		free(depthNaive);
		allocationFailed();
	}

	size_t floatOps = 0;

	calcDepthNaive(depthNaive, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement, &floatOps);
	calcDepthOptimized(depthOptimized, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);

	for (size_t i = 0; i < imageWidth * imageHeight; i++)
	{
		if (!floatsWithinTolerance(depthNaive[i], depthOptimized[i]))
		{
			free(left);
			free(right);
			free(depthNaive);
			free(depthOptimized);
			return false;
		}
	}

	double Gflops, nanoseconds = -1.0;
	// 0.25 seconds
	for (size_t numIterations = 1; nanoseconds < 250000000; numIterations *= 2)
	{
		uint64_t startTSC = __rdtsc();

		for (size_t i = 0; i < numIterations; i++)
		{
			calcDepthOptimized(depthOptimized, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);
		}

		uint64_t endTSC = __rdtsc();
		nanoseconds = timeElapsedInNanoseconds(startTSC, endTSC);

		Gflops = (floatOps * numIterations) / nanoseconds;
	}
	printf("%.4f Gflop/s\r\n", Gflops);

	free(left);
	free(right);
	free(depthNaive);
	free(depthOptimized);

	return true;
}

int main(int argc, char** argv)
{
	int returnResult = 0;

    srand(time(NULL));
	
	int tests[][5] = { { 100, 100, 4, 4, 6 }, { 200, 200, 4, 4, 6 }, { 300, 300, 4, 4, 6 }, { 400, 400, 4, 4, 6 }, { 500, 500, 4, 4, 6 } };

	printf("Testing non-odd cases: \r\n");
	for (int i = 0; i < ARRAY_SIZE(tests); i++)
	{
		printf("Testing %ix%i image, feature width %i, feature height %i, maximum diplacement %i... ", tests[i][0], tests[i][1], tests[i][2], tests[i][3], tests[i][4]);
		if (!benchmarkMatrix(tests[i][0], tests[i][1], tests[i][2], tests[i][3], tests[i][4]))
		{
			printf("Error: optimized does not match naive.\r\n");
			returnResult = -1;
		}
	}

	printf("\r\nTesting odd cases: \r\n");
	int oddTests[][5] = { { 200, 400, 3, 3, 7 }, { 400, 200, 4, 4, 7 }, { 301, 301, 4, 4, 7 }, { 401, 401, 4, 4, 4 } };
	for (int i = 0; i < ARRAY_SIZE(oddTests); i++)
	{
		printf("Testing %ix%i image, feature width %i, feature height %i, maximum diplacement %i... ", oddTests[i][0], oddTests[i][1], oddTests[i][2], oddTests[i][3], oddTests[i][4]);
		if (!benchmarkMatrix(oddTests[i][0], oddTests[i][1], oddTests[i][2], oddTests[i][3], oddTests[i][4]))
		{
			printf("Error: optimized does not match naive.\r\n");
			returnResult = -1;
		}
	}

	return returnResult;
}

