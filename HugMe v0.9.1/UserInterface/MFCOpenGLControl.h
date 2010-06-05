#pragma once
#include "afxwin.h"

#include <gl/gl.h>
#include <gl/glu.h>

class MFCOpenGLControl : public CWnd
{
private:
	CWnd    *hWnd;
	HDC     hdc;
	HGLRC   hrc;
	int     m_nPixelFormat;
	CRect   m_rect;
	CRect   m_oldWindow;
	CRect   m_originalRect;

public:
	UINT_PTR m_unpTimer;

	MFCOpenGLControl(void);

	virtual ~MFCOpenGLControl(void);

	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};