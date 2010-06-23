#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include "stdafx.h" // STL + windows
#include "boost.h" // boost libraries

static const unsigned int IMAGE_WIDTH = 320;
static const unsigned int IMAGE_HEIGHT = 240;
static const unsigned int BYTES_PER_PIXEL = 4;
static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL;

struct VideoData
{
public:
	VideoData() : rgb(IMAGE_ARRAY_SIZE) {}

	std::vector<BYTE> rgb;
private:
	VideoData(const VideoData& videoData); // intentionally not implemented
	VideoData& operator=(const VideoData& videoData); // intentionally not implemented

	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);
};

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename Archive>
void VideoData::serialize(Archive& ar, const unsigned int version)
{
	// We serialize the rgb as binary data, as it much more efficient.
	ar & boost::serialization::make_binary_object(&rgb.front(), IMAGE_ARRAY_SIZE);
	return;
}

#endif

