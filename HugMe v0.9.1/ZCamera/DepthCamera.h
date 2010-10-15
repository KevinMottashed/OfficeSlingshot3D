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

#include "stdafx.h"
#include "tdvCameraInterface.h"

/**
 * video callback definition
 */
void videoCallBack(unsigned char * pDepthBuf, unsigned char * pRgbBuf, 
						  int bufIndex, void* pObject);
/**
 * command callback definition
 */
void cmdCallBack(int cmd, void* pObject);

/**
 * The main class that deals with the
 * z-camera hardware and API
 */
class CDepthCamera
{
	/**
	 * Define the video callback as friend functions, 
	 * to enable full access to CDepthCamera object
	 * @param pDepthBuf The depth buffer
	 * @param pRgbBuf The RGB buffer
	 * @param bufIndex The buffer index
	 */
	friend void videoCallBack(unsigned char * pDepthBuf, unsigned char * pRgbBuf, 
							  int bufIndex, void* pObject);/**
	 * Define the command callback as friend functions, 
	 * to enable full access to CDepthCamera object
	 * @param cmd The command
	 */
	friend void cmdCallBack(int cmd, void* pObject);

public:
	/**
	 * Constructor
	 */
	CDepthCamera(void);

	/**
	 * Clean-up internal objects
	 */
	~CDepthCamera(void); 


	/**
	 * initialize the TDVCameraInterfaceBase object, and the callbacks
	 * in case the camera is not connected or DMachine is not running,
	 * it will wait the specified time until returning, to prevent program blocking.
	 * by default, it will wait indefinitely
	 * @param timeout The timeout. By default INFINITE
	 * @return true if successful
	 */
	bool Initialize(int timeout = INFINITE); 
	
	// currently the supported resolutions are: 
	//		- RESOLUTION_320X240_30FPS 
	//		- RESOLUTION_160X120_60FPS
	// this function blocks until the resolution has been changed.

	/**
	 * Change the video resolution
	 * @param resolutionMode The new resolution
	 */
	void ChangeResolution(int resolutionMode);

	/**
	 * get video frame parameters.
	 * @param width The width of the frame
	 * @param height The height of the frame
	 * @param rgbFullResWidth The RGB width
	 * @param rgbFullResHeight The RGB width
	 * @param depthPixelSize The depth pixel size
	 * @param rgbPixelSize The RGB pixel size
	 * @return true if successful
	 */
	bool GetVideoSize(int& width, int& height, 
					  int& rgbFullResWidth, int& rgbFullResHeight,
					  int& depthPixelSize, int& rgbPixelSize) const;

	/**
	 * Retrieves the maximal width possible for setting depth range, measured in centemeters
	 * @return the maximal width possible
	 */
	int GetMaxAllowedWidth() const;

	/**
	 * @et the depth range manually. This function chooses default settings
	 * that will best fit the distance\width specified
	 * the depth range will be between distance and distance+width, measured in centemeters
	 * @param distance The distance from the camera
	 * @param width The width
	 */
	void SetDepthWindowPosition(int distance, int width);

	/**
	 * Get the depth range distance\width
	 * the depth range will be between distance and distance+width, measured in centemeters
	 * @param distance The distance from the camera
	 * @param width The width
	 */
	void GetDepthWindowPosition(int &distance, int &width) const;

	/**
	 * Set the width specified (in centemeters), and centers the depth window 
	 * around the closest object to the camera
	 * the function blocks until object is found
	 * @param depthWidth The new width
	 */
	void FindObjectUsingWidth(int depthWidth);

	/**
	 * Enable\disable the transfer of RGB in 640x480 resolution. 
	 * When enabled, this consumes more CPU
	 * @param enable True to enable RGB
	 */
	void EnableRGBFullResolution(bool enable);

	/**
	 * Check whether the RGB in full resolution is also transfered
	 * @return true if RBG full resolution is enabled
	 */
	bool IsRGBFullResolutionEnabled() const;

	/**
	 * enable\disable the transfer of IR Video (Primary and secondary IR).
	 * When enabled, this consumes more CPU
	 * @param enable True to enable IR video
	 */
	void EnableIRVideoTransfer(bool enable);

	/**
	 * check whether the IR video (primary and secondary) is also transfered
	 * @return true if IR video is enabled
	 */
	bool IsIRVideoTransferEnabled() const;

	/**
	 * when the user needs a 300cm depth window range, enable this mode.
	 * It will automatically place the depth range to support depth range starting
	 * from 100cm and ending at 400cm (distance from the camera).
	 * @param enable True to enable wide depth mode
	 */
	void EnableWideDepthMode(bool enable);

	/**
	 * is the large depth mode enabled?
	 * @return true if wide depth mode is enabled
	 */
	bool IsWideDepthModeEnabled() const;

	/**
	 * enable\disable the "smooth" filter. also set the filter's intensity
	 * @param enable True to enable "smooth" filter
	 * @param intensity The intensity. 25 by default
	 */
	void EnableDepthFilter(bool enable, int intensity = 25);
	
	/**
	 * check if the depth filter is enabled, and if so, what is it's intensity
	 * @return true if "smooth" filter is enabled
	 */
	bool IsDepthFilterEnabled() const;

	/**
	 * returns the next frames data. 
	 * If RGB full resolution or the IR video is not enabled,
	 * their relevant pointer will be NULL.
	 * This function will block until the next frame is ready, or until timeout
	 * is reached. If timeout occurs, the function returns false, otherwise true.
	 * @param pDepthFrame The depth frame
	 * @param pRGBFrame The RGB frame
	 * @param pRGBFullResFrame The RGB full resolution frame
	 * @param pPrimaryIR The primary IR
	 * @param pSecondaryIR The secondary IR
	 * @param timeout The timeout. INFINITE by default
	 * @return true if successful
	 */
	bool GetNextFrame(unsigned char* &pDepthFrame, 
					  unsigned char* &pRGBFrame, 
					  unsigned char* &pRGBFullResFrame,
					  unsigned char* &pPrimaryIR,
					  unsigned char* &pSecondaryIR,
					  int timeout = INFINITE);
	/**
	 * when you wish to get full control over other parameters, you can get the
	 * internal TDVCameraInterfaceBase object and handle things by yourself
	 * @return TDVCameraInterfaceBase* The TDV camera
	 */
	TDVCameraInterfaceBase* GetCameraInterface();

	/**
	 * Enables tracking
	 * @param enable True to enable tracking
	 */
	void EnableTrack(bool enable);

	/**
	 * Enables smoothing
	 * @param enable True to enable smoothing
	 */
	void EnableSmoothing(bool enable, double smoothVal);

	/**
	 * Sets the clean value
	 * @param cleanVal The new clean value
	 */
	void SetCleanVal(double cleanVal);

	/**
	 * Enables median
	 * @param enable True to enable median
	 */
	void EnableMedian(bool enable);

	/**
	 * Freezes the video
	 * @param freeze True to freeze the video
	 */
	void FreezeVideo(bool freeze);
	//

private:

	/**
	 * uninitialize all internal objects.
	 */
	void UnInitialize(); 

	/**
	 * sets new frames internally (called from the video callback)
	 * @param pDepthFrame The new depth frame
	 * @param pRGBFrame The new RGB frame
	 * @param pRGBFullResFrame The new RGB full resolution frame
	 * @param pPrimaryIR The new primary IR
	 * @param pSecondaryIR The new secondary IR
	 */
	void SetFrames(unsigned char* pDepthFrame, unsigned char* pRGBFrame, 
				   unsigned char* pRGBFullResFrame,
				   unsigned char* pPrimaryIR,unsigned char* pSecondaryIR);
	/**
	 * Enter critical section
	 * @return true if successfully locked
	 */
	bool Lock();

	/**
	 * Exit the critical section
	 */
	void Unlock();

	/**
	 * The camera control object
	 */
	TDVCameraInterfaceBase* m_pCameraInterfaceBase;
	
	/**
	 * Is RGB full resolution enabled?
	 */
	bool					m_bEnableRGBFullResoltion;
	
	/**
	 * Is RGB full resolution enabled?
	 */
	bool					m_bEnableIRVideoTransfer;
	
	/**
	 * Is the large depth mode enabled?
	 */
	bool					m_bEnableLargeDepthMode;
	
	/**
	 * Handle to notify when a new frame is received
	 */
	HANDLE					m_hNewFrameReady;
	
	/**
	 * Critical section lock mutex
	 */
	HANDLE					m_hCriticalSection;
	
	/**
	 * Holds the latest depth frame
	 */
	unsigned char*			m_pDepthFrame;
	
	/**
	 * Holds the latest RGB frame
	 */
	unsigned char*			m_pRGBFrame;
	
	/**
	 * Holds the latest RGB full resolution frame
	 */
	unsigned char*			m_pRGBFullResFrame;
	
	/**
	 * Holds the latest primary IR frame
	 */
	unsigned char*			m_pPrimary;
	
	/**
	 * Holds the latest secondary IR frame
	 */
	unsigned char*			m_pSecondary;
	
	/**
	 * Is the DepthCamera initialized?
	 */
	bool					m_bIsInitialized;

#endif

