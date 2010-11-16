#ifndef ZCAMERA_H
#define ZCAMERA_H

#include "stdafx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "IZCamera.h"
#include "boost.h"

/**
 * Concrete implementation of the z camera manager
 */
class ZCamera : public IZCamera 
{
public:
	/**
	 * Constructor
	 */
	ZCamera();

	/**
	 * Destructor
	 */
	virtual ~ZCamera();

	/**
	 * Create a thread to start capturing frames from the camera
	 */
	virtual void startCapture();

	/**
	 * Stop the thread that's capturing frames from the camera
	 */
	virtual void stopCapture();
	
	/**
	 * Camera loop that fetches frames and player position.
	 * runs at 32 fps and continually sends information to controller
	 */
	void getFrameFromCamera();

private:
	/**
	 * Copy-Constructor
	 */
	ZCamera(const ZCamera& zCamera); // intentionally not implemented

	/**
	 * Copy-Constructor
	 */
	ZCamera& operator=(const ZCamera& zCamera); // intentionally not implemented

	/**
	 * Thread for capturing frames from the camera
	 */
	std::auto_ptr<boost::thread> zcameraThread;

	/**
	 * True if a camera is present
	 */
	bool zcamPresent;

	/**
	 * The CDepthCamera object
	 */
	CDepthCamera * m_depthCamera;

	/**
	 * A reference to the RGB bytes
	 */
	boost::shared_ptr<std::vector<BYTE> > RGB;

	/**
	 * The depth data
	 */
	unsigned char* DEPTH;

	unsigned char* RGBFrame; /**< A frame of RGB. */

	/**
	 * A frame of RGB, full resolution.
	 */
	unsigned char* RGBFull;

	/**
	 * The primary IR data
	 */
	unsigned char* PRIM;

	/**
	 * The secondary IR data
	 */
	unsigned char* SEC;

	/**
	 * Get player position
	 * @return the player position
	 */
	cVector3d getPlayerPosition();
};

#endif
 
