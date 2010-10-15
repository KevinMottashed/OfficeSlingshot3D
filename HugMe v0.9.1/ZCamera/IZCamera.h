#ifndef IZCAMERA_H
#define IZCAMERA_H

#include "stdafx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "ZCameraSubject.h"
#include "VideoData.h"
#include "boost.h"

/**
 * Abstract interface for a z camera
 */
class IZCamera : public ZCameraSubject
{
public:
	/**
	 * Constructor
	 */
	IZCamera();

	/**
	 * Destructor
	 */
	virtual ~IZCamera();

	/**
	 * Create a thread to start capturing frames from the camera
	 */
	virtual void startCapture() = 0;

	/**
	 * Stop the thread that's capturing frames from the camera
	 */
	virtual void stopCapture() = 0;

private:
	/**
	 * Copy-Constructor
	 */
	IZCamera(const IZCamera& zCamera); // intentionally not implemented
	
	/**
	 * Copy-Constructor
	 */
	IZCamera& operator=(const IZCamera& zCamera); // intentionally not implemented
};

#endif
 
