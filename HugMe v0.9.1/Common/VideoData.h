#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

static const unsigned int IMAGE_WIDTH = 320;
static const unsigned int IMAGE_HEIGHT = 240;
static const unsigned int BYTES_PER_PIXEL = 4;
static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL;

struct VideoData
{
	VideoData() : rgb(NULL), width(0), height(0) {}
	VideoData(char* rgb, unsigned int width, unsigned int height) : rgb(rgb), width(width), height(height) {}

	char* rgb;
	unsigned int width;
	unsigned int height;
};

#endif

