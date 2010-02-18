#if !defined(AFX_STATICCANVAS_H__F886C54F_6C70_48F0_A8E8_ADCA9A6256F6__INCLUDED_)
#define AFX_STATICCANVAS_H__F886C54F_6C70_48F0_A8E8_ADCA9A6256F6__INCLUDED_

#include "Pattern.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticCanvas.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticCanvas window

class CStaticCanvas : public CStatic
{
// Construction
public:
	CStaticCanvas();

// Attributes
public:
	Pattern* m_pDrawingPattern;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticCanvas)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticCanvas();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticCanvas)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCANVAS_H__F886C54F_6C70_48F0_A8E8_ADCA9A6256F6__INCLUDED_)
