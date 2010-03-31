//****************************************************************************
//
//  Copyright (C) 1997-2007 3DV SYSTEMS LTD.
//  All rights reserved.
//
//  Module Name:	tdvCameraInterface.h
//  Project Name: 
//  Written By:		Amir Kaplan
//  Date:			19-12-2004
//  Description:	Main header file for DMachine SDK
//  Updated By:		Gil Zigelman
//	Last Updated:	12-03-2007		
//	Version:		1.09
//****************************************************************************

#ifndef TDV_CAMERA_INTERFACE_H__
#define TDV_CAMERA_INTERFACE_H__

////////////////////////////////////////////////////////////
//                DLL definitions
// 
//    When using the lib version of the SDK, and not the dll,
//    you must define CONNECT_DM_LIB in your project settings.

#ifndef CONNECT_DM_LIB
#ifdef CONNECTDM_DLL_EXPORTS
#define CONNECTDM_DLL_API __declspec(dllexport)
#else
#define CONNECTDM_DLL_API __declspec(dllimport)
#endif
#else
#define CONNECTDM_DLL_API
#endif

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED


// Resolution and frame rate settings definitions
#define RESOLUTION_320X240_30FPS	2
#define RESOLUTION_160X120_60FPS	3

// Definitions for saving a sequence (AVI or tga files)
#define SEQUENCE_TYPE_TGA			0
#define SEQUENCE_TYPE_AVI			1

// Definitions for Buffer access from withing the video callback
// using the getVideoBuffer(...) method
#define BUFFER_TYPE_PRIMARY			1
#define BUFFER_TYPE_SECONDARY		2
#define BUFFER_TYPE_CONFIDENCE		3
#define BUFFER_TYPE_RGB_FULL_RES	4



// Camera Parameters ID's

#define CMD_ILLUMINATION_ON			1	// lasers On\Off
#define CMD_PRIMARY_BRIGHTNESS		2	// primary camera brightness
#define CMD_SECONDARY_BRIGHTNESS	3	// secondayr camera brightness
#define CMD_PRIMARY_WIDTH			4	// primary camera width
#define CMD_SECONDARY_WIDTH			5	// secondary camera width
#define CMD_PRIMARY_DISTANCE		6	// primary camera Distance
#define CMD_SECONDARY_DISTANCE		7	// secondayr Camera Distance
#define CMD_GAIN					8	// gain (not always valid - camera dependant)

// Filtering Parameters ID's

#define CMD_BIN_VAL					10	// binarization level for the binarized image
#define CMD_BINARIZE				11	// binarize depth image on\off
#define CMD_MEDIAN					12	// media filter on\off
#define CMD_INVERT					13	// invert depth on\off
#define CMD_TEMPORAL_FILTER			14	// temporal filter on\off
#define CMD_SMOOTH					15	// depth smooth filter on\off
#define CMD_SOFTNESS_VAL			16	// depth smooth filter level
#define CMD_CLEAN_VAL				17	// clear filter level
#define CMD_MASK_SMOOTH				18	// clear filter level
#define CMD_MASK_SOFTNESS_VAL		19	// depth smooth filter level

// Automation Parameters ID's

#define CMD_AUTO_FIND				20	// start object detection
#define CMD_AUTO_FIND_COMPLETED		21	// object was found
#define CMD_AUTO_BRIGHTNESS			22	// automatic brightness control on\off
#define CMD_AUTO_TRACK				23	// automatic tracking control on\off
#define CMD_AUTO_BACKGROUND			24	// automatic secondary window adjustment in mask mode
#define CMD_AUTO_DEPTH_POSITION		25	// automatic secondary window adjustment in depth mode

// Save and restore settings

#define CMD_SAVE_STATUS				30
#define CMD_RESTORE_STATUS			31

// Internal Image saving
#define CMD_SAVE_IMAGE				40
#define CMD_SAVE_IMAGE_AVG			41
#define CMD_SAVE_SEQUENCE			42
#define CMD_SAVE_SEQUENCE_TYPE		43


// Processing parameters
#define CMD_FREEZE_VIDEO			45
#define CMD_RGB_PROCESSING			46
#define CMD_USING_CONFIDENCE_MAP	47
#define CMD_USING_IR_INPUT			48
#define CMD_RGB_FULL_RES			49
#define CMD_CAMERA_RESOLUTION		50
#define CMD_PROCESSING_MODE			51	// values:  1 = depth mode; 0 = mask mode
#define CMD_WIDE_DEPTH_MODE			52
#define CMD_ELEC_FREQUENCY			53	// values: 1 = 60Hz; 0 = 50Hz

// Color Camera parameters. Can be changed through cmdColorCameraSetting(...)
// the limit values can be retrieved through getColorCameraSettingLimit(...)
// Color camera parameters changes are valid only for ZSense camera
#define CMD_COLOR_AGC				1000		// AGC on\off
#define CMD_COLOR_GAIN				1001		// Gain Value
#define CMD_COLOR_AEC				1002		// Automatic exposure control on\off
#define CMD_COLOR_EXPOSURE			1003		// EXposure value
#define CMD_COLOR_FLICKER			1004		// 0=auto, 1=50Hz, 2=60Hz
#define CMD_COLOR_AWB				1005		// Automatic white balance on\off
#define CMD_COLOR_R_GAIN			1006		// Manual R gain
#define CMD_COLOR_B_GAIN			1007		// Manual B gain
#define CMD_COLOR_AUTO_SHARPNESS	1008		// Automatic sharpness control on\off
#define CMD_COLOR_SHARPNESS			1009		// Sharpness value
#define CMD_COLOR_AUTO_CONTRAST		1010		// Auto contrast on\off
#define CMD_COLOR_CONTRAST			1011		// Manual contrast value
#define CMD_LONG_EXPOSURE			1012		// long exposure for RGB (lower frame rate)

#include "TDVCameraInterface2.h"

// forward declaration
class TDVVideoThread;
class TDVCommThread;


// video callback definition
typedef void (__cdecl * videoCallBackFunc)(unsigned char * pDepthBuf, 
								   unsigned char * pRgbBuf, 
								   int bufIndex,
								   void* pObject);

// communication callback definition
typedef void (__cdecl * cmdCallBackFunc)(int cmd, void* pObject);


// main camera control interface
class CONNECTDM_DLL_API TDVCameraInterfaceBase : public TDVCameraInterfaceBase2 {
public:
	TDVCameraInterfaceBase();
	virtual ~TDVCameraInterfaceBase();

	// check status of video. Should be valid once the main DMachine application is active
	// and the 1394 video connectors of the camera are set correctly
	bool isVideoActive() const;
	// check the status of communication with the main DMachine application
	bool isCommActive() const;

	// initialize the video callback. pObject will be a parameter to the callback.
	// this enables passing of a specific parameter to the callback function.
	void setVideoCallBack(videoCallBackFunc videoCBF, void* pObject = 0);

	// initialize the communication callback. pObject will be a parameter to the callback.
	// this enables passing of a specific parameter to the callback function.
	void setCommandCallBack(cmdCallBackFunc cmdCBF, void* pObject = 0);

	// get various parameters for the video: width, height, pixel size for 
	// the depth video and for the rgb info. returns false in case there is no
	// video connection yet (for example, if the main DMachine application is not running)
	bool getVideoSize(int &width, int& height,
					  int &rgbPixelSize,
					  int &depthPixelSize) const;

	// It is possible to get full resolution RGB even when working in low resolution IR.
	// The width\height\pixel size can be retrieved using getRGBFullResSize
	bool getRGBFullResSize(int &width, int& height, int &rgbPixelSize);

	// send a specific command (cmdID = CMD_*****) to the camera
	virtual HRESULT setCameraCommand(int cmdID, int value);
	// get the current value for specific camera parameter (cmdID = CMD_*****)
	virtual HRESULT getCameraCommandVal(int cmdID, int &value)const ;
	// get the value range for specific camera parameter (cmdID = CMD_*****)
	virtual HRESULT	getCameraCommandLimits(int cmdID, int &minValue, int &maxValue) const;

	// must be called from within the video callback. If the frame type 
	// transfer was enabled, you will be able to get the buffer.
	virtual HRESULT getVideoBuffer(int bufferType, unsigned char*& pBuffer);

	// for 3DV Systems maintenance use only
	void SetMaintenanceCmd(int ID, int val);
private:

	// internal objects
	TDVVideoThread* m_pVideoThread;
	TDVCommThread * m_pCommThread;
};


#endif
