//****************************************************************************
//
//  Copyright (C) 1997-2007 3DV SYSTEMS LTD.
//  All rights reserved.
//
//  Module Name:	DepthCamera.h
//  Date:			1-9-2007
//  Description:	header file for CDepthCamera class, which uses
//					the DMachine SDK to control the camera.
//  Updated By:		
//	Last Updated:			
//	Version:		1.00
//****************************************************************************

#ifndef DEPTH_CAMERA_H
#define DEPTH_CAMERA_H

#include "windows.h"
#include "tdvCameraInterface.h"

// video callback definition
void videoCallBack(unsigned char * pDepthBuf, unsigned char * pRgbBuf, 
						  int bufIndex, void* pObject);
// command callback definition
void cmdCallBack(int cmd, void* pObject);


class CDepthCamera
{
	// define the callbacks as friend functions, to enable them full access to CDepthCamera object
	friend void videoCallBack(unsigned char * pDepthBuf, unsigned char * pRgbBuf, 
							  int bufIndex, void* pObject);
	friend void cmdCallBack(int cmd, void* pObject);

public:
	CDepthCamera(void);

	// Clean-up internal objects.
	~CDepthCamera(void); 

	// initialize the TDVCameraInterfaceBase object, and the callbacks
	// in case the camera is not connected or DMachine is not running,
	// it will wait the specified time until returning, to prevent program blocking.
	// by default, it will wait indefinitely
	bool Initialize(int timeout = INFINITE); 
	
	// currently the supported resolutions are: 
	//		- RESOLUTION_320X240_30FPS 
	//		- RESOLUTION_160X120_60FPS
	// this function blocks until the resolution has been changed.
	void ChangeResolution(int resolutionMode);

	// get video frame parameters.
	bool GetVideoSize(int& width, int& height, 
					  int& rgbFullResWidth, int& rgbFullResHeight,
					  int& depthPixelSize, int& rgbPixelSize) const;

	// returns the maximal width possible for setting depth range, measured in centemeters
	int GetMaxAllowedWidth() const;

	// set the depth range manually. This function chooses default settings
	// that will best fit the distance\width specified
	// the depth range will be between distance and distance+width, measured in centemeters
	void SetDepthWindowPosition(int distance, int width);

	// get the depth range distance\width
	// the depth range will be between distance and distance+width, measured in centemeters
	void GetDepthWindowPosition(int &distance, int &width) const;

	// set the width specified (in centemeters), and centers the depth window 
	// around the closest object to the camera
	// the function blocks until object is found
	void FindObjectUsingWidth(int depthWidth);

	// enable\disable the transfer of RGB in 640x480 resolution. 
	// When enabled, this consumes more CPU
	void EnableRGBFullResolution(bool enable);

	// check whether the RGB in full resolution is also transfered
	bool IsRGBFullResolutionEnabled() const;

	// enable\disable the transfer of IR Video (Primary and secondary IR).
	// When enabled, this consumes more CPU
	void EnableIRVideoTransfer(bool enable);

	// check whether the IR video (primary and secondary) is also transfered
	bool IsIRVideoTransferEnabled() const;

	// when the user needs a 300cm depth window range, enable this mode.
	// It will automatically place the depth range to support depth range starting
	// from 100cm and ending at 400cm (distance from the camera).
	void EnableWideDepthMode(bool enable);

	// is the large depth mode enabled?
	bool IsWideDepthModeEnabled() const;

	// enable\disable the "smooth" filter. also set the filter's intensity
	void EnableDepthFilter(bool enable, int intensity = 25);
	
	// check if the depth filter is enabled, and if so, what is it's intensity
	bool IsDepthFilterEnabled() const;

	// returns the next frames data. 
	// If RGB full resolution or the IR video is not enabled,
	// their relevant pointer will be NULL.
	// This function will block until the next frame is ready, or until timeout
	// is reached. If timeout occurs, the function returns false, otherwise true.
	bool GetNextFrame(unsigned char* &pDepthFrame, 
					  unsigned char* &pRGBFrame, 
					  unsigned char* &pRGBFullResFrame,
					  unsigned char* &pPrimaryIR,
					  unsigned char* &pSecondaryIR,
					  int timeout = INFINITE);
	// when you wish to get full control over other parameters, you can get the
	// internal TDVCameraInterfaceBase object and handle things by yourself
	TDVCameraInterfaceBase* GetCameraInterface();

	// by gaecha
	void EnableTrack(bool enable);
	void EnableSmoothing(bool enable, double smoothVal);
	void SetCleanVal(double cleanVal);
	void EnableMedian(bool enable);
	void FreezeVideo(bool freeze);
	//

private:

	// uninitialize all internal objects.
	void UnInitialize(); 

	// sets new frames internally (called from the video callback)
	void SetFrames(unsigned char* pDepthFrame, unsigned char* pRGBFrame, 
				   unsigned char* pRGBFullResFrame,
				   unsigned char* pPrimaryIR,unsigned char* pSecondaryIR);
	bool Lock();	// enter critical section. Returns true if successfully locked
	void Unlock();	// exit critical section

	TDVCameraInterfaceBase* m_pCameraInterfaceBase;		// the camera control object
	bool					m_bEnableRGBFullResoltion;	// is RGB full resolution enables?
	bool					m_bEnableIRVideoTransfer;	// is RGB full resolution enables?
	bool					m_bEnableLargeDepthMode;	// is the large depth mode enabled?
	HANDLE					m_hNewFrameReady;			// handle to notify when a new frame is received
	HANDLE					m_hCriticalSection;			// critical section lock mutex
	unsigned char*			m_pDepthFrame;				// holds the latest depth frame
	unsigned char*			m_pRGBFrame;				// holds the latest RGB frame
	unsigned char*			m_pRGBFullResFrame;			// holds the latest RGB full resolution frame
	unsigned char*			m_pPrimary;					// holds the latest primary IR frame
	unsigned char*			m_pSecondary;				// holds the latest secondary IR frame
	bool					m_bIsInitialized;			// Is the DepthCamera initialized?
};

#endif

