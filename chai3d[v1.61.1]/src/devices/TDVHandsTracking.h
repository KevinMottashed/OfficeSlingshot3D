//****************************************************************************
//
//  Copyright (C) 1997-2006 3DV SYSTEMS LTD.
//  All rights reserved.
//
//  Module Name:	TDVHandsTracking.h
//  Date:			20-09-06
//  Description:	Main header file for Gestures SDK
//****************************************************************************

#pragma once
#include "TDVDeepMedia.h"

class TDVCameraInterfaceBase;
class TDVSkeletonCalibration;

#ifdef HANDSTRACKING_EXPORTS
#define HANDS_GESTURES_DLL __declspec(dllexport)
#else
#define HANDS_GESTURES_DLL __declspec(dllimport)
#endif

#ifndef _HANDS_SKEL_STATUS_ID_
enum TDVHandsSkelStatusID
{
	TDV_SUCCESS,
	TDV_ERR_NOT_CALIBRATED,
	TDV_ERR_NOT_INITIALIZED,
	TDV_ERR_NO_FRAME_SET,
	TDV_ERR_INVALID_IMAGE,
	TDV_ERR_INVALID_POSE,
	TDV_ERR_INVALID_PARAMETER,
	TDV_ERR_INTERNAL,
	TDV_ERR_UNKNOWN,
	TDV_USER_TOO_FAR,
	TDV_USER_TOO_CLOSE,
	TDV_USER_NOT_IN_FRAME,
	TDV_USER_ARMS_NOT_SIDEWAYS,
	TDV_CHANGE_CLEAN_LEVEL,
	TDV_USER_POSITION_OK,
	TDV_NUM_ERRORS

};
#define _HANDS_SKEL_STATUS_ID_
#endif

struct TDVHand
{
	bool finger[5]; // indicates which fingers are valid
	TDVImagePoint handPos; // position of the hand center
	TDVImagePoint fingerImagePos[5]; // position (in depth image) of each valid finger
	TDVImagePoint fingerDirection[5]; //The vector describing each finger. [dx,dy,Length]
};

struct _TDVHead
{
	float confidence;  //Confidence value of the head detection. Ranges [0.0,1.0].
	//int rotation;
	int radius;
	TDVImagePoint headPos; // position of the hand center
};


enum TDVTrackingParameters
{
	TDV_EnableFingerDetection,
	TDV_EnableHandTracking,
	TDV_EnableHeadTracking,
	TDV_EnableThrottleTracking,
	TDV_KalmanFilter,
	TDV_MirrorImage,
	TDV_TwoHandMode,
	TDV_Sensitivity,
	TDV_Focal_Length,
	TDV_Pixel_width,
	TDV_Pixel_height,
	TDV_Row_Length,
	TDV_Column_Length,
	TDV_Optical_center_x,
	TDV_Optical_center_y,
	TDV_UseColorInformation,
	TDV_CalibrationMode,      //Default: 0
	TDV_CalibrationDone,	  //Default: 0
	TDV_OptimalCleanValue,	  //Default: 2
	TDV_CameraElevationAngle, //Is set by calibration the process
};

class HANDS_GESTURES_DLL ITDVHandsTracking
{
public:


	ITDVHandsTracking();
	~ITDVHandsTracking();

	// API Functions

	// Init the tracker according to the specified depth image dimensions
	// Note: all depth images are 8 bit
	HRESULT Init(int width, int height,int RGBpixelSize);

	// Set the parameters for the 3D window
	HRESULT SetWindowParams(const TDVCameraWindowParams& window);


	// Process a new 3D frame
	// Don't change the frame buffer until the function returns.
	// Don't pass images in different sizes from frame to frame
	HRESULT ProcessFrame(const unsigned char* buf,const unsigned char* RGBbuf=0);

	// Retrieve the tracking data of the head.
	HRESULT GetHead(TDVHead& head);

	// Retrieve the gestures data of the left hand
	HRESULT GetFirstHandGesture(TDVHand& hand);
	
	// Retrieve the gestures data of the right hand
	HRESULT GetSecondHandGesture(TDVHand& hand);
	

	// Convert from depth image space to camera space [millimeters]
	// Right handed coordinate system with +X to the camera’s right,+Y the camera’s down.
	HRESULT ImagePoint2CameraPoint(const TDVImagePoint imagePoint, 
		TDVCameraPoint& spacePoint);

	// Old DMC support.
	// Resize depth 1:0.5 in xAxis (DMC compatibility)
	// the resulting image will be of size (width/2, height)
	// the buffers must be allocated prior to the call
	//HRESULT ResizeDepth(char* pSrcDepth, int width, int height, 
	//	char* pDstDepth );
	//HRESULT ResizeRGB(char* pSrcDepth, int width, int height, 
	//	char* pDstDepth,int nc = 4 );

	// Set the tracking parameters.
	HRESULT SetTrackingParameter(TDVTrackingParameters, float state);
	
	// Get the tracking parameters. Stored the specific parameter in the input argument "state".
	HRESULT GetTrackingParameter(TDVTrackingParameters params, float &state);

	// Default 3D camera settings for the ZSense and the DMC depth cameras.
	HRESULT SetZSENSE();
	//HRESULT SetDMC();

	//Messages from the SDK to the user (regarding position and working mode).
	HRESULT DeepMedia2UserMessage(unsigned char* buf, int&size);

	//Returns pointer to calibration image. 
	char* GetCalibrationImage(void);
	//Returns the size of the calibration image.
	void GetCalibrationImageSize(int&width,int&height);

	//Returns the pose of 3DV Systems's special bike throttle. 
	HRESULT GetBikeThrust(float& thrust);

private:

	//CAMERA CALIBRATION VARIABLES
	float m_fTDV_Focal_Length;
	float m_fTDV_Pixel_width;
	float m_fTDV_Pixel_height;
	float m_fTDV_Optical_center_x;
	float m_fTDV_Optical_center_y;
	int   m_iTDV_Row_length;
	int   m_iTDV_Column_length;
	bool  m_bCameraCalibration;



	//TDV_OptimalCleanValue
	int m_iOptimalCleanValue;

	//END camera calibration variables

	//Calls the TDVCalibration routine.
	//	HRESULT DoCalibrationStage(const unsigned char* buf,const unsigned char* RGBbuf);
	HRESULT DoCalibrationStage(const unsigned char* buf,const unsigned char* RGBbuf, int& CleanValue,TDVHandsSkelStatusID& RC );

	// Algorithm set to track two hands and detect the fingers' positions.
	HRESULT DefaultTracking();

	// Obsolete:
	// Set the two hands mode
	HRESULT SetTwoHandsMode(bool twoHandsMode);

	// Obsolete:
	// Set the sensitivity of the tracking values: 4-10
	HRESULT SetSensitivity(int sensitivity);

	// Set the parameters for the 3D camera according to what is stored in the
	// camera calibration variables listed below (which can be set by the calling SetTrackingParameter();) 
	HRESULT SetCameraParams(void);

	//If true tracking two hands, if false only tracking one hand.
	bool m_bTwoHandsMode;

	TDVSkeletonCalibration*	m_pTDVCalibration;
	TDVHandsSkelStatusID m_CalibrationStatus;

	//Flags for the calibration mode
	bool m_bCalibrationDone; //Whether calibration succeeded/completed.
	bool m_bCalibrationMode; //If true SDK is in calibration else in tracking mode.
};
