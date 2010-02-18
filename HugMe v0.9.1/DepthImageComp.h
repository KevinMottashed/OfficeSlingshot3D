#ifndef __Depth_Image_Comp_H__
#define __Depth_Image_Comp_H__


#include "Encoder/libr263.h"
#include "Decoder/tmndec.h"



#define CIF_WIDTH  352
#define CIF_HEIGHT 288

#define IMAGE_WIDTH       CIF_WIDTH
#define IMAGE_HEIGHT      CIF_HEIGHT


class DepthImageComp
{
public:
	// compression
	CParam cparams;
	
	// Global varialbes...
	unsigned char rgbdata[IMAGE_WIDTH*IMAGE_HEIGHT*3];
	int buffersize;
	unsigned char depthdata[IMAGE_WIDTH*IMAGE_HEIGHT];

	unsigned int compSize;
	unsigned char compImage[120000];
	unsigned int compDepthSize;
	unsigned char compDepthImage[120000];

	// Buffer for storing YUV data....
	unsigned int yuv[ CIF_WIDTH*CIF_HEIGHT  + (CIF_WIDTH*CIF_HEIGHT)/2 ];
	// Compression parameters....
	Bits bits;		// Various count about compression
			
	DepthImageComp();
	virtual ~DepthImageComp();

	void InitEncoder();
	void InitDecoder();

	void Compress(unsigned char * a_pSrc);
	void Decompress(unsigned char * a_pSrc, unsigned int a_size);
	void CompressDepth(unsigned char * a_pSrc);
	void DecompressDepth(unsigned char * a_pSrc, unsigned int a_size);
};

#endif