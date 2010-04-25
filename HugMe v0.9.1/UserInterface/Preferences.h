#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "stdafx.h"
#include "UserPreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

class CPreferences : public CDialog
{
// Construction
public:
	CPreferences(const UserPreferences& prefs, CWnd* pParent = NULL);   // standard constructor

	// getters for private attributes
	UserPreferences getPreferences();


private:
// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES };
	UserPreferences m_preferences;
	CString m_strAddress;
	CString m_userName;
	int m_armBandPort;
	int m_jacketPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
 
