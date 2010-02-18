#if !defined(AFX_VIRTUALPADDLG_H__C7E29F36_8FA5_4562_BC06_956634DBDD13__INCLUDED_)
#define AFX_VIRTUALPADDLG_H__C7E29F36_8FA5_4562_BC06_956634DBDD13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualPadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualPadDlg dialog
#include "HumanModel.h"

class CVirtualPadDlg : public CDialog
{
// Construction
public:
	bool m_bTouched;
	float m_normTouchPointX;
	float m_normTouchPointY;
	int m_padWidth;
	int m_padHeight;
	int m_padTop;
	int m_padLeft;
	HumanPart m_selectedBodyPart;
	CWnd* m_pParent;
	CVirtualPadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVirtualPadDlg)
	enum { IDD = IDD_VIRTUAL_PAD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualPadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVirtualPadDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBodyPart();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALPADDLG_H__C7E29F36_8FA5_4562_BC06_956634DBDD13__INCLUDED_)
