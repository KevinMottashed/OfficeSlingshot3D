#include "MFCOpenGLControl.h"

MFCOpenGLControl::MFCOpenGLControl(void)
{
	ve = boost::shared_ptr<VirtualEnvironment>(new VirtualEnvironment());
	displayW = 0;
	displayH = 0;
}

MFCOpenGLControl::~MFCOpenGLControl(void)
{
}

BEGIN_MESSAGE_MAP(MFCOpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void MFCOpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
   CString className = AfxRegisterWndClass(CS_HREDRAW |
      CS_VREDRAW | CS_OWNDC, NULL,
      (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

   CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE |
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

   // Set initial variables' values
   m_oldWindow    = rect;
   m_originalRect = rect;

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

	ve->initialize();
	
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

   // Send draw request
   OnDraw(NULL);
}

void MFCOpenGLControl::OnDraw(CDC *pDC)
{
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

void MFCOpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case 1:
		{
		// Clear color and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw Virtual Environment
		cVector3d ballPosition = ve->updateFrame();

		ve->getCamera()->renderView(displayW, displayH);

		// Swap buffers
		SwapBuffers(hdc);

		break;
	}

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void MFCOpenGLControl::spaceBarPressed(void)
{
	ve->shootBall();
}

void MFCOpenGLControl::shiftPressed(void)
{
	ve->receiveBall();
}