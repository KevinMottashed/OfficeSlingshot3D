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


CPreferences::CPreferences(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferences)
		string line;
		ifstream myfile ("userPreferences.txt");
		if (myfile.is_open()) {
			getline (myfile, line);
			m_userName = line.c_str();
			getline (myfile, line);
			m_strAddress = line.c_str();
			myfile.close();
		} else {
			m_userName = "UserName";
			m_strAddress = "127.0.0.1";
		}

	//}}AFX_DATA_INIT
}


void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
		DDX_Text(pDX, IDC_NAME_EDIT, m_userName);
		DDX_Text(pDX, IDC_IPADDRESS_EDIT, m_strAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferences, CDialog)
	//{{AFX_MSG_MAP(CPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers