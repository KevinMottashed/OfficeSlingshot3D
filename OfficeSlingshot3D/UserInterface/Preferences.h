#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "stdafx.h"
#include "UserPreferences.h"

/**
 * A classe used to manage the user's preferences
 * by displaying a dialog with all the required values.
 * This dialog is contained within the main window of
 * the application.
 */

class CPreferences : public CDialog
{
public:
	/**
	 * Constructor.
	 * @param prefs The user's preferences.
	 * @param pParent The parent window NULL in our case.
	 */
	CPreferences(const UserPreferences& prefs, CWnd* pParent = NULL);   // standard constructor

	/**
	* Getter for private attributes.
	* @return the user preferences.
	*/
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
 
