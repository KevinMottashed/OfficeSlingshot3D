// Preferences.cpp : implementation file
//

#include <iostream>
#include <fstream>

#include "stdafx.h"
#include "chat.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog


CPreferences::CPreferences(const UserPreferences& prefs, CWnd* pParent /*=NULL*/)
	:	m_preferences(prefs),
		CDialog(CPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferences)
	m_strAddress = m_preferences.ipAddress.c_str();
	m_userName = m_preferences.name.c_str();
	m_armBandPort = m_preferences.armBandPort;
	m_jacketPort = m_preferences.jacketPort;
	//}}AFX_DATA_INIT
}


void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
		DDX_Text(pDX, IDC_NAME_EDIT, m_userName);
		DDX_Text(pDX, IDC_IPADDRESS_EDIT, m_strAddress);
		DDX_Text(pDX, IDC_ARMBAND_EDIT, m_armBandPort);
		DDX_Text(pDX, IDC_JACKET_EDIT, m_jacketPort);
	//}}AFX_DATA_MAP
}

UserPreferences CPreferences::getPreferences()
{
	m_preferences.ipAddress = m_strAddress;
	m_preferences.name = m_userName;
	m_preferences.armBandPort = m_armBandPort;
	m_preferences.jacketPort = m_jacketPort;
	return m_preferences;
}

BEGIN_MESSAGE_MAP(CPreferences, CDialog)
	//{{AFX_MSG_MAP(CPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers