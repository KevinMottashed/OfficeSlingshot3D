#include "MFCOpenGLControl.h"
#include "UserInterfaceSignals.h"
#include "KeyboardProxy.h"

MFCOpenGLControl::MFCOpenGLControl(void) :
	openGlInitialized(false)
{
	displayW = 0;
	displayH = 0;	
}

MFCOpenGLControl::~MFCOpenGLControl(void)
{
}

void MFCOpenGLControl::camera(cCamera* camera)
{
	_camera = camera;
}

BEGIN_MESSAGE_MAP(MFCOpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ON_OPENGLDRAW, OnOpenGlDraw)
END_MESSAGE_MAP()

void MFCOpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
   CString className = AfxRegisterWndClass(CS_HREDRAW |
      CS_VREDRAW | CS_OWNDC, NULL,
      (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

   CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE |
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

   hWnd = parent;
}

void MFCOpenGLControl::OnPaint()
{
	ValidateRect(NULL);

	CDialog::OnPaint();
}

int MFCOpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
      return -1;
	
	oglInitialize();
	
	return CDialog::OnCreate(lpCreateStruct);
}

void MFCOpenGLControl::oglInitialize(void)
{
   static PIXELFORMATDESCRIPTOR pfd =
   {
	   sizeof(PIXELFORMATDESCRIPTOR),
	   1,
	   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	   PFD_TYPE_RGBA,
	   32,    // bit depth
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   16,    // z-buffer depth
	   0, 0, 0, 0, 0, 0, 0,
   };

   // Get device context only once.
   hdc = GetDC()->m_hDC;

   // Pixel format.
   m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
   SetPixelFormat(hdc, m_nPixelFormat, &pfd);

   // Create the OpenGL Rendering Context.
   hrc = wglCreateContext(hdc);
   wglMakeCurrent(hdc, hrc);

   // Set color to use when clearing the background.
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClearDepth(1.0f);

   // Turn on backface culling
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);

   // Turn on depth testing
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   openGlInitialized = true;

   // Send draw request
   OnDraw(NULL);
}

void MFCOpenGLControl::paint()
{
	// make sure the open gl component has been initialized before trying to paint it
	if (openGlInitialized && _camera != NULL)
	{
		// make sure this window's handle hasn't been released.
		// for some reason the handle can get released before the destructor is called.
		// If this check fails it means the app is shutting down
		if (m_hWnd != NULL)
		{
			SendMessage(WM_ON_OPENGLDRAW);
		}
	}
}

void MFCOpenGLControl::OnDraw(CDC *pDC)
{
}

LRESULT MFCOpenGLControl::OnOpenGlDraw(WPARAM, LPARAM)
{
	// open gl must be initialized
	assert(openGlInitialized);

	// we must have a camera to render
	assert(_camera != NULL);

	// Clear color and depth buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->renderView(displayW, displayH);

	// Swap buffers
	SwapBuffers(hdc);

	return 0;
}

void MFCOpenGLControl::OnSize(UINT nType, int cx, int cy)
{

	// update the size of the viewport
    displayW = cx;
    displayH = cy;

   if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

   // Map the OpenGL coordinates.
   glViewport(0, 0, cx, cy);

   // Projection view
   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();

   // Set our current view perspective
   gluPerspective(35.0f, (float)cx / (float)cy, 0.0f, 7.0f);

   // Model view
   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();

   CDialog::OnSize(nType, cx, cy);
}

// Method used to intercept messages before they are processed by the UI
BOOL MFCOpenGLControl::PreTranslateMessage(MSG* pMsg)
{
	// catches the message when the user presses the Escape key
	if (pMsg->message == WM_KEYDOWN)
	{
		// let the keyboard manager know that a key was pressed
		Keyboard::instance()->keyPressed(pMsg->wParam);
	}
	// perform the default action
	return CDialog::PreTranslateMessage(pMsg);
}