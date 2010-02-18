#if !defined(AFX_PATTERNDLG_H__756FF672_5B87_4E38_B031_846AD924618A__INCLUDED_)
#define AFX_PATTERNDLG_H__756FF672_5B87_4E38_B031_846AD924618A__INCLUDED_

#include "Pattern.h"
#include "StaticCanvas.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatternDlg dialog

class CPatternDlg : public CDialog
{
// Construction
public:
	CPatternDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatternDlg)
	enum { IDD = IDD_PATTERN_DIALOG };
	CStatic	m_picIcon;
	CStaticCanvas	m_picPattern;
	CScrollBar	m_scrollIntensity;
	CScrollBar	m_scrollFrame;
	CString	m_strIntensity;
	CString	m_strFrame;
	CString	m_strName;
	//}}AFX_DATA

private:
	Pattern::Point* m_curPoint;
public:
	Pattern m_pattern;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatternDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatternDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonDeleteAll();
	afx_msg void OnButtonClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonLoadico();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNDLG_H__756FF672_5B87_4E38_B031_846AD924618A__INCLUDED_)
