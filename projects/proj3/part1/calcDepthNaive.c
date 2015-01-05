// CS 61C Fall 2014 Project 3

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "calcDepthNaive.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

// Implements the displacement function
float displacementNaive(int dx, int dy)
{
	float squaredDisplacement = dx * dx + dy * dy;
	float displacement = sqrt(squaredDisplacement);
	return displacement;
}

void calcDepthNaive(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement, size_t* floatOps)
{
	if (floatOps != NULL)
	{
		*floatOps = 0;
	}

	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			if ((y < featureHeight) || (y >= imageHeight - featureHeight) || (x < featureWidth) || (x >= imageWidth - featureWidth))
			{
				depth[y * imageWidth + x] = 0;
				continue;
			}

			float minimumSquaredDifference = -1;
			int minimumDy = 0;
			int minimumDx = 0;

			for (int dy = -maximumDisplacement; dy <= maximumDisplacement; dy++)
			{
				for (int dx = -maximumDisplacement; dx <= maximumDisplacement; dx++)
				{
					if (y + dy - featureHeight < 0 || y + dy + featureHeight >= imageHeight || x + dx - featureWidth < 0 || x + dx + featureWidth >= imageWidth)
					{
						continue;
					}

					float squaredDifference = 0;

					for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
					{
						for (int boxX = -featureWidth; boxX <= featureWidth; boxX++)
						{
							int leftX = x + boxX;
							int leftY = y + boxY;
							int rightX = x + dx + boxX;
							int rightY = y + dy + boxY;

							float difference = left[leftY * imageWidth + leftX] - right[rightY * imageWidth + rightX];
							squaredDifference += difference * difference;

							if (floatOps != NULL)
							{
								*floatOps += 3;
							}
						}
					}

					if ((minimumSquaredDifference == -1) || ((minimumSquaredDifference == squaredDifference) && (displacementNaive(dx, dy) < displacementNaive(minimumDx, minimumDy))) || (minimumSquaredDifference > squaredDifference))
					{
						minimumSquaredDifference = squaredDifference;
						minimumDx = dx;
						minimumDy = dy;
					}
				}
			}

			if (minimumSquaredDifference != -1)
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
