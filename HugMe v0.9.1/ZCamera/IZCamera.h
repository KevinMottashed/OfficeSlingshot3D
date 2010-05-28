#ifndef IZCAMERA_H
#define IZCAMERA_H

#include "stdafx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "ZCameraSubject.h"
#include "VideoData.h"
#include "boost.h"

// abstract interface for a z camera
class IZCamera : public ZCameraSubject
{
public:	
	IZCamera();
	virtual ~IZCamera();

	// create a thread to start capturing frames from the camera
	virtual void startCapture() = 0;

	// stop the thread that's capturing frames from the camera
	virtual void stopCapture() = 0;

private:
	IZCamera(const IZCamera& zCamera); // intentionally not implemented
	IZCamera& operator=(const IZCamera& zCamera); // intentionally not implemented
};

#endif
 
