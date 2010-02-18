// HugMe.h: interface for the HugMe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUGME_H__6C1BAE81_D2E5_4084_8F76_37EC3F73A50D__INCLUDED_)
#define AFX_HUGME_H__6C1BAE81_D2E5_4084_8F76_37EC3F73A50D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Display device
#include "DisplayDeviceJacket.h"
#include "DisplayDeviceArmband.h"
#include "DisplayDeviceFinger.h"

// chai3d
#include "CWorld.h"
#include "CGenericObject.h"
#include "CGeneric3dofPointer.h"
#include "CFalcon3dofPointer.h"
#include "CPhantom3dofPointer.h"
#include "CFinger3dofPointer.h"
#include "CPrecisionTimer.h"
#include "CMaterial.h"
#include "CProxyGraphPointForceAlgo.h"
#include "CTriangle.h"
#include "CViewport.h"
#include "CDepthImage.h"
#include "CLight.h"
#include "CVector3d.h"
#include "CMatrix3d.h"
#include "CCamera.h"
#include "CLight.h"

///////////////////////////////////////////////////////
#include "HumanModel.h"

#include "DepthCamera.h"

#include "CellphoneSocket.h"

#include "tinyxml.h"

// image size
// But this will be dynamic based on user input in the future
#define DEPTH_IMAGE_WIDTH 320
#define DEPTH_IMAGE_HEIGHT 240

typedef enum tagDisplayDeviceType {
	TACTILE_JACKET = 0,
	TACTILE_ARMBAND,
	NUM_DISPLAY_DEVICE
} DisplayDeviceType;

#define TAG_TACTILE_JACKET 0x0001
#define TAG_TACTILE_ARMBAND 0x0002

typedef enum tagImageDeviceType{
	ZCAM = 0,
	NUM_IMAGE_DEVICE
} ImageDeviceType;

typedef enum tagInputDeviceType {
	FORCE_FEEDBACK_FALCON = 0,
	FORCE_FEEDBACK_PHANTOM,
	ONE_FINGER_GLOVE,
	CELLPHONE_TOUCHPAD,
	VIRTUAL_TOUCHPAD,
	NUM_INPUT_DEVICE
} InputDeviceType;

typedef struct tagHugMeConfig {
	// Grahphic rendering
	bool m_bShowDepthImage;
	bool m_bShowAvatar;
	bool m_bShowContactpoint;

	// Devices
	bool				m_bUseImageDevice;
	ImageDeviceType		m_typeImageDevice;
	bool				m_bUseInputDevice;
	InputDeviceType		m_typeInputDevice;
	bool				m_bUseFingerTactile;
	int					m_portNumFinger;
	bool				m_bUseDisplayDevice;
	int					m_typeDisplayDevice;
	int					m_portNum[NUM_DISPLAY_DEVICE];

	// Touchpad
	HumanPart			m_humanPartToTouch;	 
} HugMeConfig;

typedef struct tagHugMeDevice {
	bool m_bOnImageDevice;
	ImageDeviceType m_typeOnImageDevice;
	bool m_bOnInputDevice;
	InputDeviceType m_typeOnInputDevice;
	bool m_bOnDisplayDevice;
	int m_typeOnDisplayDevice;
} HugMeDevice;

typedef int HugMe_Err;

#define HUGME_ERR_NO_ERR			0x00000000
#define HUGME_ERR_NO_HAPTIC_DEVICE	0x00000001
#define HUGME_ERR_NO_DEPTH_CAMERA	0x00000002
#define HUGME_ERR_NO_CELLPHONE_TOUCHPAD 0x00000004l

class HugMe  
{
public:
	// Is the HugMeSystem started?
	bool m_bIsStarted;
	// device configuration for HugMe system
	HugMeConfig m_config;
	// device status for HugMe system
	HugMeDevice m_device;

	// Device names
	char m_nameImageDevice[NUM_IMAGE_DEVICE][50];
	char m_nameInputDevice[NUM_INPUT_DEVICE][50];
	char m_nameDisplayDevice[NUM_DISPLAY_DEVICE][50];
	char m_nameBodyPart[TOTAL_NUM_PART][50];

	/////////////////////////////////////
	// CHAI3D remote world
	//
	// virtual world
	cWorld* m_worldRemote;
	// viewport
	cViewport* m_viewport;
	// camera
	cCamera* m_camera;
	// light
	cLight* m_light;
	// An object that represent the contact point in red
	cMesh* m_pContactPoint;
	// depth image object
	cDepthImage* m_depthVideo;
	// The high-precision timer that's used (optionally) to run
	// the haptic loop
	cPrecisionTimer m_timer;
	// to get the proxy position
	cProxyGraphPointForceAlgo* m_proxyAlgo;
	// Window handle where the remote world will be rendered on
	HWND m_hWnd;
	/////////////////////////////////////
	
	/////////////////////////////////////
	// Human models that represent the local user and the remote user
	// The local model gets the color image and compute the pose of local user via ARToolKit
	// The remote model gets the pose of the remote user via network
	CHumanModel	m_localHumanModel;
	CHumanModel	m_remoteHumanModel;

	/////////////////////////////////////
	// Devices that are needed for HugMe
	// Depth camera : ZCam
	// -> Captures the local user in 2.5D
	// Input devices : 3 dof force feedback device (Falcon, phantom, etc), One finger glove, Google phone
	// -> Captures the local user's hand movement and give back force or tactile feedback.
	// Tactile display : Tactle Jacket, Tactile Armband
	// -> Gives tactile feedback
	// Interface to the depth camera
	CDepthCamera * m_depthCamera;
	// initialize image buffers
	unsigned char * pDepth;
	unsigned char * pRGB;
	unsigned char * pRGBFullRes;
	unsigned char * pPrimary;
	unsigned char * pSecondary;
	int m_depthWidth;
	int m_depthHeight;
	int dSize;
	int rgbSize;
	// camera parameter
	int m_primaryDistance;
	int m_depthDistance;
	double m_focalLength;
	double m_pixelWidth;
	////////////////////////////////////////////////////////////////////////////
	unsigned char * m_pLocalVideo;
	unsigned char * m_pLocalDepth;
	unsigned char * m_pRemoteVideo;
	unsigned char * m_pRemoteDepth;
	////////////////////////////////////////////////////////////////////////////
	// Interface to the haptic device or one finger tactile glove...
	cGeneric3dofPointer* m_pInputDevice;
	DisplayDeviceFinger* m_pDisplayDeviceFinger;
	// Interface to the tactile displays
	DisplayDeviceJacket* m_pDisplayDeviceJacket;
	DisplayDeviceArmband* m_pDisplayDeviceArmband;
	// Interface to a cellphone (socket)
	CellphoneSocket* m_pCellphoneSocket;

	HugMe(HWND a_hWnd);
	virtual ~HugMe();

	void SaveConfig(void);
	int LoadConfig(void);

	void SetConfig(HugMeConfig a_config)
	{
		m_config = a_config;
	}
	HugMeConfig GetConfig(void)
	{
		return m_config;
	}

	int Initialize(void);
	void Uninitialize(void);
	void CheckDevices(void);
	bool CheckInputDevice(void);
	bool CheckImageDevice(void);
	
	// Build the remote world that shows the remote person and its avatar
	void InitRemoteWorld();
	void UninitRemoteWorld();

	HugMe_Err InitInputDevice(void);
	void UninitInputDevice(void);
	HugMe_Err InitImageDevice(void);
	void UninitImageDevice();
	HugMe_Err InitRemoteDepthVideo(void);
	void UninitRemoteDepthVideo(void);
	HugMe_Err InitDisplayDevice(void);
	void UninitDisplayDevice(void);

	void CaptureFrame(void);

	void ShowContactPoint(bool a_show);
};

#endif // !defined(AFX_HUGME_H__6C1BAE81_D2E5_4084_8F76_37EC3F73A50D__INCLUDED_)
