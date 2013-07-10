#pragma once
#include "afxwin.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>
#include <SDL/SDL.h>

class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl(void);
	virtual ~COpenGLControl(void);

public:
	UINT_PTR m_unpTimer;
	bool	m_bIsMaximized;
protected:
	//Window information
	CWnd	*hWnd;
	HDC		hdc;
	HGLRC	hrc;
	int		m_nPixelFormat;
	CRect	m_rect;
	CRect	m_oldWindow;
	CRect	m_originalRect;
protected:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fZoom;
	float	m_fRotX;
	float	m_fRotY;
	float	m_fLastX;
	float	m_fLastY;
public:
	void	oglCreate( CRect rect, CWnd *parent );
	void	oglInitialize(void);

	//specific draw functions
	virtual void oglDrawScene() = 0;
public:
	afx_msg void OnPaint();
	afx_msg void OnDraw( CDC *pDC ); 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


