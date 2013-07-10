#include "StdAfx.h"
#include "OpenGLControlStatusView.h"



COpenGLControlStatusView::~COpenGLControlStatusView(void)
{
}

BEGIN_MESSAGE_MAP(COpenGLControlStatusView, COpenGLControl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


int COpenGLControlStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here		oglInitialize();
	oglInitialize();
	return 0;
}


void COpenGLControlStatusView::OnSize(UINT nType, int cx, int cy)
{
	COpenGLControl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}


void COpenGLControlStatusView::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call COpenGLControl::OnPaint() for painting messages
	ValidateRect( NULL );
}


void COpenGLControlStatusView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	COpenGLControl::OnTimer(nIDEvent);
}

void COpenGLControlStatusView::OnDraw()
{
	glLoadIdentity();							// Every time OnDraw is called, do the rotation and translation
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControlStatusView::oglDrawScene(){

	string text = m_recordGood;
	oglDrawText( text.data(), text.size(),30.0f, 150.0f );
	text = m_recordBad;
	oglDrawText( text.data(), text.size(),30.0f, 50.0f );
	text = m_timer;
	oglDrawText( text.data(), text.size(), 300.0f, 30.0f);
	text = m_systemStatus;
	oglDrawText( text.data(), text.size(), 300.0f, 150.0f);
}

void COpenGLControlStatusView::oglDrawText( const char* text, int length, int x, int y )
{
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 600, 0.0, 200);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2i( x, y );
	for( int i=0; i< length; i++)
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i] );
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}