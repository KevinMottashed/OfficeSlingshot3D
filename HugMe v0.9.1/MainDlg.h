#if !defined(AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_)
#define AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

#include <string>

#include "Resource.h"
#include "UserInterfaceManager.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class UserInterfaceManager;

class CMainDlg : public CDialog
{
private:
	UserInterfaceManager* pUserInterfaceManager;
	string m_userName;
	string m_ipAddress;
// Construction
public:
	CMainDlg(UserInterfaceManager* pUserInterfaceManager, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	afx_msg void OnNetworkConnect();
	afx_msg void OnNetworkDisconnect();
	afx_msg void OnNetworkListen();
	afx_msg LRESULT OnNetworkEstablished(WPARAM);
	afx_msg LRESULT OnNetworkDisconnected(WPARAM);
	afx_msg LRESULT OnNetworkError();
	afx_msg void OnPreferencesEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_)
