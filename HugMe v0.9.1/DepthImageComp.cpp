#include "DepthImageComp.h"

int vcount=0;
unsigned char cdata[120000];
int cbuffer_size=120000;
							
// Callback function gets invoked during compression...
// It returns the compressed data byte by byte...
void OwnWriteFunction(int byte)
{
	if(vcount<cbuffer_size)
	cdata[vcount]=(unsigned char)byte;
	
	vcount++;

}


DepthImageComp::DepthImageComp()
{
	buffersize = IMAGE_WIDTH*IMAGE_HEIGHT*3;
}

DepthImageComp::~DepthImageComp()
{
}


void DepthImageComp::InitEncoder()
{
	// Initialize the compressor
	// Initialize table for RGB to YUV conversion
	InitLookupTable();

	// Initialize the compressor
	cparams.format = CPARAM_CIF;
	InitH263Encoder(&cparams);  
	// Set up the callback function
	WriteByteFunction = OwnWriteFunction;
}

void DepthImageComp::InitDecoder()
{
	// Initialize decompressor
	InitH263Decoder();
}

void DepthImageComp::Compress(unsigned char * a_pSrc)
{
	//Convert the data from rgb format to YUV format	
	ConvertRGB2YUV(IMAGE_WIDTH,IMAGE_HEIGHT,a_pSrc,yuv);
	vcount=0;
	//Compress the data...to h263
	cparams.format=CPARAM_CIF;
	cparams.inter = CPARAM_INTRA;  
	cparams.Q_intra = 8;
	cparams.data=yuv;		//  Data in YUV format...
	CompressFrame(&cparams, &bits);

	compSize = vcount;

	memcpy(compImage, cdata, compSize);
}

void DepthImageComp::Decompress(unsigned char * a_pSrc, unsigned int a_size)
{
	int retvalue;
	retvalue=DecompressFrame(a_pSrc, a_size, rgbdata, buffersize);
}

void DepthImageComp::CompressDepth(unsigned char * a_pSrc)
{
	//Convert the data from rgb format to YUV format	
	ConvertD2YUV(IMAGE_WIDTH,IMAGE_HEIGHT,a_pSrc,yuv);
	vcount=0;
	//Compress the data...to h263
	cparams.format=CPARAM_CIF;
	cparams.inter = CPARAM_INTRA;  
	cparams.Q_intra = 8;
	cparams.data=yuv;		//  Data in YUV format...
	CompressFrame(&cparams, &bits);

	compDepthSize = vcount;

	memcpy(compDepthImage, cdata, compDepthSize);
}

void DepthImageComp::DecompressDepth(unsigned char * a_pSrc, unsigned int a_size)
{
	unsigned char tempdata[IMAGE_WIDTH*IMAGE_HEIGHT*3];
	int retvalue;
	retvalue=DecompressFrame(a_pSrc, a_size, tempdata, buffersize);

	for(int i=0; i<buffersize; i+=3) {
		depthdata[(int)((float)i/3.0)] = tempdata[i];
	}
}