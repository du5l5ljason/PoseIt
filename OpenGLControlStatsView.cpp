#include "StdAfx.h"
#include "OpenGLControlStatsView.h"
#include <string>

using namespace std;




COpenGLControlStatsView::~COpenGLControlStatsView(void)
{
}
BEGIN_MESSAGE_MAP(COpenGLControlStatsView, COpenGLControl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


int COpenGLControlStatsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here		oglInitialize();
	oglInitialize();
	return 0;
}


void COpenGLControlStatsView::OnSize(UINT nType, int cx, int cy)
{
	COpenGLControl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}


void COpenGLControlStatsView::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call COpenGLControl::OnPaint() for painting messages
	ValidateRect( NULL );
}


void COpenGLControlStatsView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	COpenGLControl::OnTimer(nIDEvent);
}

void COpenGLControlStatsView::OnDraw()
{
	glLoadIdentity();							// Every time OnDraw is called, do the rotation and translation
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControlStatsView::oglDrawScene(){
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

 //   glBegin(GL_QUADS);
 //     // Top Side
 //     glVertex3f( 1.0f, 1.0f,  1.0f);
 //     glVertex3f( 1.0f, 1.0f, -1.0f);
 //     glVertex3f(-1.0f, 1.0f, -1.0f);
 //     glVertex3f(-1.0f, 1.0f,  1.0f);
 //
 //     // Bottom Side
 //     glVertex3f(-1.0f, -1.0f, -1.0f);
 //     glVertex3f( 1.0f, -1.0f, -1.0f);
 //     glVertex3f( 1.0f, -1.0f,  1.0f);
 //     glVertex3f(-1.0f, -1.0f,  1.0f);
 //
 //     // Front Side
 //     glVertex3f( 1.0f,  1.0f, 1.0f);
 //     glVertex3f(-1.0f,  1.0f, 1.0f);
 //     glVertex3f(-1.0f, -1.0f, 1.0f);
 //     glVertex3f( 1.0f, -1.0f, 1.0f);
 //
 //     // Back Side
 //     glVertex3f(-1.0f, -1.0f, -1.0f);
 //     glVertex3f(-1.0f,  1.0f, -1.0f);
 //     glVertex3f( 1.0f,  1.0f, -1.0f);
 //     glVertex3f( 1.0f, -1.0f, -1.0f);
 //
 //     // Left Side
 //     glVertex3f(-1.0f, -1.0f, -1.0f);
 //     glVertex3f(-1.0f, -1.0f,  1.0f);
 //     glVertex3f(-1.0f,  1.0f,  1.0f);
 //     glVertex3f(-1.0f,  1.0f, -1.0f);
 //
 //     // Right Side
 //     glVertex3f( 1.0f,  1.0f,  1.0f);
 //     glVertex3f( 1.0f, -1.0f,  1.0f);
 //     glVertex3f( 1.0f, -1.0f, -1.0f);
 //     glVertex3f( 1.0f,  1.0f, -1.0f);
 //  
	//glEnd();
	string text = m_recordGood;
	oglDrawText( text.data(), text.size(),50.0f, 150.0f );
	text = m_recordBad;
	oglDrawText( text.data(), text.size(),50.0f, 50.0f );

}

void COpenGLControlStatsView::oglDrawText( const char* text, int length, int x, int y )
{
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 200, 0.0, 200);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2i( x, y );
	for( int i=0; i< length; i++)
	{
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, (int)text[i] );
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}