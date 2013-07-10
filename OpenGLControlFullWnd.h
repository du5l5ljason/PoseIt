#pragma once
#include "openglcontrol.h"
#include "KinectSensor.h"

using namespace xn;
class COpenGLControlFullWnd :
	public COpenGLControl
{
private:
	KinectOpenNI* hKinect;
public:
	COpenGLControlFullWnd(void);
	virtual ~COpenGLControlFullWnd(void);

	KinectOpenNI* getHKinect(){return hKinect;};
	void setHKinect( KinectOpenNI* kinect){
		if(kinect!=NULL)
			hKinect = kinect;
			}
	
	void oglDrawScene();


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDraw();
	DECLARE_MESSAGE_MAP()
};

