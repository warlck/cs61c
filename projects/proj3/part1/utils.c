// CS 61C Fall 2014 Project 3

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// Call this function when a memory allocation fails: exits the program, returning -1.
void allocationFailed()
{
	printf("Memory allocation failed.\r\n");
	exit(-1);
}

// Fills dst with n random floats between 0 and 1.
void fillRandomFloat(float* dst, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		dst[i] = (int)((float)UCHAR_MAX * ((float)rand() / (float)RAND_MAX)) % UCHAR_MAX;
	}
}

// Checks for floating point equality.
bool floatsWithinTolerance(float a, float b)
{
	if (a == INFINITY || b == INFINITY || a == -INFINITY || b == -INFINITY)
	{
		return false;
	}
	if (abs(a - b) <= FLOATING_POINT_TOLERANCE)
	{
		return true;
	}
	return false;
}

// Converts a float array to a normalized char array.
void floatToChar(unsigned char* dst, const float* const src, size_t n, int maximumDisplacement)
{
	for (size_t i = 0; i < n; i++)
	{
		dst[i] = (unsigned char)round((float)UCHAR_MAX * src[i] / sqrt(2 * maximumDisplacement * maximumDisplacement));
	}

}

// Converts a char array into a float array.
void charToFloat(float* dst, const unsigned char* const src, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		dst[i] = (float)src[i];
	}
}

// Reads a little-endian int from a buffer (works on both little and big endian machines).
static unsigned int readInt(const unsigned char* buffer)
{
	return (buffer[0] & 0xff) | ((buffer[1] & 0xff) << 8) | ((buffer[2] & 0xff) << 16) | ((buffer[3] & 0xff) << 24);
}

// Writes a int to a buffer (works on both little and big endian machines).
static void writeInt(unsigned char* buffer, unsigned int value)
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
}

// Loads a bitmap file.
Image loadImage(char *filename)
{
	unsigned char info[54];
	unsigned char* data;
	unsigned char* colorTable;
	int width;
	int height;
	int size;
	int colorTableSize;
	int check;
	int row;
	int rowPaddingPerRow;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("Error opening file: %s\r\n", filename);
		exit(-1);
	}

	// Read the 54-byte info header
	check = fread(info, sizeof(unsigned char), 54, f);
	if (check != 54)
	{
		printf("Bad file format\r\n");
		exit(-1);
	}

	colorTableSize = readInt(info + 0x0a) - 54;

	// Expecting a 54-byte header and a bit-depth of 8 bits (grayscale)
	if (readInt(info + 0x0e) != 40 || readInt(info + 0x1c) != 8)
	{
		printf("Unsupported image format. Please specify an 8-bit (grayscale) BMP image.\r\n");
		exit(-1);
	}

	colorTable = (unsigned char *)malloc(sizeof(unsigned char)* colorTableSize);
	if (fread(colorTable, sizeof(unsigned char), colorTableSize, f) != colorTableSize)
	{
		printf("Error reading image file.\r\n");
		exit(-1);
	}

	width = readInt(info + 0x12);
	height = readInt(info + 0x16);
	width = (width > 0) ? width : -1 * width;
	height = (height > 0) ? height : -1 * height;
	size = width * height;

	data = (unsigned char*)malloc(sizeof(unsigned char)* size);
	if (data == NULL)
	{
		allocationFailed();
	}

	// Assumes 1 byte per pixel
	rowPaddingPerRow = (4 - ((width * 1) % 4)) % 4;

	// Load data from bottom to top
	for (row = height - 1; row >= 0; row--)
	{
		if (fread(data + row * width, sizeof(unsigned char), width, f) != width)
		{
			printf("Error reading image file.\r\n");
			exit(-1);
		}
		if (rowPaddingPerRow > 0)
		{
			fseek(f, rowPaddingPerRow, SEEK_CUR);
		}
	}

	fclose(f);

	Image img = { .data = data, .width = width, .height = height };
	free(colorTable);
	return img;
}

// Prints a 2D float array.
void printFloatImage(const float *data, int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			printf("%04.4f ", data[y * width + x]);
		}
		printf("\r\n");
	}
}

// Prints a 2D char array.
void printImage(const unsigned char *data, int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			printf("%02x ", data[y * width + x]);
		}
		printf("\r\n");
	}
}

// Common code for writing bitmap files.
// NOTE: Pixels should be {blue, green, red} in that order, if 24 bit color
static void saveImageInternal(char *filename, const unsigned char *data, int width, int height, int bitDepth)
{
	if (bitDepth != 8 && bitDepth != 24)
	{
		printf("Unsupported bit depth: %i\r\n", bitDepth);
		exit(-1);
	}

	FILE *f = fopen(filename, "wb");
	int row;
	int color;
	int bytesPerPixel = bitDepth / 8;

	if (f == NULL)
	{
		printf("Error opening file for writing: %s\r\n", filename);
		exit(-1);
	}

	int infoSize = 54;
	int colorTableSize = (bitDepth == 24) ? 0 : 256 * 4;
	int rowPaddingPerRow = (4 - ((width * bytesPerPixel) % 4)) % 4;
	int dataSize = (width * bytesPerPixel + rowPaddingPerRow) * height;
	int footerSize = (4 - ((infoSize + colorTableSize + dataSize) % 4)) % 4;
	int totalSize = infoSize + colorTableSize + dataSize + footerSize;

	unsigned char info[] = 
	{
		// 0x00 Magic byte
		0x42, 0x4d,
		// 0x02 File size
		0x00, 0x00, 0x00, 0x00,
		// 0x06 Unused
		0x00, 0x00, 0x00, 0x00,
		// 0x0a Location of data (from start of file)
		0x00, 0x00, 0x00, 0x00,
		// 0x0e Size of the remainder of the header (including this field)
		0x00, 0x00, 0x00, 0x00,
		// 0x12 Width
		0x00, 0x00, 0x00, 0x00,
		// 0x16 Height
		0x00, 0x00, 0x00, 0x00,
		// 0x1A Number of planes
		0x01, 0x00,
		// 0x1C Bit depth (8 for grayscale, 24 for rgb color)
		0x00, 0x00,
		// 0x1E BI_RBG
		0x00, 0x00, 0x00, 0x00,
		// 0x22 Size of the data (including padding)
		0x00, 0x00, 0x00, 0x00,
		// 0x26 Horizontal PPI
		0x12, 0x0b, 0x00, 0x00,
		// 0x2A Vertical PPI
		0x12, 0x0b, 0x00, 0x00,
		// Number of palette colors (defaults to 2^{bit depth})
		0x00, 0x00, 0x00, 0x00,
		// Number of "important colors"
		0x00, 0x00, 0x00, 0x00
	};

	// A positive height means bottom-to-top data order
	width = (width > 0) ? width : -1 * width;
	height = (height > 0) ? height : -1 * height;

	writeInt(info + 0x2, totalSize);
	writeInt(info + 0xa, infoSize + colorTableSize);
	writeInt(info + 0xe, infoSize - 0xe);
	writeInt(info + 0x12, width);
	writeInt(info + 0x16, height);
	writeInt(info + 0x22, dataSize + footerSize);

	info[0x1c] = (unsigned char)bitDepth;

	fwrite(info, 1, 54, f);

	unsigned char colorTable[4];
	if (bitDepth == 8)
	{
		for (color = 0x00; color <= 0xff; color++)
		{
			colorTable[0] = (unsigned char)color;
			colorTable[1] = (unsigned char)color;
			colorTable[2] = (unsigned char)color;
			colorTable[3] = 0x00;
			fwrite(colorTable, 1, 4, f);
		}
	}

	unsigned char rowPadding[4] = { 0x00, 0x00, 0x00, 0x00 };

	// Save data from bottom to top
	for (row = height - 1; row >= 0; row--)
	{
		fwrite(data + row * width * bytesPerPixel, bytesPerPixel, width, f);
		if (rowPaddingPerRow > 0)
		{
			fwrite(rowPadding, 1, rowPaddingPerRow, f);
		}
	}

	// Pad the bmp to a round number (multiple of 4)
	unsigned char footer[4] = { 0x00, 0x00, 0x00, 0x00 };
	fwrite(footer, 1, footerSize, f);

	fclose(f);
}

// Saves a grayscale bitmap image.
void saveImage(char *filename, const unsigned char *data, int width, int height)
{
	saveImageInternal(filename, data, width, height, 8);
}

// Saves a color bitmap image.
void saveImageWithDepth(char *filename, const unsigned char *data, const unsigned char *depth, int width, int height, int featureWidth, int featureHeight)
{
	// Blue (far away)
	unsigned int farColor[] = { 0xff, 0x060, 0x60 };
	// Red (close up)
	unsigned int closeColor[] = { 0x60, 0x60, 0xff };
	unsigned int depthColor[3];

	unsigned char *data24bit = (unsigned char*)malloc(sizeof(unsigned char) * 24 / 8 * width * height);
	if (data24bit == NULL)
	{
		allocationFailed();
	}

	int x;
	int y;
	int channel;
	unsigned char dataPixel;
	unsigned char depthPixel;
	unsigned char* destinationPixel;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			dataPixel = data[y * width + x];
			depthPixel = depth[y * width + x];
			destinationPixel = &data24bit[3 * (y * width + x)];
			for (channel = 0; channel < 3; channel++)
			{
				if (x < featureWidth || x >= width - featureWidth || y < featureHeight || y >= height - featureHeight)
				{
					// If the target is at the edge, just use grayscale
					destinationPixel[channel] = dataPixel;
				}
				else
				{
					depthColor[channel] = (farColor[channel] * (256 - depthPixel) + closeColor[channel] * depthPixel) / 256;
					destinationPixel[channel] = (unsigned char)((depthColor[channel] + dataPixel) / 2);
				}
			}
		}
	}

	saveImageInternal(filename, data24bit, width, height, 24);
}

