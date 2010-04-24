#ifndef VIDEO_DATA_H_
#define VIDEO_DATA_H_

static const unsigned int IMAGE_WIDTH = 320;
static const unsigned int IMAGE_HEIGHT = 240;
static const unsigned int BYTES_PER_PIXEL = 4;
static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL;

struct VideoData
{
	VideoData(const char* rgb, unsigned int width, unsigned int height) : rgb(rgb), width(width), height(height) {}

	const char* const rgb;
	const unsigned int width;
	const unsigned int height;
};

#endif