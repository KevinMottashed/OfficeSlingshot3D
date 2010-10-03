#ifndef MFCOPENGLCONTROL_H
#define MFCOPENGLCONTROL_H

#include "afxwin.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "boost.h"
#include "chai3d.h"

class MFCOpenGLControl : public CDialog
{
private:
	CWnd    *hWnd;
	HDC     hdc;
	HGLRC   hrc;
	int     m_nPixelFormat;
	CRect   m_rect;
	CRect   m_oldWindow;
	CRect   m_originalRect;

	int displayW;
	int displayH;

	cCamera* _camera;

	UINT_PTR m_unpTimer;

public:

	/**
	 * Set the camera that the ogl frame will use to display frames
	 * @param camera The new camera
	 */
	void camera(cCamera* camera);

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
#endif