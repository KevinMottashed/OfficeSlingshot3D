#if !defined(AFX_OPTIONDLG_H__8877E69C_69BC_4557_948E_C551C3989946__INCLUDED_)
#define AFX_OPTIONDLG_H__8877E69C_69BC_4557_948E_C551C3989946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//
#include "ChatDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

class COptionDlg : public CDialog
{
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION };
	BOOL	m_bShowRemotePerson;
	BOOL	m_bShowAvatar;
	BOOL	m_bShowContactPoint;
	BOOL	m_bUseInputDevice;
	BOOL	m_bUseDisplayDevice;
	BOOL	m_bUseImageDevice;
	BOOL	m_bUseTactileJacket;
	BOOL	m_bUseTactileArmband;
	int		m_portNumJacket;
	int		m_portNumArmband;
	BOOL	m_bUseFingerTactile;
	int		m_portNumFinger;
	//}}AFX_DATA
	HugMeConfig m_config;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CChatDlg * m_pChatDlg;

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUseImageDevice();
	afx_msg void OnUseInputDevice();
	afx_msg void OnUseDisplayDevice();
	afx_msg void OnTactileJacket();
	afx_msg void OnTactileArmband();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeInputDevice();
	afx_msg void OnFingerTactile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__8877E69C_69BC_4557_948E_C551C3989946__INCLUDED_)
