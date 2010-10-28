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

#ifndef TDV_CAMERA_INTERFACE2_H
#define TDV_CAMERA_INTERFACE2_H

/**
 * TDVCameraInterfaceBase2 contains 
 */
class CONNECTDM_DLL_API TDVCameraInterfaceBase2 {

public:
	/**
	 * Constructor
	 */
	TDVCameraInterfaceBase2() {};

	/**
	 * Destructor
	 */
	virtual ~TDVCameraInterfaceBase2() {};

	/**
	 * Set the camera command
	 * @param cmdID The camera command ID
	 * @param value The value of the camera command
	 */
	virtual HRESULT setCameraCommand(int cmdID, int value) = 0;

	/**
	 * Retrieve the camera command
	 * @param cmdID The camera command ID
	 * @param value The value of the camera command
	 */
	virtual HRESULT getCameraCommandVal(int cmdID, int &value) const = 0;

	/**
	 * Retrieve the camera command limits
	 * @param cmdID The camera command ID
	 * @param minValue The minimal value of the camera command
	 * @param maxValue The maximal value of the camera command
	 */
	virtual HRESULT	getCameraCommandLimits(int cmdID, int &minValue, int &maxValue) const = 0;

	/**
	 * Retrieves the video buffer
	 * @param bufferType The type of video buffer
	 * @param pBuffer A reference to the video buffer
	 */
	virtual HRESULT getVideoBuffer(int bufferType, unsigned char*& pBuffer) = 0;

	/**
	 * Changing the color camera settings. for changing on or off parameters use 
	 * value 1 or 0 correspondingly.
	 * valid only for ZSense camera
	 * @param iColorCommandID The command for the color camera setting
	 * @param iValue The value of the command
	 */
	void cmdColorCameraSetting(int iColorCommandID, int iValue);

	/**
	 * Get the current color camera setting. Returns false if iColorCommandID is invalid
	 * valid only for ZSense camera
	 * @param iColorCommandID The command for the color camera setting
	 * @param iValue The value of the command
	 * @return true if successful
	 */
	bool getColorCameraSetting(int iColorCommandID, int &iValue) const;

	/**
	 * Get the limit values for color camera settings.
	 * returns false if iColorCommandID is invalid
	 * valid only for ZSense camera
	 * @param iColorCommandID The command for the color camera setting
	 * @param min The minimal color camera setting
	 * @param max The maximal color camera setting
	 * @return false if iColorCommandID is invalid
	 */
	bool getColorCameraSettingLimit(int iColorCommandID, int& min, int& max) const;

	/**
	 * Turn on illumination
	 */
	void cmdCameraOn();

	/**
	 * Turn off illumination
	 */
	void cmdCameraOff();

	/**
	 * Get status of illumination
	 * @return the status of illumination
	 */
	bool isCameraOn() const;

	/**
	 * Get maximal value for the brightness parameter
	 * @return the maximal value for the brightness parameter
	 */
	int  getMaxBrightness() const;

	/**
	 * Get minimal value for the brightness parameter
	 * @return the minimal value for the brightness parameter
	 */
	int  getMinBrightness() const;

	/**
	 * Update the primary camera brightness
	 * @param value The new brightness value
	 */
	void cmdPrimaryBrightness(int value);

	/**
	 * Retrieve the current value of the primary camera brightness
	 * @return the current value of the primary camera brightness
	 */
	int	 getPrimaryBrightness() const;

	/**
	 * Update the secondary camera brightness
	 * @param value The new brightness value
	 */
	void cmdSecondaryBrightness(int value);

	/**
	 * Retrieve the current value of the secondary camera brightness
	 * @return the current value of the secondary camera brightness
	 */
	int	 getSecondaryBrightness() const;

	/**
	 * Retrive maximum width
	 * @return the maximum width
	 */
	int  getMaxWidth() const;

	/**
	 * Retrive minimum width
	 * @return the minimum width
	 */
	int  getMinWidth() const;

	/**
	 * Update the primary camera width
	 * @param value The new width
	 */
	void cmdPrimaryWidth(int value);

	/**
	 * Retrieve the current value of the primary camera width
	 * @return the current value of the primary camera width
	 */
	int	 getPrimaryWidth() const;

	/**
	 * Update the secondary camera width
	 * @param value The new width
	 */
	void cmdSecondaryWidth(int value);

	/**
	 * Retrieve the current value of the secondary camera width
	 * @return the current value of the secondary camera width
	 */
	int	 getSecondaryWidth() const;

	/**
	 * Retrive maximum distance
	 * @return the maximum distance
	 */
	int  getMaxDistance() const;

	/**
	 * Retrive minimum distance
	 * @return the minimum distance
	 */
	int  getMinDistance() const;

	/**
	 * Update the primary camera distance
	 * @param value The new primary camera distance
	 */
	void cmdPrimaryDist(int value);

	/**
	 * Retrieve the current value of the primary camera distance
	 * @return the current value of the primary camera distance
	 */
	int	 getPrimaryDist() const;

	/**
	 * Update the secondary camera distance
	 * @param value The new secondary camera distance
	 */
	void cmdSecondaryDist(int value);

	/**
	 * Retrieve the current value of the secondary camera distance
	 * @return the current value of the secondary camera distance
	 */
	int	 getSecondaryDist() const;

	/**
	 * Get maximum value for the gain parameters. The gain parameter 
	 * is only valid in certain camera models
	 * @return the maximum value for the gain parameters
	 */
	int  getMaxGain() const;

	/**
	 * Get minimum value for the gain parameters. The gain parameter 
	 * is only valid in certain camera models
	 * @return the minimum value for the gain parameters
	 */
	int  getMinGain() const;

	/**
	 * Update the gain
	 * @param value The new gain value
	 */
	void cmdGain(int value);

	/**
	 * Retrieve the current value of the gain
	 * @return the current value of the gain
	 */
	int	 getGain() const;

	/**
	 * Get minimal value for the filtering parameters. The only
	 * filter which has different limit values is the softness filter.
	 * @return the minimal value for the filtering parameters
	 */
	int  getMinFilterVal() const;

	/**
	 * Get maximal value for the filtering parameters. The only
	 * filter which has different limit values is the softness filter.
	 * @return the maximal value for the filtering parameters
	 */
	int  getMaxFilterVal() const;

	/**
	 * Get minimal value for the softness filter
	 * @return the minimal value for the softness filter
	 */
	int  getMinSoftnessVal() const;

	/**
	 * Get maximal value for the softness filter
	 * @return the maximal value for the softness filter
	 */
	int  getMaxSoftnessVal() const;

	/**
	 * Update the binarization level
	 * @param value The new Binarization level
	 */
	void cmdBinVal(int value);

	/**
	 * Retrieve the current binarization level
	 * @return the current binarization level
	 */
	int	 getBinVal() const;

	/**
	 * Binarize the depth image (on or off)
	 * @param value True to enable the depth image
	 */
	void cmdBinarizeOnOff(bool value);

	/**
	 * Is binarization on or off
	 * @return true if binarization is on
	 */
	bool getBinarizeOnOff() const;
	
	/**
	 * Turn median filter on or off
	 * @param value True to turn median filter on
	 */
	void cmdMedianOnOff(bool value);

	/**
	 * Is median filter on or off
	 * @return true if median filtering is on
	 */
	bool getMedianOnOff() const;

	/**
	 * Invert the depth video (on or off)
	 * @param value True to invert the depth video
	 */
	void cmdInvertOnOff(bool value);

	/**
	 * Is the depth image inverted (on or off)
	 * @return true if the depth image is inverted
	 */
	bool getInvertOnOff() const;

	/**
	 * Turn the temporal filter on or off
	 * @param value True to turn the temporal filter on
	 */
	void cmdTemporalFilterOnOff(bool value);

	/**
	 * Is the temporal filter on or off
	 * @return true if temporal filter is on
	 */
	bool getTemporalFilterOnOff() const;

	/**
	 * Turn the depth smooth filter on or off
	 * @param value True to enable depth smooth filter
	 */
	void cmdSmoothOnOff(bool value);

	/**
	 * Is the depth smooth filter on or off
	 * @return true if depth smooth filter is on
	 */
	bool getSmoothOnOff() const;

	/**
	 * Update the depth smooth filter level
	 * @param value The new depth smooth filter level
	 */
	void cmdSoftness(int value);

	/**
	 * Retrieve the current depth smooth filter level
	 * @return the current depth smooth filter level
	 */
	int  getSoftness() const;

	/**
	 * Turn the mask smooth filter on or off
	 * @param value True to enable mask smooth filter
	 */
	void cmdMaskSmoothOnOff(bool value);

	/**
	 * Is the mask smooth filter on or off
	 * @return true is mask smooth filter is on
	 */
	bool getMaskSmoothOnOff() const;

	/**
	 * Update the mask smooth filter level
	 * @param value The new mask smooth filter level
	 */
	void cmdMaskSoftness(int value);

	/**
	 * Retrieve the current mask smooth filter level
	 * @return the current mask smooth filter level
	 */
	int  getMaskSoftness() const;

	/**
	 * Turn the automatic background options on or off
	 * @param value True to turn automatic background options on
	 */
	void cmdAutomaticBackgroundOnOff(bool value);

	/**
	 * Retrieve the current setting background status
	 * @return true if automatic background options are on
	 */
	bool getAutomaticBackground();

	/**
	 * Turn the automatic depth positions for secondary window on or off
	 * @param value True to enable automatic depth positions for secondary window
	 */
	void cmdAutomaticDepthPositionOnOff(bool value);

	/**
	 * Retrieve the automatic depth positions for secondary window
	 * @return true if automatic depth positions for secondary window is enabled
	 */
	bool getAutomaticDepthPosition();

	/**
	 * Update the clean filter level
	 * @param value The new clean filter level
	 */
	void cmdClean(int value);

	/**
	 * Retrieve the current clean level
	 * @ return the current clean level
	 */
	int	 getClean() const;

	/**
	 * Saves the current camera parameters in the camera (index must be 0-7)
	 * Returns false for invalid index
	 * @param index The index of the camera parameter
	 * @return false for invalid index
	 */
	bool cmdSaveStatus(int index);

	/**
	 * Restores the camera parameters to those save in index (index must be 0-7)
	 * returns false for invalid index
	 * @param index The index of the camera parameter
	 * @return false for invalid index
	 */
	bool cmdRestoreStatus(int index);

	/**
	 * Start the automatic find operation. Once the object is detected, the
	 * automatic brightness will be turned on
	 */
	void cmdStartAutomaticFind();

	/**
	 * Are we during the "find" operation
	 * @return true is in the find operation
	 */
	bool isDuringAutomaticFind() const;

	/**
	 * Turn automatic tracking on. Automatic brightness will also be turned on 
	 * @param state True to turn on automatic tracking
	 */
	void cmdAutomaticTracking(bool state);

	/**
	 * Is the automatic tracking option on or off
	 * @return true if automatic tracking is on
	 */
	bool isAutomaticTrackingOn() const;

	/**
	 * Turn automatic brightness on or off
	 * @param state True to enable automatic brightness
	 */
	void cmdAutomaticBrightness(bool state);

	/**
	 * Is the automatic brightness turned on or off
	 * @return true if automatic brightness is on
	 */
	bool isAutomaticBrightnessOn() const;

	/**
	 * Forces the DMAchine to save an image (or two images if 
	 * the camera supports color), as if "Save Img" was pressed
	 * in the "camera control" application
	 */
	void cmdSaveImage();

	/**
	 * Forces the DMAchine to save an image (or two images if 
	 * the camera supports color), as if "Save Avg" was pressed
	 * in the "camera control" application
	 */
	void cmdSaveImageAverage();

	/**
	 * Forces the DMAchine to save a sequence of images, or AVI movies
	 * as if saving sequence was pressed in the "camera control" application
	 * if saveAVI is true, the DMachine will save AVI files, otherwise tga sequence
	 * @param length The length of the sequence of images
	 * @param saveAVI If true, will save AVI files
	 */
	void cmdSaveSequence(int length, bool saveAVI);

	/**
	 * Freezes the video output of the DMachine, as if "freeze video" was pressed 
	 * in the "camera control" application. If useAverage is true, then an 
	 * average of the last 8 input frames is averages, and constantly outputted 
	 * as the DMachine output.
	 * @param state True to freeze the video output of the DMachine
	 * @param useAverage If true, anaverage of the last 8 input frames is averaged, and constantly outputted as the DMachine output. False by default
	 */
	void cmdVideoFreeze(bool state, bool useAverage = false);

	/**
	 * Enable or disable RGB processing. Saves CPU power when the RGB
	 * is not needed.
	 * @param status True to enable RGB processing
	 */
	void cmdRGBProcessing(bool status);

	/**
	 * Is RGB being processed? (might consume more CPU)
	 * @return true if RGB processing is enabled
	 */
	bool getRGBProcessing() const;
	
	/**
	 * Enable or disable Confidence map processing. When enabled, you also receive
	 * a frame for each depth frame containing confidence levels per pixel.
	 * the higher the value is, the more accurate the corresponding 
	 * depth level is.
	 * @param status True to enable confidence map processing
	 */
	void cmdConfidenceMapProcessing(bool status);

	/**
	 * Is Confidence map being created? 
	 * check cmdConfidenceMapProcessing() for more infomation
	 * @return true if confidence map processing is enabled
	 */
	bool getConfidenceMapProcessing() const;

	/**
	 * Enable or disable IR input source transfer for processing by the SDK user
	 * @param status True to enable IR input source transfer
	 */
	void cmdIRTransfer(bool status);

	/**
	 * Check if valid IR video source is provided.
	 * @return true if IR video source is provided
	 */
	bool getIRTransfer() const;

	/**
	 * Enable or disable full resolution rgb transfer for processing by the SDK user
	 * @param status True to enable full resolution RGB transfer
	 */
	void cmdRGBFullRes(bool status);

	/**
	 * Check if valid full resolution rgb video source is provided.
	 * @return true if full resolution RGB is provided
	 */
	bool getRGBFullRes() const;

	/**
	 * Must be called from within the callback function in order to get synchronized 
	 * confidence map. Will contain valid data only after calling 
	 * cmdConfidenceMapProcessing(true)
	 * @param pConfidence A reference to the confidence map
	 * @return true if successful
	 */
	bool getConfidenceFrame(unsigned char* &pConfidence);

	/**
	 * Must be called from within the callback function in order to get synchronized 
	 * primary and secondary frames. Will contain valid data only after calling 
	 * cmdIRTransfer(true)
	 * @param pPrimary The primary frame
	 * @param pSecondary The secondary frame
	 * @return true if successful
	 */
	bool getSourceIRFrames(unsigned char* &pPrimary, unsigned char* &pSecondary);

	/**
	 * Must be called from within the callback function in order to get synchronized 
	 * video data. Will contain valid data only after calling cmdRGBFullRes(true)
	 * @param pRGBFullRes The full resolution RGB frame
	 * @return true if successful
	 */
	bool getRGBFullResolutionFrame(unsigned char* &pRGBFullRes);
};


#endif

