#pragma once
#include "OpenGLControl.h"
#include <string>
#include <sstream>

using namespace std;
class COpenGLControlStatusView :
	public COpenGLControl
{
private: 
	string	m_systemStatus;
	string  m_timer;
	string m_recordGood;
	string m_recordBad;
public:
	COpenGLControlStatusView(void){
		m_systemStatus = "Welcome to PoseIt!";
		m_timer = "";
		m_recordGood = "No. of GOOD ";
		m_recordBad = "No. of BAD ";
	};

	virtual ~COpenGLControlStatusView(void);

public:
	void setSystemStatus( string text ){
		m_systemStatus = text;
	}

	void setTimer( float number ){
		stringstream ss (stringstream::in | stringstream::out);
		ss << number;
		string temp = ss.str();
		m_timer = temp;
	}
	void setRecordGood( int number ){
	stringstream ss (stringstream::in | stringstream::out);
	ss << number;
	string temp = ss.str();
	m_recordGood = "No. of GOOD " + temp;}
	void setRecordBad( int number ){
	stringstream ss (stringstream::in | stringstream::out);
	ss << number;
	string temp = ss.str();
	m_recordBad = "No. of BAD " + temp;}
	void oglDrawScene();
	void oglDrawText( const char* text, int length, int x, int y );


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDraw();
	DECLARE_MESSAGE_MAP()
};

