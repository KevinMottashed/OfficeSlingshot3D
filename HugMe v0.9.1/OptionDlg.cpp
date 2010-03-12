// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(UserInterfaceManager* pUserInterfaceManager, CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent), m_pUserInterfaceManager(pUserInterfaceManager)
{
	m_pChatDlg = (CChatDlg *) pParent;

	m_config = m_pChatDlg->getLocalConfig();

	//{{AFX_DATA_INIT(COptionDlg)
	m_bShowRemotePerson = m_config.m_bShowDepthImage;
	m_bShowAvatar = m_config.m_bShowAvatar;
	m_bShowContactPoint = m_config.m_bShowContactpoint;
	m_bUseInputDevice = m_config.m_bUseInputDevice;
	m_bUseDisplayDevice = m_config.m_bUseDisplayDevice;
	m_bUseImageDevice = m_config.m_bUseImageDevice;
	m_bUseTactileJacket = m_config.m_typeDisplayDevice & TAG_TACTILE_JACKET;
	m_bUseTactileArmband = m_config.m_typeDisplayDevice & TAG_TACTILE_ARMBAND;
	m_portNumJacket = m_config.m_portNum[TACTILE_JACKET];
	m_portNumArmband = m_config.m_portNum[TACTILE_ARMBAND];
	m_bUseFingerTactile = m_config.m_bUseFingerTactile;
	m_portNumFinger = m_config.m_portNumFinger;
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Check(pDX, IDC_SHOW_REMOTE_PERSON, m_bShowRemotePerson);
	DDX_Check(pDX, IDC_SHOW_AVATAR, m_bShowAvatar);
	DDX_Check(pDX, IDC_SHOW_CONTACT, m_bShowContactPoint);
	DDX_Check(pDX, IDC_USE_INPUT_DEVICE, m_bUseInputDevice);
	DDX_Check(pDX, IDC_USE_DISPLAY_DEVICE, m_bUseDisplayDevice);
	DDX_Check(pDX, IDC_USE_IMAGE_DEVICE, m_bUseImageDevice);
	DDX_Check(pDX, IDC_TACTILE_JACKET, m_bUseTactileJacket);
	DDX_Check(pDX, IDC_TACTILE_ARMBAND, m_bUseTactileArmband);
	DDX_Text(pDX, IDC_PORT_JACKET, m_portNumJacket);
	DDX_Text(pDX, IDC_PORT_ARMBAND, m_portNumArmband);
	DDX_Check(pDX, IDC_FINGER_TACTILE, m_bUseFingerTactile);
	DDX_Text(pDX, IDC_PORT_FINGER, m_portNumFinger);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_BN_CLICKED(IDC_USE_IMAGE_DEVICE, OnUseImageDevice)
	ON_BN_CLICKED(IDC_USE_INPUT_DEVICE, OnUseInputDevice)
	ON_BN_CLICKED(IDC_USE_DISPLAY_DEVICE, OnUseDisplayDevice)
	ON_BN_CLICKED(IDC_TACTILE_JACKET, OnTactileJacket)
	ON_BN_CLICKED(IDC_TACTILE_ARMBAND, OnTactileArmband)
	ON_BN_CLICKED(IDC_TEST_JACKET, OnTestJacket)
	ON_BN_CLICKED(IDC_TEST_FALCON, OnTestFalcon)
	ON_BN_CLICKED(IDC_TEST_CAMERA, OnTestCamera)

	ON_CBN_SELCHANGE(IDC_INPUT_DEVICE, OnSelchangeInputDevice)
	ON_BN_CLICKED(IDC_FINGER_TACTILE, OnFingerTactile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;

	CComboBox * pImageDevice = (CComboBox *)GetDlgItem(IDC_IMAGE_DEVICE);
	for(i=0; i<NUM_IMAGE_DEVICE; i++)
		pImageDevice->InsertString(i, m_pChatDlg->getHugMeSystem()->getImageDeviceName((tagImageDeviceType)i));
	pImageDevice->SetCurSel(m_config.m_typeImageDevice);
	if(!m_config.m_bUseImageDevice)
		pImageDevice->EnableWindow(FALSE);

	CComboBox * pInputDevice = (CComboBox *)GetDlgItem(IDC_INPUT_DEVICE);
	for(i=0; i<NUM_INPUT_DEVICE; i++)
		pInputDevice->InsertString(i, m_pChatDlg->getHugMeSystem()->getInputDeviceName((tagInputDeviceType)i));
	pInputDevice->SetCurSel(m_config.m_typeInputDevice);
	if(!m_config.m_bUseInputDevice)
		pInputDevice->EnableWindow(FALSE);
	CButton * pFingerTactile = (CButton *)GetDlgItem(IDC_FINGER_TACTILE);
	CEdit * pPortNumFinger = (CEdit *)GetDlgItem(IDC_PORT_FINGER);
	if(m_config.m_typeInputDevice != ONE_FINGER_GLOVE) {
		pFingerTactile->EnableWindow(FALSE);
		pPortNumFinger->EnableWindow(FALSE);
	} else {
		if(!m_config.m_bUseFingerTactile)
			pPortNumFinger->EnableWindow(FALSE);
	}

	CButton * pTactileJacket = (CButton *)GetDlgItem(IDC_TACTILE_JACKET);
	CButton * pTactileArmband = (CButton *)GetDlgItem(IDC_TACTILE_ARMBAND);
	CEdit * pPortNumJacket = (CEdit *)GetDlgItem(IDC_PORT_JACKET);
	CEdit * pPortNumArmband = (CEdit *)GetDlgItem(IDC_PORT_ARMBAND);
	if(!m_config.m_bUseDisplayDevice) {
		pTactileJacket->EnableWindow(FALSE);
		pTactileArmband->EnableWindow(FALSE);
		pPortNumJacket->EnableWindow(FALSE);
		pPortNumArmband->EnableWindow(FALSE);
	} else {
		if( !(m_config.m_typeDisplayDevice & TAG_TACTILE_JACKET) )
			pPortNumJacket->EnableWindow(FALSE);
		if( !(m_config.m_typeDisplayDevice & TAG_TACTILE_ARMBAND) )
			pPortNumArmband->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	

void COptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);


	// != 0 converts to bool without a compiler warning
	m_config.m_bShowAvatar = m_bShowAvatar != 0;
	m_config.m_bShowContactpoint = m_bShowContactPoint != 0;
	m_config.m_bShowDepthImage = m_bShowRemotePerson != 0;
	m_config.m_bUseDisplayDevice = m_bUseDisplayDevice != 0;
	m_config.m_bUseImageDevice = m_bUseImageDevice != 0;
	m_config.m_bUseInputDevice = m_bUseInputDevice != 0;

	CComboBox * pImageDevice = (CComboBox *)GetDlgItem(IDC_IMAGE_DEVICE);
	m_config.m_typeImageDevice = (ImageDeviceType)pImageDevice->GetCurSel();

	CComboBox * pInputDevice = (CComboBox *)GetDlgItem(IDC_INPUT_DEVICE);
	m_config.m_typeInputDevice = (InputDeviceType)pInputDevice->GetCurSel();

	if(m_portNumFinger < 1)
		m_portNumFinger = 1;
	if(m_portNumFinger > 99)
		m_portNumFinger = 99;
	if(m_config.m_typeInputDevice == ONE_FINGER_GLOVE) {
		m_config.m_bUseFingerTactile = m_bUseFingerTactile != 0;
		m_config.m_portNumFinger = m_portNumFinger;
	}

	if(m_portNumJacket < 1)
		m_portNumJacket = 1;
	if(m_portNumJacket > 99)
		m_portNumJacket = 99;
	if(m_portNumArmband < 1)
		m_portNumArmband = 1;
	if(m_portNumArmband > 99)
		m_portNumArmband = 99;
	m_config.m_typeDisplayDevice = 0;
	if(m_bUseTactileJacket) {
		m_config.m_typeDisplayDevice |= TAG_TACTILE_JACKET;
		m_config.m_portNum[TACTILE_JACKET] = m_portNumJacket;
	}
	if(m_bUseTactileArmband) {
		m_config.m_typeDisplayDevice |= TAG_TACTILE_ARMBAND;
		m_config.m_portNum[TACTILE_ARMBAND] = m_portNumArmband;
	}

	CDialog::OnOK();
}

void COptionDlg::OnUseImageDevice() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CComboBox * pImageDevice = (CComboBox *)GetDlgItem(IDC_IMAGE_DEVICE);
	if( m_bUseImageDevice ) {
		pImageDevice->EnableWindow(TRUE);
		pImageDevice->SelectString(0, m_pChatDlg->getHugMeSystem()->getImageDeviceName((tagImageDeviceType)m_config.m_typeImageDevice));
	}
	else
		pImageDevice->EnableWindow(FALSE);
}

void COptionDlg::OnUseInputDevice() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CComboBox * pInputDevice = (CComboBox *)GetDlgItem(IDC_INPUT_DEVICE);
	CEdit * pPortNumFinger = (CEdit *)GetDlgItem(IDC_PORT_FINGER);
	if( m_bUseInputDevice ) {
		pInputDevice->EnableWindow(TRUE);
		pInputDevice->SelectString(0, m_pChatDlg->getHugMeSystem()->getInputDeviceName((tagInputDeviceType)m_config.m_typeInputDevice));
		if(m_config.m_typeInputDevice == ONE_FINGER_GLOVE)
			pPortNumFinger->EnableWindow(TRUE);
		else
			pPortNumFinger->EnableWindow(FALSE);
	}
	else {
		pInputDevice->EnableWindow(FALSE);
		pPortNumFinger->EnableWindow(FALSE);
	}

	
}

void COptionDlg::OnUseDisplayDevice() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CButton * pTactileJacket = (CButton *)GetDlgItem(IDC_TACTILE_JACKET);
	CButton * pTactileArmband = (CButton *)GetDlgItem(IDC_TACTILE_ARMBAND);
	CEdit * pPortNumJacket = (CEdit *)GetDlgItem(IDC_PORT_JACKET);
	CEdit * pPortNumArmband = (CEdit *)GetDlgItem(IDC_PORT_ARMBAND);
	if( m_bUseDisplayDevice ) {
		pTactileJacket->EnableWindow(TRUE);
		pTactileArmband->EnableWindow(TRUE);
		pPortNumJacket->EnableWindow(TRUE);
		pPortNumArmband->EnableWindow(TRUE);
	}
	else {
		pTactileJacket->EnableWindow(FALSE);
		pTactileArmband->EnableWindow(FALSE);
		pPortNumJacket->EnableWindow(FALSE);
		pPortNumArmband->EnableWindow(FALSE);
	}
}

void COptionDlg::OnTactileJacket() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CEdit * pPortNumJacket = (CEdit *)GetDlgItem(IDC_PORT_JACKET);
	if( m_bUseTactileJacket )
		pPortNumJacket->EnableWindow(TRUE);
	else
		pPortNumJacket->EnableWindow(FALSE);
	
}

void COptionDlg::OnTactileArmband() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CEdit * pPortNumArmband = (CEdit *)GetDlgItem(IDC_PORT_ARMBAND);
	if( m_bUseTactileArmband )
		pPortNumArmband->EnableWindow(TRUE);
	else
		pPortNumArmband->EnableWindow(FALSE);
	
}

void COptionDlg::OnTestJacket() 
{
	/*
	// TODO: Add your control notification handler code here
	if(m_pChatDlg->getHugMeSystem()->getDevice().m_bOnDisplayDevice) {
		m_pChatDlg->getHugMeSystem()->getDisplayDeviceJacket()->test1by1();
	}
	*/
	m_pUserInterfaceManager->testJacketButtonPressed();
}

void COptionDlg::OnTestFalcon() 
{
	m_pUserInterfaceManager->testFalconButtonPressed();
}

void COptionDlg::OnTestCamera() 
{
	m_pUserInterfaceManager->testCameraButtonPressed();
}

void COptionDlg::OnSelchangeInputDevice() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pInputDevice = (CComboBox *)GetDlgItem(IDC_INPUT_DEVICE);
	InputDeviceType typeInputDevice = (InputDeviceType)pInputDevice->GetCurSel();

	CEdit * pPortFinger = (CEdit *)GetDlgItem(IDC_PORT_FINGER);

	if(typeInputDevice == ONE_FINGER_GLOVE) {
		pPortFinger->EnableWindow(TRUE);
	} else
		pPortFinger->EnableWindow(FALSE);
}


void COptionDlg::OnFingerTactile() 
{
	// TODO: Add your control notification handler code here
	
}
