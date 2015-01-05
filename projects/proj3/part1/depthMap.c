// CS 61C Fall 2014 Project 3

#include <sys/types.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#include "calcDepthNaive.h"
#include "calcDepthOptimized.h"

#define USAGE "\
 USAGE: %s [options] \n\
 \n\
 REQUIRED \n\
     -l [LEFT_IMAGE]       The left image\n\
     -r [RIGHT_IMAGE]      The right image\n\
     -w [WIDTH_PIXELS]     The width of the smallest feature\n\
     -h [HEIGHT_PIXELS]    The height of the smallest feature\n\
     -t [MAX_DISPLACE]     The threshold for maximum displacement\n\
 \n\
 OPTIONAL \n\
     -o [OUTPUT_IMAGE]     Draw output to this file\n\
     -v                    Print the output to stdout as readable bytes\n\
 "

int main(int argc, char **argv)
{
	char *leftFile = NULL;
	char *rightFile = NULL;
	char *outFile = NULL;

	int featureWidth = -1;
	int featureHeight = -1;
	int maximumDisplacement = -1;

	bool verbose = false;

	for (size_t i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'l':
				if (argc > i + 1)
				{
					leftFile = argv[++i];
				}
				break;
			case 'r':
				if (argc > i + 1)
				{
					rightFile = argv[++i];
				}
				break;
			case 'o':
				if (argc > i + 1)
				{
					outFile = argv[++i];
				}
				break;
			case 'w':
				if (argc > i + 1)
				{
					featureWidth = atoi(argv[++i]);
				}
				break;
			case 'h':
				if (argc > i + 1)
				{
					featureHeight = atoi(argv[++i]);
				}
				break;
			case 't':
				if (argc > i + 1)
				{
					maximumDisplacement = atoi(argv[++i]);
				}
				break;
			case 'v':
				verbose = true;
				break;
			default:
				printf("Unknown option: %s\r\n", argv[i]);
				exit(-1);
			}
		}
	}

	if (leftFile == NULL || rightFile == NULL || featureWidth == -1 || featureHeight == -1 || maximumDisplacement == -1)
	{
		printf(USAGE, argv[0]);
		exit(-1);
	}

	Image left = loadImage(leftFile);
	Image right = loadImage(rightFile);
	if (left.height != right.height || left.width != right.width)
	{
		printf("The two images do not have the same dimensions.\n");
		exit(-1);
	}

	Image depth;

	depth.width = left.width;
	depth.height = right.height;
	depth.data = (unsigned char*)malloc(depth.width * depth.height);
	if (!depth.data) allocationFailed();

	float* depthResult = malloc(sizeof(float)* depth.width * depth.height);
	float* leftImageData = malloc(sizeof(float)* left.width * left.height);
	float* rightImageData = malloc(sizeof(float)* right.width * right.height);

	charToFloat(leftImageData, left.data, left.width * left.height);
	charToFloat(rightImageData, right.data, right.width * right.height);

	calcDepthOptimized(depthResult, leftImageData, rightImageData,
		left.width, left.height, featureWidth,
		featureHeight, maximumDisplacement);

	floatToChar(depth.data, depthResult, depth.width * depth.height, maximumDisplacement);

	if (outFile != NULL)
	{
		saveImageWithDepth(outFile, left.data, depth.data, left.width, left.height, featureWidth, featureHeight);
	}

	if (verbose)
	{
		printImage(depth.data, depth.width, depth.height);
	}

	return 0;
}
