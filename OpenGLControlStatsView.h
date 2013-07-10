#pragma once
#include "openglcontrol.h"
#include <string>
#include <sstream>

using namespace std;
class COpenGLControlStatsView :
	public COpenGLControl
{
private:
	//show info
	string m_recordGood;
	string m_recordBad;
	//bad and good score
	//计时器的数字
	string m_timer;
	//Excellent, good, bad
	//
public:
	COpenGLControlStatsView(void){
		m_recordGood = "No. of GOOD ";
		m_recordBad = "No. of BAD ";
		m_timer = "";
	};
	virtual ~COpenGLControlStatsView(void);
public:
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
	void setTimer( float number ){
		stringstream ss (stringstream::in | stringstream::out);
		ss << number;
		string temp = ss.str();
		m_timer = temp;
	}
	void oglDrawScene();
	void oglDrawText( const char* text,int length, int x, int y  );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDraw();
	DECLARE_MESSAGE_MAP()
};

