#include "StdAfx.h"
#include "OpenGLControl.h"
#include <string>

using namespace std;

COpenGLControl::COpenGLControl(void)
{
   m_fPosX = 0.0f;    // X position of model in camera view
   m_fPosY = 0.0f;    // Y position of model in camera view
   m_fZoom = 10.0f;   // Zoom on model in camera view
   m_fRotX = 0.0f;    // Rotation on model in camera view
   m_fRotY = 0.0f;    // Rotation on model in camera view
   m_fLastX = 0.0f;
   m_fLastY = 0.0f;
}


COpenGLControl::~COpenGLControl(void)
{
}

void COpenGLControl::oglCreate( CRect rect, CWnd *parent )
{
	CString className = AfxRegisterWndClass( CS_HREDRAW | 
		CS_VREDRAW | CS_OWNDC, NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx( 0, className, (LPCTSTR)("OpenGL"), WS_CHILD | WS_VISIBLE | 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0 );

	// Set initial variables' values
	m_oldWindow = rect;
	m_originalRect = rect;

	hWnd = parent;
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	ValidateRect( NULL );
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,							// bit depth
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		16,							// z-buffer depth
		0,0,0,0,0,0,0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	//Pixel format
	m_nPixelFormat = ChoosePixelFormat( hdc, &pfd );
	SetPixelFormat( hdc, m_nPixelFormat, &pfd );

	// Create the OpenGL Rendering Context
	hrc = wglCreateContext( hdc );
	wglMakeCurrent( hdc, hrc );

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );

	// Turn on backface culling
	glFrontFace( GL_CCW );
	glCullFace( GL_BACK );

	// Turn on depth testing
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	// Send draw request
	OnDraw( NULL );

}

void COpenGLControl::OnDraw( CDC *pDC )
{
	// TODO: Camera controls.
	glLoadIdentity();							// Every time OnDraw is called, do the rotation and translation
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
	{
	case 1:
		{
			// Clear color and depth buffer bits
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
			// Draw OpenGL scene
			oglDrawScene();
			// Swap buffers
			SwapBuffers( hdc );

			break;
		}

	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( 0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED ) return;

	// Map the OpenGL coordinates
	glViewport( 0, 0, cx, cy );

	// Projection View
	glMatrixMode( GL_PROJECTION );

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective( 35.0f, (float)cx/ (float)cy, 0.01f, 2000.0f );

	// Model view
	glMatrixMode( GL_MODELVIEW );

	switch (nType)
	{
	   // If window resize token is "maximize"
	   case SIZE_MAXIMIZED:
	   {
		  // Get the current window rect
		  GetWindowRect(m_rect);
 
		  // Move the window accordingly
		  MoveWindow(6, 6, cx - 14, cy - 14);
 
		  // Get the new window rect
		  GetWindowRect(m_rect);
 
		  // Store our old window as the new rect
		  m_oldWindow = m_rect;
 
		  break;
	   }
 
	   // If window resize token is "restore"
	   case SIZE_RESTORED:
	   {
		  // If the window is currently maximized
		  if (m_bIsMaximized)
		  {
			 // Get the current window rect
			 GetWindowRect(m_rect);
 
			 // Move the window accordingly (to our stored old window)
			 MoveWindow(m_oldWindow.left,
						m_oldWindow.top - 18,
						m_originalRect.Width() - 4,
						m_originalRect.Height() - 4);
 
			 // Get the new window rect
			 GetWindowRect(m_rect);
 
			 // Store our old window as the new rect
			 m_oldWindow = m_rect;
		  }
 
		  break;
	   }
	}
}

//void COpenGLControl::oglDrawScene(void)
//{
//	// Wireframe Mode
//	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//
// //   glBegin(GL_QUADS);
// //     // Top Side
// //     glVertex3f( 1.0f, 1.0f,  1.0f);
// //     glVertex3f( 1.0f, 1.0f, -1.0f);
// //     glVertex3f(-1.0f, 1.0f, -1.0f);
// //     glVertex3f(-1.0f, 1.0f,  1.0f);
// //
// //     // Bottom Side
// //     glVertex3f(-1.0f, -1.0f, -1.0f);
// //     glVertex3f( 1.0f, -1.0f, -1.0f);
// //     glVertex3f( 1.0f, -1.0f,  1.0f);
// //     glVertex3f(-1.0f, -1.0f,  1.0f);
// //
// //     // Front Side
// //     glVertex3f( 1.0f,  1.0f, 1.0f);
// //     glVertex3f(-1.0f,  1.0f, 1.0f);
// //     glVertex3f(-1.0f, -1.0f, 1.0f);
// //     glVertex3f( 1.0f, -1.0f, 1.0f);
// //
// //     // Back Side
// //     glVertex3f(-1.0f, -1.0f, -1.0f);
// //     glVertex3f(-1.0f,  1.0f, -1.0f);
// //     glVertex3f( 1.0f,  1.0f, -1.0f);
// //     glVertex3f( 1.0f, -1.0f, -1.0f);
// //
// //     // Left Side
// //     glVertex3f(-1.0f, -1.0f, -1.0f);
// //     glVertex3f(-1.0f, -1.0f,  1.0f);
// //     glVertex3f(-1.0f,  1.0f,  1.0f);
// //     glVertex3f(-1.0f,  1.0f, -1.0f);
// //
// //     // Right Side
// //     glVertex3f( 1.0f,  1.0f,  1.0f);
// //     glVertex3f( 1.0f, -1.0f,  1.0f);
// //     glVertex3f( 1.0f, -1.0f, -1.0f);
// //     glVertex3f( 1.0f,  1.0f, -1.0f);
// //  
//	//glEnd();
//
//	//string text;
//	//text = " This is me.";
//
//	//oglDrawText( text.data(), text.size(), -1.0f, 1.0f );
//	////glutSwapBuffers();
//}

//void COpenGLControl::oglDrawText( const char* text, int length, int x, int y )
//{
//	//glMatrixMode( GL_PROJECTION );
//	//double *matrix = new double[16];
//	//glGetDoublev( GL_PROJECTION_MATRIX, matrix );
//	//glLoadIdentity();
//	//glOrtho( 0, 800, 0, 600, -5, 5 );
//	//glMatrixMode( GL_MODELVIEW );
//	//glLoadIdentity();
//	//glPushMatrix();
//	//glLoadIdentity();
//	glRasterPos2i( x, y );
//	for( int i=0; i< length; i++)
//	{
//		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, (int)text[i] );
//	}
//	//glPopMatrix();
//	//glMatrixMode( GL_PROJECTION );
//	//glLoadMatrixd( matrix );
//	//glMatrixMode( GL_MODELVIEW );
//
//}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;
 
	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;
 
		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}
 
		m_fRotY += (float)0.5f * diffX;
 
		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}
 
	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
	}
 
	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}
 
	OnDraw(NULL);
 

	CWnd::OnMouseMove(nFlags, point);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	oglInitialize();

	return 0;
}
