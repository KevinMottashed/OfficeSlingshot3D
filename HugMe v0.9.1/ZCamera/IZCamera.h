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

	// create a thread to poll the camera
	virtual void start() = 0;

	// stop the thread that's polling the camera
	virtual void stop() = 0;

private:
	IZCamera(const IZCamera& zCamera); // intentionally not implemented
	IZCamera& operator=(const IZCamera& zCamera); // intentionally not implemented
};

#endif
 
