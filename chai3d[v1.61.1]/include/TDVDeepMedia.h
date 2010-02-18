#pragma once

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED


// represent a point in 3D space
struct TDVCameraPoint
{
	float x, y, z;
};

// represent a point in image space. d is the depth value
struct TDVImagePoint
{
	//int x, y, d;  //2007.05.24 Changed to floating point due to that the Kalman filter generates subpixel
	float x, y, d;  //           coordinates, which can be taken advantage of.
};

struct TDVHead
{
	float confidence;  //Confidence value of the head detection. Ranges [0.0,1.0].
	//int rotation;
	int radius;
	TDVImagePoint headPos; // position of the hand center
};

struct TDVCameraParams
{
	int		m_iImageWidth;	//Number of rows (Y)
	int		m_iImageHeight;	//Number of columns (X)
	float	m_fFocalLength;	//focal length [mm]
	float	m_fPixelWidth;	//pixel width on sensor [mm]
	float	m_fPixelHeight;	//pixel height on sensor [mm]
	float	m_fXOpticalCenter; //PRINCIPALPOINT X-axis 
	float	m_fYOpticalCenter; //PRINCIPALPOINT y-axis
};

struct TDVCameraWindowParams
{
	int m_nWindowWidth; // in cm
	int m_nWindowDistance; // in cm
};

struct ImagePoint
{
	unsigned short x;
	unsigned short y;
};

struct CompleteData
{
	int NrFingers;
	ImagePoint fingers[10];
};
