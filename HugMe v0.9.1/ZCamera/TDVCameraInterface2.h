//****************************************************************************
//
//  Copyright (C) 1997-2007 3DV SYSTEMS LTD.
//  All rights reserved.
//
//  Module Name:	tdvCameraInterface2.h
//  Project Name: 
//  Written By:		Gil Zigelman
//  Date:			04-06-2007
//  Description:	Main header file for DMachine SDK
//  Updated By:		Gil Zigelman
//	Last Updated:	
//	Version:		1.12
//****************************************************************************

#ifndef TDV_CAMERA_INTERFACE2_H__
#define TDV_CAMERA_INTERFACE2_H__

// TDVCameraInterfaceBase2 contains 
class CONNECTDM_DLL_API TDVCameraInterfaceBase2 {
public:
	TDVCameraInterfaceBase2() {};
	virtual ~TDVCameraInterfaceBase2() {};

	virtual HRESULT setCameraCommand(int cmdID, int value) = 0;
	virtual HRESULT getCameraCommandVal(int cmdID, int &value) const = 0;
	virtual HRESULT	getCameraCommandLimits(int cmdID, int &minValue, int &maxValue) const = 0;
	virtual HRESULT getVideoBuffer(int bufferType, unsigned char*& pBuffer) = 0;

	// changing the color camera settings. for changing on\off parameters use 
	// value 1\0 correspondingly.
	// valid only for ZSense camera
	void cmdColorCameraSetting(int iColorCommandID, int iValue);

	// get the current color camera setting. Returns false if iColorCommandID is invalid
	// valid only for ZSense camera
	bool getColorCameraSetting(int iColorCommandID, int &iValue) const;

	// get the limit values for color camera settings.
	// returns false if iColorCommandID is invalid
	// valid only for ZSense camera
	bool getColorCameraSettingLimit(int iColorCommandID, int& min, int& max) const;


	// turn on illumination
	void cmdCameraOn();
	// turn off illumination
	void cmdCameraOff();
	// get status of illumination
	bool isCameraOn() const;

	// get limit values for the brightness parameters
	int  getMaxBrightness() const;
	int  getMinBrightness() const;

	// update the primary camera brightness
	void cmdPrimaryBrightness(int value);
	// retrieve the current value of the primary camera brightness
	int	 getPrimaryBrightness() const;
	// update the secondary camera brightness
	void cmdSecondaryBrightness(int value);
	// retrieve the current value of the secondary camera brightness
	int	 getSecondaryBrightness() const;

	// get limit values for the width parameters
	int  getMaxWidth() const;
	int  getMinWidth() const;

	// update the primary camera width
	void cmdPrimaryWidth(int value);
	// retrieve the current value of the primary camera width
	int	 getPrimaryWidth() const;
	// update the secondary camera width
	void cmdSecondaryWidth(int value);
	// retrieve the current value of the secondary camera width
	int	 getSecondaryWidth() const;

	// get limit values for the distance parameters
	int  getMaxDistance() const;
	int  getMinDistance() const;

	// update the primary camera distance
	void cmdPrimaryDist(int value);
	// retrieve the current value of the primary camera distance
	int	 getPrimaryDist() const;
	// update the secondary camera distance
	void cmdSecondaryDist(int value);
	// retrieve the current value of the secondary camera distance
	int	 getSecondaryDist() const;

	// get limit values for the gain parameters. The gain parameter 
	// is only valid in certain camera models
	int  getMaxGain() const;
	int  getMinGain() const;

	// update the gain
	void cmdGain(int value);
	// retrieve the current value of the gain
	int	 getGain() const;


	// get limit values for the various filtering parameters. The only
	// filter which has different limit values is the softness filter.
	int  getMinFilterVal() const;
	int  getMaxFilterVal() const;
	// get limit values for the softness filter 
	int  getMinSoftnessVal() const;
	int  getMaxSoftnessVal() const;

	// update the binarization level
	void cmdBinVal(int value);
	// retrieve the current binarization level
	int	 getBinVal() const;
	// binarize the depth image (on\off)
	void cmdBinarizeOnOff(bool value);
	// is binarization on\off
	bool getBinarizeOnOff() const;
	// turn median filter on\off
	void cmdMedianOnOff(bool value);
	// is median filter on\off
	bool getMedianOnOff() const;
	// invert the depth video (on\off)
	void cmdInvertOnOff(bool value);
	// is the depth image inverted (on\off)
	bool getInvertOnOff() const;
	// turn the temporal filter on\off
	void cmdTemporalFilterOnOff(bool value);
	// is the temporal filter on\off
	bool getTemporalFilterOnOff() const;

	// turn the depth smooth filter on\off
	void cmdSmoothOnOff(bool value);
	// is the depth smooth filter on\off
	bool getSmoothOnOff() const;
	// update the depth smooth filter level
	void cmdSoftness(int value);
	// retrieve the current depth smooth filter level
	int  getSoftness() const;

	// turn the mask smooth filter on\off
	void cmdMaskSmoothOnOff(bool value);
	// is the mask smooth filter on\off
	bool getMaskSmoothOnOff() const;
	// update the mask smooth filter level
	void cmdMaskSoftness(int value);
	// retrieve the current mask smooth filter level
	int  getMaskSoftness() const;

	// turn the automatic background options on\off
	void cmdAutomaticBackgroundOnOff(bool value);
	// retrieve the current setting background status
	bool getAutomaticBackground();

	// turn the automatic depth positions for secondary window on\off
	void cmdAutomaticDepthPositionOnOff(bool value);
	// retrieve the current setting background status
	bool getAutomaticDepthPosition();

	// update the clean filter level
	void cmdClean(int value);
	// retrieve the current clean level
	int	 getClean() const;

	// saves the current camera parameters in the camera (index must be 0-7)
	// returns false for invalid index
	bool cmdSaveStatus(int index);
	// restores the camera parameters to those save in index (index must be 0-7)
	// returns false for invalid index
	bool cmdRestoreStatus(int index);

	// start the automatic find operation. Once the object is detected, the
	// automatic brightness will be turned on
	void cmdStartAutomaticFind();
	// are we during the "find" operation 
	bool isDuringAutomaticFind() const;
	// turn automatic tracking on. Automatic brightness will also be turned on 
	void cmdAutomaticTracking(bool state);
	// is the automatic trakcing option on\off
	bool isAutomaticTrackingOn() const;
	// turn automatic brightness on
	void cmdAutomaticBrightness(bool state);
	// is the automatic brightness turned on\off
	bool isAutomaticBrightnessOn() const;

	// forces the DMAchine to save an image (or two images if 
	// the camera supports color), as if "Save Img" was pressed
	// in the "camera control" application
	void cmdSaveImage();
	// forces the DMAchine to save an image (or two images if 
	// the camera supports color), as if "Save Avg" was pressed
	// in the "camera control" application
	void cmdSaveImageAverage();
	// forces the DMAchine to save a sequence of images, or AVI movies
	// as if saving sequence was pressed in the "camera control" application
	// if saveAVI is true, the DMachine will save AVI files, otherwise tga sequence
	void cmdSaveSequence(int length, bool saveAVI);

	// Freezes the video output of the DMachine, as if "freeze video" was pressed 
	// in the "camera control" application. If useAverage is true, then an 
	// average of the last 8 input frames is averages, and constantly outputted 
	// as the DMachine output.
	void cmdVideoFreeze(bool state, bool useAverage = false);


	// Enable\disable RGB processing. Saves CPU power when the RGB
	// is not needed.
	void cmdRGBProcessing(bool status);
	// Is RGB being processed? (might consume more CPU)
	bool getRGBProcessing() const;
	
	// enable\disable Confidence map processing. When enabled, you also receive
	// a frame for each depth frame containing confidence levels per pixel.
	// the higher the value is, the more accurate the corresponding 
	// depth level is.
	void cmdConfidenceMapProcessing(bool status);
	// is Confidence map being created? 
	// check cmdConfidenceMapProcessing() for more infomation
	bool getConfidenceMapProcessing() const;

	// enable\disable IR input source transfer for processing by the SDK user
	void cmdIRTransfer(bool status);
	// check if valid IR video source is provided.
	bool getIRTransfer() const;

	// enable\disable full resolution rgb transfer for processing by the SDK user
	void cmdRGBFullRes(bool status);
	// check if valid full resolution rgb video source is provided.
	bool getRGBFullRes() const;

	// must be called from within the callback function in order to get synchronized 
	// confidence map. Will contain valid data only after calling 
	// cmdConfidenceMapProcessing(true)
	bool getConfidenceFrame(unsigned char* &pConfidence);

	// must be called from within the callback function in order to get synchronized 
	// primary and secondary frames. Will contain valid data only after calling 
	// cmdIRTransfer(true)
	bool getSourceIRFrames(unsigned char* &pPrimary, unsigned char* &pSecondary);

	// must be called from within the callback function in order to get synchronized 
	// video data. Will contain valid data only after calling cmdRGBFullRes(true)
	bool getRGBFullResolutionFrame(unsigned char* &pRGBFullRes);
};


#endif
