//****************************************************************************
//
//  Copyright (C) 1997-2007 3DV SYSTEMS LTD.
//  All rights reserved.
//
//  Module Name:	DepthCamera.cpp
//  Date:			1-9-2007
//  Description:	Implementation file for CDepthCamera class, which uses
//					the DMachine SDK to control the camera.
//  Updated By:		
//	Last Updated:			
//	Version:		1.00
//****************************************************************************

#include "DepthCamera.h"
#include "tdvcamerainterface.h"

// video callback function
void videoCallBack(unsigned char * pDepthBuf, unsigned char * pRgbBuf, 
				   int bufIndex, void* pObject)
{
	CDepthCamera *pDepthCamera = (CDepthCamera*)pObject;
	if (!pDepthCamera)
		return;
	TDVCameraInterfaceBase *pCamera = pDepthCamera->GetCameraInterface();
	if (!pCamera)
		return;

	unsigned char* pRGBFullRes	= NULL;
	unsigned char* pPrimary	= NULL;
	unsigned char* pSecondary = NULL;

	if (pDepthCamera->IsRGBFullResolutionEnabled())
		pCamera->getVideoBuffer(BUFFER_TYPE_RGB_FULL_RES, pRGBFullRes);
	if (pDepthCamera->IsIRVideoTransferEnabled())
	{
		pCamera->getVideoBuffer(BUFFER_TYPE_PRIMARY, pPrimary);
		pCamera->getVideoBuffer(BUFFER_TYPE_SECONDARY, pSecondary);
	}
	// there might be a small delay between the request for full res RGB\IR and the actual
	// time the transfer is started. Let's skip frames not synced
	if (pDepthCamera->IsRGBFullResolutionEnabled() && !pRGBFullRes)
		return;
	if (pDepthCamera->IsIRVideoTransferEnabled() && (!pPrimary || !pSecondary))
		return;
	pDepthCamera->SetFrames(pDepthBuf, pRgbBuf, pRGBFullRes, pPrimary, pSecondary);
}

// Command callback function
void cmdCallBack(int cmd, void* pObject)
{
	/*
	we only receive command if they value parameter is 
	different than currently set value

	only when the main DMachine application sends a message, we can 
	be sure the value was set in the camera. for example, when turning on the
	illumincation, only when CMD_ILLUMINCATION_ON is returned, the value
	was set in the camera.
	*/
	CDepthCamera *pDepthCamera = (CDepthCamera*)pObject;
	TDVCameraInterfaceBase *pCamera = pDepthCamera->GetCameraInterface();

	// at first run, or when the resolution has changed, we need to 
	// re-initialize the videos that we want to be processed
	if (cmd == CMD_CAMERA_RESOLUTION)
	{
		// might need to re-enable options that may have chenged to default after resolution change
		pDepthCamera->EnableRGBFullResolution(pDepthCamera->IsRGBFullResolutionEnabled()?1:0);
		pDepthCamera->EnableIRVideoTransfer(pDepthCamera->IsIRVideoTransferEnabled()?1:0);
		pDepthCamera->EnableWideDepthMode(pDepthCamera->IsWideDepthModeEnabled()?true:false);
	}
}



CDepthCamera::CDepthCamera(void)
{
	m_pCameraInterfaceBase = NULL;
	m_bEnableRGBFullResoltion = false;
	m_bEnableIRVideoTransfer = false;
	m_bEnableLargeDepthMode = false;
	m_hNewFrameReady = 0;
	m_hCriticalSection = 0;
	m_pDepthFrame = NULL;				
	m_pRGBFrame = NULL;				
	m_pRGBFullResFrame = NULL;			
	m_pPrimary = NULL;					
	m_pSecondary = NULL;			
	m_bIsInitialized = false;
}

CDepthCamera::~CDepthCamera(void)
{
	if (m_bIsInitialized)
	{
		EnableRGBFullResolution(false); // save on processing time...
		UnInitialize();
	}
}

// initialize the TDVCameraInterfaceBase object, and the callbacks
// in case the camera is not connected or DMachine is not running,
// it will wait the specified time until returning, to prevent program blocking.
// by default, it will wait indefinitely
bool CDepthCamera::Initialize(int timeout)
{
	if (m_bIsInitialized) // already initialized
		return true;
	m_bEnableRGBFullResoltion = false;
	m_pDepthFrame = m_pRGBFrame = m_pRGBFullResFrame = NULL;
	m_hCriticalSection = CreateEvent(NULL, false, true, NULL); 
	m_hNewFrameReady = CreateEvent(NULL, false, false, NULL); 
	m_pCameraInterfaceBase = new TDVCameraInterfaceBase();
	m_pCameraInterfaceBase->setCommandCallBack(cmdCallBack, this);
	m_pCameraInterfaceBase->setVideoCallBack(videoCallBack, this);
	int waitTime = timeout;
	const int waitResolution = 20; // wait in constant quantities, to save processing
	// wait for camera to get initialized
	while (!m_pCameraInterfaceBase->isCommActive())
	{
		if (waitTime != INFINITE && waitTime > 0)
		{
			Sleep(waitResolution);
			waitTime -= waitResolution;
			if (waitTime < 0)
				waitTime = 0;
		}
	}
	if (waitTime == 0) // timeout occured
	{
		UnInitialize();
		return false;
	}
	while (!m_pCameraInterfaceBase->isVideoActive())
	{
		if (waitTime != INFINITE && waitTime > 0)
		{
			Sleep(waitResolution);
			waitTime -= waitResolution;
			if (waitTime < 0)
				waitTime = 0;
		}
	}
	if (waitTime == 0) // timeout occured
	{
		UnInitialize();
		return false;
	}

	// camera is now ready for use. Lets set some default values
	m_pCameraInterfaceBase->setCameraCommand(CMD_PROCESSING_MODE, 1); // make sure we are in depth mode...
	m_pCameraInterfaceBase->setCameraCommand(CMD_ILLUMINATION_ON, 1); // make sure ilumination is on
	m_pCameraInterfaceBase->setCameraCommand(CMD_AUTO_DEPTH_POSITION, 1);
	m_pCameraInterfaceBase->setCameraCommand(CMD_MEDIAN, 1);
	m_pCameraInterfaceBase->setCameraCommand(CMD_TEMPORAL_FILTER, 0);
	SetDepthWindowPosition(100, 160);
	m_bIsInitialized = true;
	return true;
}

// uninitialize all internal objects.
void CDepthCamera::UnInitialize()
{
	if (m_pCameraInterfaceBase)
	{
		delete m_pCameraInterfaceBase;
		m_pCameraInterfaceBase = NULL;
	}
	if (m_hCriticalSection)
	{
		CloseHandle(m_hCriticalSection);
		m_hCriticalSection = 0;
	}	
	if (m_hNewFrameReady)
	{
		CloseHandle(m_hNewFrameReady);
		m_hNewFrameReady = 0;
	}
	m_bIsInitialized = false;
}


// currently the supported resolutions are: 
//		- RESOLUTION_320X240_30FPS 
//		- RESOLUTION_160X120_60FPS
// this function blocks until the resolution has been changed.
void CDepthCamera::ChangeResolution(int resolutionMode)
{
	int resolusion = 0;
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_CAMERA_RESOLUTION, resolusion);
	if (resolusion == resolutionMode)
	{	// nothing to change - resolution already valid
		return;
	}
	if (resolutionMode == RESOLUTION_320X240_30FPS || resolutionMode == RESOLUTION_160X120_60FPS)
	{
		m_pCameraInterfaceBase->setCameraCommand(CMD_CAMERA_RESOLUTION, resolutionMode);
	}
	// wait until new resolusion is set
	while (resolusion != resolutionMode)
		m_pCameraInterfaceBase->getCameraCommandVal(CMD_CAMERA_RESOLUTION, resolusion);
	// make sure all the videos and comm are valid again
	while (!m_pCameraInterfaceBase->isCommActive())
	{
		Sleep(500);
	}
	while (!m_pCameraInterfaceBase->isVideoActive())
	{
		Sleep(500);
	}
}

// get video frame parameters.
bool CDepthCamera::GetVideoSize(int& width, int& height, 
								int& rgbFullResWidth, int& rgbFullResHeight,
								int& depthPixelSize, int& rgbPixelSize) const
{
	if (!m_pCameraInterfaceBase->isVideoActive())
		return false;

	m_pCameraInterfaceBase->getVideoSize(width, height, rgbPixelSize, depthPixelSize);
	m_pCameraInterfaceBase->getRGBFullResSize(rgbFullResWidth, rgbFullResHeight, rgbPixelSize);
	return true;
}

// returns the maximal width possible for setting depth range, measured in centemeters
int CDepthCamera::GetMaxAllowedWidth() const
{
	int min, max;
	m_pCameraInterfaceBase->getCameraCommandLimits(CMD_PRIMARY_WIDTH, min, max);
	return max;
}


// set the depth range manually. This function chooses default settings
// that will best fit the distance\width specified
// the depth range will be between distance and distance+width, measured in centemeters
void CDepthCamera::SetDepthWindowPosition(int distance, int width)
{
	if (IsWideDepthModeEnabled())
		width = GetMaxAllowedWidth();
	m_pCameraInterfaceBase->setCameraCommand(CMD_AUTO_BRIGHTNESS, 0);
	m_pCameraInterfaceBase->setCameraCommand(CMD_PRIMARY_DISTANCE, distance);
	if (!m_bEnableLargeDepthMode) // in large depth mode the width is fixed to 300cm
		m_pCameraInterfaceBase->setCameraCommand(CMD_PRIMARY_WIDTH, width);
	// here we set default values for other settings when using this specified width
	int resolution;
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_CAMERA_RESOLUTION, resolution);
	int gain=0;
	int clean=2;

	int minBrightness;
	int maxBrightness;
	m_pCameraInterfaceBase->getCameraCommandLimits(CMD_PRIMARY_BRIGHTNESS, minBrightness, maxBrightness);
	int primaryBrightness = maxBrightness;
	int secondaryBrightness = maxBrightness;

	int maxGain;
	int minGain;
	m_pCameraInterfaceBase->getCameraCommandLimits(CMD_GAIN, minGain, maxGain);

	// assume we want not to saturate the video at the close range

	if (distance < 60)
	{
		primaryBrightness = int(0.5+.75*maxBrightness);
	}
	else if (distance < 100)
	{
	}
	else if (distance < 150)
	{
		gain = int(0.4*maxGain);
		clean  = 3;
		if (m_bEnableLargeDepthMode)
		{
			gain = int(0.6*maxGain);
		}
	}
	else if (distance < 200)
	{
		gain = int(0.5*maxGain);
		clean  = 4;
	}
	else
	{
		gain = maxGain;
		clean = 4;
	}


	switch (resolution)
	{
	case RESOLUTION_320X240_30FPS:
		break;	
	case RESOLUTION_160X120_60FPS:
		if (distance < 200 && !m_bEnableLargeDepthMode)
		{
			gain -= 40;
			if (gain < minGain)
			{
				primaryBrightness = int(0.6*primaryBrightness);
				gain = minGain;
			}
		}
		clean += 1;
		break;
	default:
		break;
	}
	m_pCameraInterfaceBase->setCameraCommand(CMD_GAIN, gain);
	m_pCameraInterfaceBase->setCameraCommand(CMD_CLEAN_VAL, clean);
	m_pCameraInterfaceBase->setCameraCommand(CMD_PRIMARY_BRIGHTNESS, primaryBrightness);
	m_pCameraInterfaceBase->setCameraCommand(CMD_SECONDARY_BRIGHTNESS, secondaryBrightness);
}


// get the depth range distance\width
// the depth range will be between distance and distance+width, measured in centemeters
void CDepthCamera::GetDepthWindowPosition(int &distance, int &width) const
{
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_PRIMARY_WIDTH, width);
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_PRIMARY_DISTANCE, distance);
}

// set the width specified, and positions the depth window 
// around the closest object to the camera
// the function blocks until object is found
void CDepthCamera::FindObjectUsingWidth(int depthWidth)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_PRIMARY_WIDTH, depthWidth);
	m_pCameraInterfaceBase->setCameraCommand(CMD_AUTO_FIND, 1);
	int value = 0;
	// wait for object to be found
	while (value==0)
	{
		Sleep(100);
		m_pCameraInterfaceBase->getCameraCommandVal(CMD_AUTO_FIND_COMPLETED, value);
	}
	int distance;
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_PRIMARY_DISTANCE, distance);
	SetDepthWindowPosition(distance, depthWidth); // set default settings for the found range
}


// enable\disable the transfer of RGB in 640x480 resolution. 
// When enabled, this consumes more CPU
void CDepthCamera::EnableRGBFullResolution(bool enable)
{
	m_bEnableRGBFullResoltion = enable;
	m_pCameraInterfaceBase->setCameraCommand(CMD_RGB_FULL_RES, m_bEnableRGBFullResoltion?1:0);
}

// check whether the RGB in full resolution is also transfered
bool CDepthCamera::IsRGBFullResolutionEnabled() const
{
	return m_bEnableRGBFullResoltion;
}

// enable\disable the transfer of IR Video (Primary and secondary IR).
// When enabled, this consumes more CPU
void CDepthCamera::EnableIRVideoTransfer(bool enable)
{
	m_bEnableIRVideoTransfer = enable;
	m_pCameraInterfaceBase->setCameraCommand(CMD_USING_IR_INPUT, m_bEnableIRVideoTransfer?1:0);
}

// check whether the IR video (primary and secondary) is also transfered
bool CDepthCamera::IsIRVideoTransferEnabled() const
{
	return m_bEnableIRVideoTransfer;
}

// when the user needs a 300cm depth window range, enable this mode.
// It will automatically place the depth range to support depth range starting
// from 100cm and ending at 400cm (distance from the camera).
void CDepthCamera::EnableWideDepthMode(bool enable)
{
	m_bEnableLargeDepthMode = enable;
	m_pCameraInterfaceBase->setCameraCommand(CMD_WIDE_DEPTH_MODE, m_bEnableLargeDepthMode?1:0);
	// set proper window settings for the wide depth mode
	SetDepthWindowPosition(100, GetMaxAllowedWidth());
}

// is the large depth mode enabled?
bool CDepthCamera::IsWideDepthModeEnabled() const
{
	return m_bEnableLargeDepthMode;
}

// enable\disable the "smooth" filter. also set the filter's intensity
void CDepthCamera::EnableDepthFilter(bool enable, int intensity)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_SMOOTH, enable?1:0);
	m_pCameraInterfaceBase->setCameraCommand(CMD_TEMPORAL_FILTER, enable?1:0);
	int min, max;
	m_pCameraInterfaceBase->getCameraCommandLimits(CMD_SOFTNESS_VAL, min, max);
	if (intensity > max) // make sure we do not set higher than allowed intensity value
		intensity = max;
	m_pCameraInterfaceBase->setCameraCommand(CMD_SOFTNESS_VAL, intensity);
}

// check if the depth filter is enabled, and if so, what is it's intensity
bool CDepthCamera::IsDepthFilterEnabled() const
{
	int iEnable;
	m_pCameraInterfaceBase->getCameraCommandVal(CMD_SMOOTH, iEnable);
	return iEnable?true:false;
}

// returns the next frames data. If RGB full resolution is not enabled,
// the pRGBFullResFrame will be NULL.
// This function will block until the next frame is ready, or until timeout
// is reached. If timeout occurs, the function returns false, otherwize true.
bool CDepthCamera::GetNextFrame(unsigned char* &pDepthFrame, 
								unsigned char* &pRGBFrame, 
								unsigned char* &pRGBFullResFrame,
								unsigned char* &pPrimary,
								unsigned char* &pSecondary,
								int timeout)
{
	if (WaitForSingleObject(m_hNewFrameReady, timeout) == WAIT_OBJECT_0)
	{
		// lock critical section while returning the frames, to make sure
		// RGB and depth are synced
		if (Lock())
		{
			pDepthFrame = m_pDepthFrame;
			pRGBFrame = m_pRGBFrame;
			pRGBFullResFrame = m_pRGBFullResFrame;
			pPrimary = m_pPrimary;
			pSecondary = m_pSecondary;
			Unlock();
			return true;
		}
	}
	return false;
}

// when you wish to get full control over other parameters, you can get the
// internal TDVCameraInterfaceBase object and handle things by yourself
TDVCameraInterfaceBase* CDepthCamera::GetCameraInterface() 
{
	return m_pCameraInterfaceBase;
}

void CDepthCamera::SetFrames(unsigned char* pDepthFrame, 
							 unsigned char* pRGBFrame, 
							 unsigned char* pRGBFullResFrame,
							 unsigned char* pPrimary,
							 unsigned char* pSecondary)
{
	if (Lock())
	{
		m_pDepthFrame = pDepthFrame;
		m_pRGBFrame = pRGBFrame;
		m_pRGBFullResFrame = pRGBFullResFrame;
		m_pPrimary = pPrimary;
		m_pSecondary = pSecondary;
		Unlock();
		// signal that a new frame is ready
		SetEvent(m_hNewFrameReady);
	}
}

bool CDepthCamera::Lock()
{
	if (WaitForSingleObject(m_hCriticalSection, INFINITE) == WAIT_OBJECT_0)
		return true;
	return false;
}

void CDepthCamera::Unlock()
{
	SetEvent(m_hCriticalSection);
}

void CDepthCamera::EnableTrack(bool enable)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_AUTO_TRACK, enable);
}


void CDepthCamera::EnableSmoothing(bool enable, double smoothVal)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_SMOOTH, enable);
	m_pCameraInterfaceBase->setCameraCommand(CMD_SOFTNESS_VAL, (int)smoothVal);
}

void CDepthCamera::SetCleanVal(double cleanVal)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_CLEAN_VAL, (int)cleanVal);
}

void CDepthCamera::EnableMedian(bool enable)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_MEDIAN, enable);
}

void CDepthCamera::FreezeVideo(bool freeze)
{
	m_pCameraInterfaceBase->setCameraCommand(CMD_FREEZE_VIDEO, freeze);
}