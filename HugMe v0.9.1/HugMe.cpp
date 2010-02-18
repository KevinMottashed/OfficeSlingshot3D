// HugMe.cpp: implementation of the HugMe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "HugMe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// chai library //
#ifndef M_PI
#define M_PI 3.1415926535898
#endif

void haptic_iteration(void* param);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HugMe::HugMe(HWND a_hWnd)
{
	m_hWnd = a_hWnd;

	m_worldRemote = NULL;
	m_viewport = NULL;
	m_camera = NULL;
	m_light = NULL;
	m_pContactPoint = NULL;
	m_depthVideo = NULL;
	m_pInputDevice = NULL;
	m_proxyAlgo = NULL;

	m_config.m_bShowAvatar = false;
	m_config.m_bShowContactpoint = false;
	m_config.m_bShowDepthImage = false;

	m_config.m_bUseImageDevice = false;
	m_config.m_typeImageDevice = ZCAM;
	m_config.m_bUseInputDevice = false;
	m_config.m_typeInputDevice = FORCE_FEEDBACK_FALCON;
	m_config.m_bUseFingerTactile = true;
	m_config.m_portNumFinger = 1;
	m_config.m_bUseDisplayDevice = false;
	m_config.m_typeDisplayDevice = TAG_TACTILE_JACKET;
	m_config.m_portNum[TACTILE_JACKET] = 97;
	m_config.m_portNum[TACTILE_ARMBAND] = 98;

	m_config.m_humanPartToTouch = RIGHT_UPPER_ARM;

	if( !LoadConfig() ) {
		SaveConfig();
	}

	strcpy(m_nameImageDevice[ZCAM], "Zcam");

	strcpy(m_nameInputDevice[FORCE_FEEDBACK_FALCON],	"Falcon");
	strcpy(m_nameInputDevice[FORCE_FEEDBACK_PHANTOM],	"Phantom");
	strcpy(m_nameInputDevice[ONE_FINGER_GLOVE],			"Finger glove");
	strcpy(m_nameInputDevice[CELLPHONE_TOUCHPAD],		"Cellphone touchpad");
	strcpy(m_nameInputDevice[VIRTUAL_TOUCHPAD],			"Virtual touchpad");

	strcpy(m_nameDisplayDevice[TACTILE_JACKET],		"Tactile Jacket");
	strcpy(m_nameDisplayDevice[TACTILE_ARMBAND],	"Tactile Armband");

	strcpy(m_nameBodyPart[CHEST], "Chest");
	strcpy(m_nameBodyPart[RIGHT_UPPER_ARM], "Upper arm");
	strcpy(m_nameBodyPart[RIGHT_LOWER_ARM], "Forearm");

	// Initialize depth camera parameters
	m_primaryDistance = 50;
	m_depthDistance = 250;
	m_focalLength = 0.47;
	m_pixelWidth = 0.00112;

	m_pDisplayDeviceJacket = NULL;

	// All the devices are off
	m_device.m_bOnImageDevice = false;
	m_device.m_bOnInputDevice = false;
	m_device.m_bOnDisplayDevice = false;

	//
	m_bIsStarted = false;

	m_pLocalVideo = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];
	m_pLocalDepth = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];
#if USE_H263
	m_pRemoteVideo = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 3];
	m_pRemoteDepth = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];
#else
	m_pRemoteVideo = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];
	m_pRemoteDepth = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];
#endif

	InitRemoteWorld();

	//
	m_pCellphoneSocket = NULL;
}

HugMe::~HugMe()
{
	//m_pTactileDisplay->closeSerialPort();
	//delete m_pTactileDisplay;

	if(m_config.m_bUseInputDevice)
		UninitInputDevice();

	UninitRemoteWorld();

	delete [] m_pLocalVideo;
	delete [] m_pLocalDepth;
	delete [] m_pRemoteVideo;
	delete [] m_pRemoteDepth;
}

void HugMe::SaveConfig(void)
{
	TiXmlDocument m_configManager;

	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	m_configManager.LinkEndChild( decl );

	TiXmlElement * element_graphic_rendering = new TiXmlElement( "GraphicRendering" );
	if( m_config.m_bShowAvatar )
		element_graphic_rendering->SetAttribute("ShowAvatar", "Yes");
	else
		element_graphic_rendering->SetAttribute("ShowAvatar", "No");
	if( m_config.m_bShowContactpoint )
		element_graphic_rendering->SetAttribute("ShowContactpoint", "Yes");
	else
		element_graphic_rendering->SetAttribute("ShowContactpoint", "No");
	if( m_config.m_bShowDepthImage )
		element_graphic_rendering->SetAttribute("ShowDepthImage", "Yes");
	else
		element_graphic_rendering->SetAttribute("ShowDepthImage", "No");
	m_configManager.LinkEndChild( element_graphic_rendering );

	TiXmlElement * element_image_device = new TiXmlElement( "ImageDevice" );
	if( m_config.m_bUseImageDevice ) {
		element_image_device->SetAttribute("UseImageDevice", "Yes");
		TiXmlElement * element_device_type = new TiXmlElement( "DeviceType" );
		TiXmlText * text;
		if( m_config.m_typeImageDevice == ZCAM )
			 text = new TiXmlText( "ZCam" );
		element_device_type->LinkEndChild( text );
		element_image_device->LinkEndChild( element_device_type );
	} else
		element_image_device->SetAttribute("UseDepthCamera", "No");
	m_configManager.LinkEndChild( element_image_device );

	TiXmlElement * element_input_device = new TiXmlElement( "InputDevice" );
	if( m_config.m_bUseInputDevice ) {
		element_input_device->SetAttribute("UseInputDevice", "Yes");
		TiXmlElement * element_device_type = new TiXmlElement( "DeviceType" );
		TiXmlText * text;
		if( m_config.m_typeInputDevice == FORCE_FEEDBACK_FALCON )
			text = new TiXmlText( "Falcon" );
		else if( m_config.m_typeInputDevice == FORCE_FEEDBACK_PHANTOM )
			text = new TiXmlText( "Phantom" );
		else if( m_config.m_typeInputDevice == ONE_FINGER_GLOVE ) {
			text = new TiXmlText( "FingerGlove" );
			if(m_config.m_bUseFingerTactile)
				element_device_type->SetAttribute("UseFingerTactile", "Yes");
			else
				element_device_type->SetAttribute("UseFingerTactile", "No");
			element_device_type->SetAttribute("PortNum", m_config.m_portNumFinger);
		}
		else if( m_config.m_typeInputDevice == CELLPHONE_TOUCHPAD )
			text = new TiXmlText( "CellphoneTouchpad" );
		else if( m_config.m_typeInputDevice == VIRTUAL_TOUCHPAD )
			text = new TiXmlText( "VirtualTouchpad" );
		element_device_type->LinkEndChild( text );
		element_input_device->LinkEndChild( element_device_type );
	} else {
		element_input_device->SetAttribute("UseInputDevice", "No");
	}
	m_configManager.LinkEndChild( element_input_device );

	TiXmlElement * element_display_device = new TiXmlElement( "DisplayDevice" );
	if( m_config.m_bUseDisplayDevice ) {
		element_display_device->SetAttribute("UseDisplayDevice", "Yes");
		if( m_config.m_typeDisplayDevice & TAG_TACTILE_JACKET ) {
			TiXmlElement * element_device_type = new TiXmlElement( "DeviceType" );
			//
			element_device_type->SetAttribute( "PortNum", m_config.m_portNum[TACTILE_JACKET]);
			//
			TiXmlText * text = new TiXmlText( "TactileJacket" );
			element_device_type->LinkEndChild( text );
			element_display_device->LinkEndChild( element_device_type );
		}
		if( m_config.m_typeDisplayDevice & TAG_TACTILE_ARMBAND ) {
			TiXmlElement * element_device_type = new TiXmlElement( "DeviceType" );
			//
			element_device_type->SetAttribute( "PortNum", m_config.m_portNum[TACTILE_ARMBAND]);
			//
			TiXmlText * text = new TiXmlText( "TactileArmband" );
			element_device_type->LinkEndChild( text );
			element_display_device->LinkEndChild( element_device_type );
		}
		
	} else {
		element_display_device->SetAttribute("UseDisplayDevice", "No");
	}
	m_configManager.LinkEndChild( element_display_device );

	m_configManager.SaveFile("config_HugMe.cfg");
}

int HugMe::LoadConfig(void)
{
	TiXmlDocument m_configManager;

	if( !m_configManager.LoadFile("config_HugMe.cfg") )
		return -1;
	
	// Graphic rendering section
	TiXmlElement* element_graphic_rendering = m_configManager.FirstChildElement( "GraphicRendering" );
	// Avatar
	const char * strShowAvatar = element_graphic_rendering->Attribute( "ShowAvatar" );
	if(strShowAvatar)
		if( strcmp(strShowAvatar, "Yes") == 0 )
			m_config.m_bShowAvatar = true;
		else m_config.m_bShowAvatar = false;
	else m_config.m_bShowAvatar = false;
	// Contact point
	const char * strShowContactpoint = element_graphic_rendering->Attribute( "ShowContactpoint" );
	if(strShowContactpoint)
		if( strcmp(strShowContactpoint, "Yes") == 0 )
			m_config.m_bShowContactpoint = true;
		else m_config.m_bShowContactpoint = false;
	else m_config.m_bShowContactpoint = false;
	// Depth image
	const char * strShowDepthImage = element_graphic_rendering->Attribute( "ShowDepthImage" );
	if(strShowDepthImage)
		if( strcmp(strShowDepthImage, "Yes") == 0 )
			m_config.m_bShowDepthImage = true;
		else m_config.m_bShowDepthImage = false;
	else m_config.m_bShowDepthImage = false;
	
	// Image device section
	TiXmlElement* element_image_device = m_configManager.FirstChildElement( "ImageDevice" );
	const char * strUseImageDevice = element_image_device->Attribute( "UseImageDevice" );
	if(strUseImageDevice) {
		if( strcmp(strUseImageDevice, "Yes") == 0 ) {
			m_config.m_bUseImageDevice = true;
			TiXmlElement* element_device_type = element_image_device->FirstChildElement( "DeviceType" );
			if(element_device_type) {
				if( strcmp(element_device_type->GetText(), "ZCam") == 0 )
					m_config.m_typeImageDevice = ZCAM;
			} else {//error: No device name
			}
		} else
			m_config.m_bUseImageDevice = false;
	} else m_config.m_bUseImageDevice = false;

	// Input device section
	TiXmlElement* element_input_device = m_configManager.FirstChildElement( "InputDevice" );
	const char * strUseInputDevice = element_input_device->Attribute( "UseInputDevice" );
	if(strUseInputDevice) {
		if( strcmp(strUseInputDevice, "Yes") == 0 ) {
			m_config.m_bUseInputDevice = true;
			TiXmlElement* element_device_type = element_input_device->FirstChildElement( "DeviceType" );
			if( element_device_type ) {
				const char * strDeviceName = element_device_type->GetText();
				if( strDeviceName ) {
					if( strcmp(strDeviceName, "Falcon") == 0 )
						m_config.m_typeInputDevice = FORCE_FEEDBACK_FALCON;
					else if( strcmp(strDeviceName, "Phantom") == 0 )
						m_config.m_typeInputDevice = FORCE_FEEDBACK_PHANTOM;
					else if( strcmp(strDeviceName, "FingerGlove") == 0 ) {
						m_config.m_typeInputDevice = ONE_FINGER_GLOVE;
						const char * strUseFingerTactile = element_device_type->Attribute( "UseFingerTactile" );
						if(strUseFingerTactile)
							if( strcmp(strUseFingerTactile, "Yes") == 0 )
								m_config.m_bUseFingerTactile = true;
							else m_config.m_bUseFingerTactile = false;
						element_device_type->Attribute( "PortNum", &m_config.m_portNumFinger );
						if(m_config.m_portNumFinger < 1)
							m_config.m_portNumFinger = 1;
						if(m_config.m_portNumFinger > 99)
							m_config.m_portNumFinger = 99;
					}
					else if( strcmp(strDeviceName, "CellphoneTouchpad") == 0 )
						m_config.m_typeInputDevice = CELLPHONE_TOUCHPAD;
					else if( strcmp(strDeviceName, "VirtualTouchpad") == 0 )
						m_config.m_typeInputDevice = VIRTUAL_TOUCHPAD;
				} else {//error: No device name
					m_config.m_typeInputDevice = FORCE_FEEDBACK_FALCON;
				}
			} else {//error: No device name
				m_config.m_typeInputDevice = FORCE_FEEDBACK_FALCON;
			}
		} else
			m_config.m_bUseInputDevice = false;
	} else m_config.m_bUseInputDevice = false;
		
	// Display device section
	TiXmlElement* element_display_device = m_configManager.FirstChildElement( "DisplayDevice" );
	const char * strUseDisplayDevice = element_display_device->Attribute( "UseDisplayDevice" );
	if( strUseDisplayDevice) {
		if( strcmp(strUseDisplayDevice, "Yes") == 0 ) {
			m_config.m_bUseDisplayDevice = true;
			TiXmlElement* element_device_type = element_display_device->FirstChildElement( "DeviceType" );
			while(element_device_type) {
				const char * strDeviceName = element_device_type->GetText();
				if( strDeviceName ) {
					if( strcmp(strDeviceName, "TactileJacket") == 0 ) {
						m_config.m_typeDisplayDevice |= TAG_TACTILE_JACKET;
						element_device_type->Attribute( "PortNum", &m_config.m_portNum[TACTILE_JACKET] );
						if(m_config.m_portNum[TACTILE_JACKET] < 1)
							m_config.m_portNum[TACTILE_JACKET] = 1;
						if(m_config.m_portNum[TACTILE_JACKET] > 99)
							m_config.m_portNum[TACTILE_JACKET] = 99;
					}
					else if( strcmp(strDeviceName, "TactileArmband") == 0 ) {
						m_config.m_typeDisplayDevice |= TAG_TACTILE_ARMBAND;
						element_device_type->Attribute( "PortNum", &m_config.m_portNum[TACTILE_ARMBAND] );
						if(m_config.m_portNum[TACTILE_ARMBAND] < 1)
							m_config.m_portNum[TACTILE_ARMBAND] = 1;
						if(m_config.m_portNum[TACTILE_ARMBAND] > 99)
							m_config.m_portNum[TACTILE_ARMBAND] = 99;
					}
				}
				element_device_type = element_device_type->NextSiblingElement();
			}
			if( m_config.m_typeDisplayDevice == 0)
				m_config.m_typeDisplayDevice |= TAG_TACTILE_JACKET;
		} else
			m_config.m_bUseDisplayDevice = false;
	} else m_config.m_bUseDisplayDevice = false;
	return 0;
}

void HugMe::ShowContactPoint(bool a_show)
{
	if(m_pContactPoint) {
		if(a_show) {
			m_pContactPoint->setShow(true, true);
		} else {
			m_pContactPoint->setShow(false, true);
		}
	}
}

void HugMe::InitRemoteWorld()
{
	m_worldRemote = new cWorld();

	// Create a mesh - we will build a cube manually, and later let the
	// user load 3d models
	m_remoteHumanModel.SetParentWorld(m_worldRemote);
	m_remoteHumanModel.ShowAvatar(m_config.m_bShowAvatar);

	// for debug
	m_pContactPoint = new cMesh(m_worldRemote);
	m_worldRemote->addChild(m_pContactPoint);
	m_pContactPoint->loadFromFile("Blender/ContactPoint.obj");
	m_pContactPoint->translate(CHAI_LARGE, CHAI_LARGE, CHAI_LARGE);
	m_pContactPoint->computeGlobalPositions();
	m_pContactPoint->setHapticEnabled(false, true);
	ShowContactPoint(m_config.m_bShowContactpoint);

	// set camera position and orientation
	// 
	// We choose to put it out on the positive z axis, so things appear
	// the way OpenGL users expect them to appear, with z going in and
	// out of the plane of the screen.
	double angle;
	angle = atan(240.0*m_pixelWidth/m_focalLength)*180.0/3.141592;
	if(!m_camera) {
		m_camera = new cCamera(m_worldRemote);
		int result = m_camera->set(cVector3d(0.0,0.0,0.0),   // position of camera
				  cVector3d(0.0, 0.0, -1.0),   // camera looking at origin
				  cVector3d(0.0, 1.0, 0.0));  // orientation of camera (standing up)
		m_camera->setFieldViewAngle(angle);
		m_camera->setClippingPlanes(1.0, (double)m_primaryDistance+(double)m_depthDistance);
	}

	if(!m_light) {
		// Turn on one light...
		m_light = new cLight(m_worldRemote);

		m_light->setEnabled(true);

		// Put the light somewhere that looks nice
		m_light->setDirectionalLight(true);
		m_light->setPos(cVector3d(1000.0, 1000.0, 1000));
		m_light->setDir(cVector3d(0.0, 0.0, -1.0));
		m_light->m_ambient.set(0.6f, 0.6f, 0.6f, 1.0f);
		m_light->m_diffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
		m_light->m_specular.set(0.5f, 0.5f, 0.5f, 1.0f);
		// Don't make it a spotlight
		m_light->setCutOffAngle(120.0);
	}

	// Create a display for graphic rendering
	m_viewport = new cViewport(m_hWnd, m_camera, false);
}

void HugMe::UninitRemoteWorld()
{
	if(m_worldRemote) {
		delete m_worldRemote;
		m_worldRemote = NULL;
	}

	if(m_camera) {
		delete m_camera;
		m_camera = NULL;
	}

	if(m_viewport) {
		delete m_viewport;
		m_viewport = NULL;
	}
}

// A single iteration through the loop...
void haptic_iteration(void* param) {
	HugMe * hugMe = (HugMe*)param;

	hugMe->m_pInputDevice->updatePose();
	hugMe->m_pInputDevice->computeForces();
	hugMe->m_pInputDevice->applyForces();
}

HugMe_Err HugMe::InitInputDevice(void) {
	// if the requested device is already connected, ignore this.
	if(m_device.m_bOnInputDevice && m_config.m_typeInputDevice == m_device.m_typeOnInputDevice)
		return HUGME_ERR_NO_ERR;
	
	if(m_device.m_bOnInputDevice)
		UninitInputDevice();

	switch(m_config.m_typeInputDevice) {
		case FORCE_FEEDBACK_FALCON:
		case FORCE_FEEDBACK_PHANTOM:
		case ONE_FINGER_GLOVE: {
			int bToolAvailable = -1;
			if (m_pInputDevice == 0) {
				if(m_config.m_typeInputDevice == FORCE_FEEDBACK_FALCON) {
					if(!(m_pInputDevice = new cFalcon3dofPointer(m_worldRemote, 0))) {
						return HUGME_ERR_NO_HAPTIC_DEVICE;
					}
				}
				else if(m_config.m_typeInputDevice == FORCE_FEEDBACK_PHANTOM) {
					if(!(m_pInputDevice = new cPhantom3dofPointer(m_worldRemote, 0))) {
						return HUGME_ERR_NO_HAPTIC_DEVICE;
					}
				} else if(m_config.m_typeInputDevice == ONE_FINGER_GLOVE) {
					if(!(m_pInputDevice = new cFinger3dofPointer(m_worldRemote, 0))) {
					  return HUGME_ERR_NO_HAPTIC_DEVICE;
					}
				}
			  
			  m_worldRemote->addChild(m_pInputDevice);

			  // set up the device
			  bToolAvailable = m_pInputDevice->initialize();

			  if(bToolAvailable != 0) {
					switch(m_config.m_typeInputDevice) {
					case FORCE_FEEDBACK_FALCON:
						AfxMessageBox("Falcon device is not connected!!!");
					break;
					case FORCE_FEEDBACK_PHANTOM:
						AfxMessageBox("Phantom device is not connected!!!");
					break;
					case ONE_FINGER_GLOVE:
						AfxMessageBox("Unable to initialize the hand tracking API!!!");
					break;
				  }
				  return HUGME_ERR_NO_HAPTIC_DEVICE;
			  }

  			  //m_pInputDevice->setPos(0.0, 0.0, -125.0);
			  switch(m_config.m_typeInputDevice) {
			  case FORCE_FEEDBACK_FALCON:
				  m_pInputDevice->setPos(0.0, 0.0, -80.0);
			  break;
			  case FORCE_FEEDBACK_PHANTOM:
			  break;
			  case ONE_FINGER_GLOVE:
				  m_pInputDevice->setPos(0.0, 0.0, -220.0);
			  break;
			  }
			  
			  // update initial orientation and position of device
			  m_pInputDevice->updatePose();

			  // set up a nice-looking workspace for the phantom so 
			  // it fits nicely with our shape
			  m_pInputDevice->setWorkspace(130.0,130.0,130.0);
			  m_pInputDevice->setRadius(1.0);
			}
			

			// I need to call this so the tool can update its internal
			// transformations before performing collision detection, etc.
			m_pInputDevice->computeGlobalPositions();

			// Open communication with the device
			m_pInputDevice->start();

			// Enable forces
			m_pInputDevice->setForcesON();

			// Tell the proxy algorithm associated with this tool to enable its
			// "dynamic mode", which allows interaction with moving objects
			m_pInputDevice->getProxy()->enableDynamicProxy(1);

			// Get the cProxyGraphPointForceAlgo instance
			m_proxyAlgo = m_pInputDevice->getProxy();

			m_worldRemote->computeGlobalPositions(true);
			m_proxyAlgo->m_useForceShading = false;
			m_proxyAlgo->setUseMelderFriction(true);

			// start the mm timer to run the haptic loop
			switch(m_config.m_typeInputDevice) {
				case FORCE_FEEDBACK_FALCON:
				case FORCE_FEEDBACK_PHANTOM:
					m_timer.set(1,haptic_iteration,this);
				break;
				case ONE_FINGER_GLOVE:
					//m_timer.set(33,haptic_iteration,this);
					m_pDisplayDeviceFinger = new DisplayDeviceFinger(m_config.m_portNumFinger);
					m_pDisplayDeviceFinger->initialize();
				break;
			}
		}
		break;
		case VIRTUAL_TOUCHPAD:
		break;
		case CELLPHONE_TOUCHPAD:
		break;
		default:
		break;
	}

	m_device.m_bOnInputDevice = true;
	m_device.m_typeOnInputDevice = m_config.m_typeInputDevice;

	return HUGME_ERR_NO_ERR;
//////////////////////////////////////////////////////////////////////////////////////////////
}

void HugMe::UninitInputDevice()
{
	switch(m_device.m_typeOnInputDevice) {
	case FORCE_FEEDBACK_FALCON:
	case FORCE_FEEDBACK_PHANTOM:
	case ONE_FINGER_GLOVE: {
		if(m_timer.m_numActiveTimers)
			m_timer.stop();
		m_pInputDevice->setForcesOFF();
		m_pInputDevice->stop();
		m_worldRemote->removeChild(m_pInputDevice);
		delete m_pInputDevice;
		m_pInputDevice = NULL;
		
	}
	case VIRTUAL_TOUCHPAD:
	break;
	case CELLPHONE_TOUCHPAD:
	break;
	default:
	break;
	}

	m_device.m_bOnInputDevice = false;
}

HugMe_Err HugMe::InitRemoteDepthVideo(void) {
	// Create depth image
	m_depthVideo = new cDepthImage(m_worldRemote);
	m_worldRemote->addChild(m_depthVideo);
	// Rotate the depth video to convert camera coordinate to opengl coordinate
	cVector3d axis(0.0, 0.0, 1.0);
	m_depthVideo->rotate(axis, 3.141592);

	m_depthVideo->computeGlobalPositions();
	m_depthVideo->computeBoundaryBox(true);

	m_depthVideo->m_lElapsedGraphicTime = 65000;
	
	m_depthVideo->m_imageMaterial.setStiffness(1.0);
	m_depthVideo->m_imageMaterial.setStaticFriction(0.7);
	m_depthVideo->m_imageMaterial.setDynamicFriction(0.6);
	//m_depthVideo->useImageBump(true);
	m_depthVideo->useSmoothing(true);
	m_depthVideo->m_smoothingArea = 3;
	m_depthVideo->setRenderSample(2);

	m_depthVideo->setCameraParameters(m_focalLength, m_pixelWidth, (double)m_primaryDistance, (double)m_depthDistance);

	/////////////////////////////////////////////////////////////////////
	// THIS THREE LINE SHOULD BE AFTER SETTING DEPTH VIDEO CLASS SETTING
	// DON'T FORGET
	/////////////////////////////////////////////////////////////////////
	double angle = atan(240.0*m_pixelWidth/m_focalLength)*180.0/3.141592;
	m_camera->setFieldViewAngle(angle);
	m_camera->setClippingPlanes(1.0, m_primaryDistance+m_depthDistance);
	/////////////////////////////////////////////////////////////////////
#ifdef USE_H263	
	m_depthVideo->setColorImageProperties(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, 3, 0);
	m_depthVideo->setDepthImageProperties(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, 1, 0);
#else
	m_depthVideo->setColorImageProperties(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, 4, 0);
	m_depthVideo->setDepthImageProperties(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, 1, 0);
#endif
	m_worldRemote->computeGlobalPositions(true);

	//if(m_pInputDevice)
	//	m_pInputDevice->getProxy()->initialize(m_worldRemote, cVector3d(0.0, 0.0, 0.0));

	return HUGME_ERR_NO_ERR;
}

void HugMe::UninitRemoteDepthVideo(void)
{
	if(m_depthVideo) {
		m_worldRemote->removeChild(m_depthVideo);
		delete m_depthVideo;
		m_depthVideo = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

HugMe_Err HugMe::InitImageDevice()
{
	// if the requested device is already connected, ignore this.
	if(m_device.m_bOnImageDevice && m_config.m_typeImageDevice == m_device.m_typeOnImageDevice)
		return HUGME_ERR_NO_ERR;
	
	if(m_device.m_bOnImageDevice)
		UninitInputDevice();

	switch(m_config.m_typeImageDevice) {
	case ZCAM:
		// Instance for controlling Z-Cam
		m_depthCamera = new CDepthCamera();
		// Registering the callback function
		// from within some other function
		if (!m_depthCamera->Initialize(1000))
		{
			return HUGME_ERR_NO_DEPTH_CAMERA;
		}
		m_depthCamera->ChangeResolution(RESOLUTION_320X240_30FPS);
		int rgbFullResWidth, rgbFullResHeight;
		m_depthCamera->GetVideoSize(m_depthWidth, m_depthHeight, rgbFullResWidth,rgbFullResHeight, dSize, rgbSize);
		m_depthCamera->SetDepthWindowPosition(m_primaryDistance, m_depthDistance);
		//m_depthCamera->EnableTrack(true);
		m_depthCamera->EnableSmoothing(true, 39.0);
		m_depthCamera->SetCleanVal(9.0);
		m_depthCamera->EnableMedian(true);
	break;
	}

	// image device is now on
	m_device.m_bOnImageDevice = true;
	m_device.m_typeOnImageDevice = ZCAM;

	return HUGME_ERR_NO_ERR;
}

void HugMe::UninitImageDevice()
{
	switch(m_device.m_typeOnImageDevice) {
	case ZCAM:
		delete m_depthCamera;
	break;
	}
	m_device.m_bOnImageDevice = false;
}

HugMe_Err HugMe::InitDisplayDevice(void)
{
	// For now, there is no way to check the display devices are connected or not.
	m_device.m_typeOnDisplayDevice = 0;

	if(m_config.m_typeDisplayDevice & TAG_TACTILE_JACKET) {
 		m_pDisplayDeviceJacket = new DisplayDeviceJacket(m_config.m_portNum[TACTILE_JACKET]);
		m_pDisplayDeviceJacket->initialize();
		m_device.m_typeOnDisplayDevice |= TAG_TACTILE_JACKET;
	}
	if(m_config.m_typeDisplayDevice & TAG_TACTILE_ARMBAND) {
 		m_pDisplayDeviceArmband = new DisplayDeviceArmband(m_config.m_portNum[TACTILE_ARMBAND]);
		m_pDisplayDeviceArmband->initialize();
		m_device.m_typeOnDisplayDevice |= TAG_TACTILE_ARMBAND;
	}

	m_device.m_bOnDisplayDevice = true;

	return HUGME_ERR_NO_ERR;
}

void HugMe::UninitDisplayDevice(void)
{
	if(m_device.m_typeOnDisplayDevice & TAG_TACTILE_JACKET) {
		if(m_pDisplayDeviceJacket) {
 			delete m_pDisplayDeviceJacket;
			m_pDisplayDeviceJacket = NULL;
		}
	}
	if(m_device.m_typeOnDisplayDevice & TAG_TACTILE_ARMBAND) {
		if(m_pDisplayDeviceArmband) {
 			delete m_pDisplayDeviceArmband;
			m_pDisplayDeviceArmband = NULL;
		}
	}

	m_device.m_typeOnDisplayDevice = 0;
	m_device.m_bOnDisplayDevice = false;
}

int HugMe::Initialize(void)
{
	HugMe_Err err = HUGME_ERR_NO_ERR;
	HugMe_Err result_err = HUGME_ERR_NO_ERR;

	if(m_config.m_bUseImageDevice) {
		err = InitImageDevice();
		if(err != HUGME_ERR_NO_ERR) {
			//AfxMessageBox("Failed to connect to a depth camera!!!");
			m_config.m_bUseImageDevice = false;
			result_err |= err;
		} else
			//m_localHumanModel.setTactileArray(m_pTactileDisplay);
			m_localHumanModel.m_ar.SetImageSize(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT);

	} else if(m_device.m_bOnImageDevice)
		UninitImageDevice();

	if(m_config.m_bUseInputDevice) {
		err = InitInputDevice();
		if(err != HUGME_ERR_NO_ERR) {
			//AfxMessageBox("Failed to connect to a input device");
			m_config.m_bUseInputDevice = false;
			result_err |= err;
		}
		m_localHumanModel.m_contactInfo.isContacted = false;
	} else if(m_device.m_bOnInputDevice)
		UninitInputDevice();
	
	if(m_config.m_bUseDisplayDevice) {
		err = InitDisplayDevice();
	}

	SaveConfig();
	
	
	return result_err;
}

void HugMe::Uninitialize(void)
{
	if(m_config.m_bUseImageDevice && m_device.m_bOnImageDevice)
		UninitImageDevice();
	if(m_config.m_bUseInputDevice && m_device.m_bOnInputDevice)
		UninitInputDevice();
	if(m_config.m_bUseDisplayDevice && m_device.m_bOnDisplayDevice)
		UninitDisplayDevice();
}

void HugMe::CheckDevices(void)
{
	if( m_config.m_bUseImageDevice )
		if( !CheckImageDevice() ) {
			m_config.m_bUseImageDevice = false;
		}

	if( m_config.m_bUseInputDevice )
		if( !CheckInputDevice() ) {
			m_config.m_bUseInputDevice = false;
		}

	SaveConfig();
}

bool HugMe::CheckImageDevice(void)
{
	bool nDeviceAvailable = false;
	// Instance for controlling Z-Cam
	CDepthCamera * tempCamera = new CDepthCamera();
	// Registering the callback function
	// from within some other function
	if (tempCamera->Initialize(1000))
		nDeviceAvailable = true;
	delete tempCamera;

	if(!nDeviceAvailable)
		AfxMessageBox("ZCam is not connected!!!");

	return nDeviceAvailable;
}

bool HugMe::CheckInputDevice(void)
{
	int nDeviceAvailable = -1;
	cWorld tempWorld;
	cGeneric3dofPointer * tempInputDevice;
	if( m_config.m_bUseInputDevice ) {
		switch( m_config.m_typeInputDevice ) {
		case FORCE_FEEDBACK_FALCON:
			tempInputDevice = new cFalcon3dofPointer(&tempWorld, 0);
			tempWorld.addChild(tempInputDevice);
			nDeviceAvailable = tempInputDevice->initialize();
			if(nDeviceAvailable == -1)
				AfxMessageBox("Falcon device is not connected!!!");
			tempInputDevice->setForcesOFF();
			tempInputDevice->stop();
			tempWorld.removeChild(tempInputDevice);
			delete tempInputDevice;
		break;
		case FORCE_FEEDBACK_PHANTOM:
			tempInputDevice = new cPhantom3dofPointer(&tempWorld, 0);
			tempWorld.addChild(tempInputDevice);
			nDeviceAvailable = tempInputDevice->initialize();
			if(nDeviceAvailable == -1)
				AfxMessageBox("Phantom device is not connected!!!");
		break;
		case ONE_FINGER_GLOVE:
			if(!CheckImageDevice()) {
				AfxMessageBox("Finger Glove cannot be used without a depth camera!!!");
				nDeviceAvailable = -1;
			} else
				nDeviceAvailable = 0;
			break;
		case CELLPHONE_TOUCHPAD:
			nDeviceAvailable = 0;
		break;
		case VIRTUAL_TOUCHPAD:
			nDeviceAvailable = 0;
			break;
		}
	}
	if( nDeviceAvailable == 0 )
		return true;
	else
		return false;
}

void HugMe::CaptureFrame(void)
{
	int distance, width;
	m_depthCamera->GetDepthWindowPosition(distance, width);

	//connect prgb to mpvideo4send same with depth data
	m_depthCamera->GetNextFrame(pDepth, pRGB, pRGBFullRes, pPrimary, pSecondary);
	memcpy(m_pLocalVideo, pRGB, sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*4);
	memcpy(m_pLocalDepth, pDepth, sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT);
	m_localHumanModel.UpdatePose(m_pLocalVideo);
}
