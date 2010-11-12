#ifndef MFCOPENGLCONTROL_H
#define MFCOPENGLCONTROL_H

#include "afxwin.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "boost.h"
#include "chai3d.h"

/**
 * A class representing the OpenGL window.
 * The OpenGL window is the UI element in charge of
 * the rendering of the game environment.
 */

class MFCOpenGLControl : public CDialog
{
public:

   /**
	* Constructor.
	*/
	MFCOpenGLControl(void);

	/**
	* Destructor.
	*/
	virtual ~MFCOpenGLControl(void);

	/**
	 * Set the camera that the ogl frame will use to display frames
	 * @param camera The new camera
	 */
	void camera(cCamera* camera);
	
	/**
	 * Create the OpenGL properties to the UI control.
	 * @param rect The rectangle where the OpenGL environment is going to be rendered.
	 * @param parent The parent window.
	 */
	void oglCreate(CRect rect, CWnd *parent);

	/**
	 * Initialize OpenGL properties.
	 */
	void oglInitialize(void);

	/**
	 * Sends an OnPaint request to the opengl scene
	 */
	void paint();

// all this stuff is protected
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDraw(CDC *pDC);
	afx_msg LRESULT OnOpenGlDraw(WPARAM, LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:

	/**
	 * A reference to the window.
	 */
	CWnd    *hWnd;

	/**
	 * A reference to the DC component.
	 */
	HDC     hdc;

	/**
	 * A reference to the GLRC component.
	 */
	HGLRC   hrc;

	/**
	 * The pixel format to be used for rendering.
	 */
	int     m_nPixelFormat;

	/**
	 * The OpenGL window's width.
	 */
	int displayW;

	/**
	 * The OpenGL window's height.
	 */
	int displayH;

	/**
	 * The virtual environment's camera.
	 */
	cCamera* _camera;

	bool openGlInitialized; /**< True if the open gl component has been initialized */

	/**
	 * Inherited method to catch messages before they are sent to the UI.
	 * @param pMsg the message being intercepted
	 */
	virtual BOOL MFCOpenGLControl::PreTranslateMessage(MSG* pMsg);
};
#endif