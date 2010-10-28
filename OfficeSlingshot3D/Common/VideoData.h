#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include "stdafx.h" // STL + windows
#include "boost.h" // boost libraries

/**
 * The width of the image.
 */
static const unsigned int IMAGE_WIDTH = 320;

/**
 * The height of the image.
 */
static const unsigned int IMAGE_HEIGHT = 240;

/**
 * The number of bytes per pixel.
 * The camera takes pictures in RBGA red-green-blue-alpha so we need 4 bytes per pixel.
 */
static const unsigned int BYTES_PER_PIXEL = 4;

/**
 * The size of a picture
 */
static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL;

/**
 * Represent a frame of video data.
 */
struct VideoData
{
public:
	/**
	 * Constructor
	 */
	VideoData() : rgb(IMAGE_ARRAY_SIZE) {}

	/**
	 * The rgb data of the video frame
	 */
	std::vector<BYTE> rgb;
private:
	/**
	 * Copy constructor. Not implemented to protect from use.
	 * @param videoData The object to copy.
	 */
	VideoData(const VideoData& videoData);
	
	/**
	 * Assignment operator. Not implemented to protect from use.
	 * @param videoData The object to copy.
	 */
	VideoData& operator=(const VideoData& videoData); 

	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize or deserialize the video data.
	 * This function is used by the boost library and should not be called directly.
	 * @param ar The archive to serialize to or deserialize from.
	 * @param version The archive version.
	 */
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

