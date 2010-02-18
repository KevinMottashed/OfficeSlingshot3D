#if !defined(AFX_CELLPHONEDLG_H__89D6E013_AD36_4C12_8155_579423ECCDD5__INCLUDED_)
#define AFX_CELLPHONEDLG_H__89D6E013_AD36_4C12_8155_579423ECCDD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CellphoneDlg.h : header file
//
#include "CellphoneSocket.h"
#include "HugMe.h"

/////////////////////////////////////////////////////////////////////////////
// CCellphoneDlg dialog

class CCellphoneDlg : public CDialog
{
// Construction
public:
	bool m_bIsListening;
	bool m_bIsConnected;

	bool m_bTouched;
	HumanPart m_selectedBodyPart;
	float m_normTouchPointX;
	float m_normTouchPointY;

	CCellphoneDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCellphoneDlg)
	enum { IDD = IDD_CELLPHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellphoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CellphoneSocket* m_pCellphoneSocket;
	HugMeDevice * m_device;

	// Generated message map functions
	//{{AFX_MSG(CCellphoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAccept(WPARAM, LPARAM);
	afx_msg LRESULT OnCloseSocket(WPARAM, LPARAM);
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg void OnClose();
	afx_msg void OnYes();
	afx_msg void OnNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_pChatDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLPHONEDLG_H__89D6E013_AD36_4C12_8155_579423ECCDD5__INCLUDED_)
