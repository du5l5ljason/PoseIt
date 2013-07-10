#include "StdAfx.h"
#include "OpenGLControlFullWnd.h"


COpenGLControlFullWnd::COpenGLControlFullWnd(void)
{
	hKinect = new KinectOpenNI();
}

COpenGLControlFullWnd::~COpenGLControlFullWnd(void)
{
	if(hKinect!=NULL) delete hKinect;
}

BEGIN_MESSAGE_MAP(COpenGLControlFullWnd, COpenGLControl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COpenGLControlFullWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	oglInitialize();
	return 0;
}

void COpenGLControlFullWnd::oglDrawScene(){
	
	XnUInt16 nXRes = hKinect->getRGBImg()->width();
	XnUInt16 nYRes = hKinect->getRGBImg()->height();

	glPushMatrix();
	glPushAttrib( GL_ENABLE_BIT );
	glPointSize( 1 );

	glBegin( GL_POINTS );

	BYTE *pImg = (BYTE*)hKinect->getRGBImg()->getData();

	for( XnUInt16 nY = 0; nY < nYRes; ++nY )
	{
		pImg = (BYTE*)(hKinect->getRGBImg()->getData() + nY*hKinect->getRGBImg()->widthBytes());
		for( XnUInt16 nX = 0; nX < nXRes; ++nX, pImg +=3 )
		{
			float r = (float)(*(pImg))/255.0;
			float g = (float)(*(pImg+1))/255.0;
			float b = (float)(*(pImg+2))/255.0;
			glColor3f( (GLfloat)r, (GLfloat)g, (GLfloat)b );
			glVertex2d( nX, nY );
		}
	}

	glEnd();
	glPopAttrib();
	glPopMatrix();


}
void COpenGLControlFullWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	COpenGLControl::OnTimer(nIDEvent);
}

void COpenGLControlFullWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call COpenGLControl::OnPaint() for painting messages
	ValidateRect( NULL );
}

void COpenGLControlFullWnd::OnSize(UINT nType, int cx, int cy)
{
	COpenGLControl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}

void COpenGLControlFullWnd::OnDraw(){
	glLoadIdentity();							// Every time OnDraw is called, do the rotation and translation
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}