#if !defined(AFX_LISTENDLG_H__8A6C0B87_7643_43FB_B02A_3A3014B6F2C4__INCLUDED_)
#define AFX_LISTENDLG_H__8A6C0B87_7643_43FB_B02A_3A3014B6F2C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CListenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListenDlg dialog

class CListenDlg : public CDialog
{
// Construction
public:
	CListenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListenDlg)
	enum { IDD = IDD_LISTEN_DIALOG };
	CString	m_localName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListenDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTDLG_H__8A6C0B87_7643_43FB_B02A_3A3014B6F2C4__INCLUDED_)
