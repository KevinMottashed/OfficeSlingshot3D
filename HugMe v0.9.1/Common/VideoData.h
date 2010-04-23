#ifndef VIDEO_DATA_H_
#define VIDEO_DATA_H_

struct VideoData
{
	VideoData(const char* rgb, unsigned int size) : rgb(rgb), size(size) {}

	const char* const rgb;
	const unsigned int size;
};

#endif