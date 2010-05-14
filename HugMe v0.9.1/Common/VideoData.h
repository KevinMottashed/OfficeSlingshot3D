#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include "boost.h" // boost libraries
#include "stdafx.h" // STL + windows

static const unsigned int IMAGE_WIDTH = 320;
static const unsigned int IMAGE_HEIGHT = 240;
static const unsigned int BYTES_PER_PIXEL = 4;
static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL;

struct VideoData
{
	VideoData() {}
	VideoData(boost::shared_ptr<std::vector<BYTE> > rgb) : rgb(rgb) {}

	boost::shared_ptr<std::vector<BYTE> > rgb;
};

#endif

