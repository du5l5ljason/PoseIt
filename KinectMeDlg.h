
// KinectMeDlg.h : header file
//

#pragma once
#include "ImgWnd.h"
#include "DrawWnd.h"
#include "KinectSensor.h"
#include "KinectCalibration.h"
#include "openNIKinectSkeletonData.h"
#include "OpenGLControlStatsView.h"
#include "OpenGLControlStatusView.h"
#include "StructDefine.h"
#include "Background.h"
#include "Buffer.h"
#include "ImageProc.h"
#include "ksPose.h"
#include "ksPoseUtils.h"
enum COLORTABLE{
	BLUE,
	RED,
	GREEN,
	YELLOW,
	PINK,
	BROWN,
	CYAN,
	ORANGE,
};

enum SYMSTATUS{
	_SS_REST,
	_SS_CHOOSE,
	_SS_RESEMBLE,
	_SS_SHOW,
	_SS_RECORD,
	_SS_END,
};

// KinectMeDlg dialog
class KinectMeDlg : public CDialogEx
{
private:
	//member
	CImgWnd m_wndDepth;		//
	CImgWnd m_wndDepth2;
	CImgWnd m_wndDepth3;
	CImgWnd m_wndDepth4;
	CImgWnd* m_wndFull;
	CImgWnd m_wndTest;
	//COpenGLControlFullWnd m_oglFullWnd;
	COpenGLControlStatsView m_oglStatsView;
	COpenGLControlStatusView m_oglStatusView;
	KinectOpenNI kinect;	//kinect device class
	KinectCalibration* m_pCalib;
	openNIKinectSkeletonData* m_pSkeleton;
	Background* m_pBG;
	BaseBuf* m_fullImg;
	BaseBuf* m_removalImg;
	ImageProc* m_pImgProc;
	ksPose	m_modelPose;
	ksPoseUtils m_poseUtils;
//Timer

public:
	//getter and setter
	CImgWnd* getDepthWnd(){return &m_wndDepth;};
	CImgWnd* getDepthWnd2(){return &m_wndDepth2;};
	CImgWnd* getDepthWnd3(){return &m_wndDepth3;};
	CImgWnd* getDepthWnd4(){return &m_wndDepth4;};
	CImgWnd* getFullWnd(){return m_wndFull;};
	CImgWnd* getTestWnd(){return &m_wndTest;};
	COpenGLControlStatsView* getOglStatsView(){return &m_oglStatsView;};
	COpenGLControlStatusView* getOglStatusView(){return &m_oglStatusView;};
	KinectOpenNI* getKinect(){return &kinect;};
	openNIKinectSkeletonData* getSkeleton(){return m_pSkeleton;}
	KinectCalibration* getCalibration(){return m_pCalib;};
	Background* getBG(){return m_pBG;};
	BaseBuf*	getImg(){return m_fullImg;};
	BaseBuf*	getRemovalImg(){return m_removalImg;};
	ImageProc* getImgProc(){return m_pImgProc;};
	ksPose* getPose(){return &m_modelPose;};
	ksPoseUtils* getPoseUtils(){return &m_poseUtils;};
private:
	void InitImgWnd();
	void InitKinect();
public:
	void OnChoose();
	void OnResemble();
	void OnShow();
	void OnRecord();
	void OnEnd();
public:
//Construction and Deconstruction
	KinectMeDlg(CWnd* pParent = NULL);
	~KinectMeDlg(void){
		//if(m_wndFull!=NULL)delete m_wndFull;
		if(m_pCalib!=NULL)delete m_pCalib;
		if(m_pSkeleton!=NULL)delete m_pSkeleton;
		if(m_pBG!=NULL)delete m_pBG;
		if(m_fullImg!=NULL)delete m_fullImg;
		if(m_pImgProc!=NULL)delete m_pImgProc;
		if(m_removalImg!=NULL)delete m_removalImg;
	}

		// Dialog Data
	enum { IDD = IDD_KINECTME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// global methods
	
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
