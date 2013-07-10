
// KinectMeDlg.cpp : implementation file
//
#include "stdafx.h"
#include "KinectMe.h"
#include "KinectMeDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include "time.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "ksPose.h"
#include <string>
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
bool g_RunKinect = true;
HANDLE g_hKinectThread;
int gnSystemStatus = _SS_REST;
struct timeval timeStamp;
double tCurrent = 0.0f;
double tChooseStart = 0.0f;
double tResembleStart = 0.0f;
double tShowStart = 0.0f;
double tRecordStart = 0.0f;
double tEndStart = 0.0f;
KinectOpenNI gKinect ;
float gDistScore = 0.0f;
string gSystemHelp = "";
extern int gnUsers;
vector<ksPose> poseGroup;

int colorType[8][3] = {
	{0,0,255},
	{0,255,0},
	{255,0,0},
	{255,255,0},
	{255,192,203},
	{150,113,23},
	{125,249,255},
	{255,165,0}
};

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

// Definition of a gettimeofday function
 
void getTimeofDay(struct timeval *tv, struct timezone *tz)
{
// Define a structure to receive the current Windows filetime
  FILETIME ft;
 
// Initialize the present time to 0 and the timezone to UTC
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
// The GetSystemTimeAsFileTime returns the number of 100 nanosecond 
// intervals since Jan 1, 1601 in a structure. Copy the high bits to 
// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
// Convert to microseconds by dividing by 10
    tmpres /= 10;
 
// The Unix epoch starts on Jan 1 1970.  Need to subtract the difference 
// in seconds from Jan 1 1601.
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
 
// Finally change microseconds to seconds and place in the seconds value. 
// The modulus picks up the microseconds.
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
  
// Adjust for the timezone west of Greenwich
      tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// KinectMeDlg dialog
KinectMeDlg::KinectMeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KinectMeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pCalib = new KinectCalibration;
	m_pImgProc = new ImageProc;
}

void KinectMeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(KinectMeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &KinectMeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KinectMeDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KinectMeDlg message handlers

BOOL KinectMeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitImgWnd();
	InitKinect();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void KinectMeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void KinectMeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR KinectMeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

///////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void KinectMeDlg::InitImgWnd()
{
	CRect rect;
	GetDlgItem( IDC_DEPTHFRAME )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_wndDepth.Create( WS_VISIBLE , rect, this, IDC_DEPTHFRAME );

	GetDlgItem( IDC_DEPTHFRAME2 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_wndDepth2.Create( WS_VISIBLE , rect, this, IDC_DEPTHFRAME2 );

	GetDlgItem( IDC_DEPTHFRAME3 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_wndDepth3.Create( WS_VISIBLE , rect, this, IDC_DEPTHFRAME3 );

	GetDlgItem( IDC_DEPTHFRAME4 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_wndDepth4.Create( WS_VISIBLE , rect, this, IDC_DEPTHFRAME4 );
	
	GetDlgItem( IDC_FULLFRAME )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_wndFull = new CDrawWnd();
	m_wndFull->Create( WS_VISIBLE , rect, this, IDC_FULLFRAME );
	

	//GetDlgItem( IDC_STATSBAR )->GetWindowRect( rect );
	//ScreenToClient( rect );
	//m_oglStatsView.oglCreate( rect, this );
	//m_oglStatsView.m_unpTimer = m_oglStatsView.SetTimer( 1, 1, 0 );

	GetDlgItem( IDC_STATSBAR2 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_oglStatusView.oglCreate( rect ,this );
	m_oglStatusView.m_unpTimer = m_oglStatusView.SetTimer( 1, 1, 0 );
	//m_oglFullWnd.oglCreate( rect, this );

	//m_oglFullWnd.m_unpTimer = m_oglFullWnd.SetTimer( 1, 1, 0 );
	
}

DWORD WINAPI ShowStreams(LPVOID lpParam){
	KinectMeDlg* dlg = (KinectMeDlg*)(lpParam);
	KinectOpenNI* kinect = dlg->getKinect();

	int w = kinect->getRGBImg()->width();
	int h = kinect->getRGBImg()->height();
	int wb = kinect->getRGBImg()->widthBytes();
	int nUsers = 4;
	CImgWnd* depthWnd = dlg->getDepthWnd();
	CImgWnd* depthWnd2 = dlg->getDepthWnd2();
	CImgWnd* depthWnd3 = dlg->getDepthWnd3();
	CImgWnd* depthWnd4 = dlg->getDepthWnd4();

	vector<CImgWnd*> wndGroup;
	vector<CImgWnd*>::iterator iterWnd;
	wndGroup.push_back( depthWnd );
	wndGroup.push_back( depthWnd3 );
	wndGroup.push_back( depthWnd2 );
	wndGroup.push_back( depthWnd4 );

	BYTE* pImg = NULL;
	BYTE* pBinImg = NULL;
	BYTE* pRGBImg = NULL;
	int nSampleRate = 2;
	//if there is pose in stock, just show the in good order. 
	if(poseGroup.size() > 1 )
	{
		for( int id = 0; id < poseGroup.size()-1; id ++ )
		{
			wndGroup.at(id)->ShowDownSampleImg24( w, h, wb, poseGroup.at(id+1).getBinaryImg()->getData(), nSampleRate );

		}
	}
	
	CImgWnd* fullWnd = dlg->getFullWnd();
	wb = kinect->getRGBImg()->widthBytes();
	// show full window
	switch( gnSystemStatus ){
			case _SS_REST:
				//show kinect RGB frame
				fullWnd->ShowImg( w, h, wb, kinect->getRGBImg()->getData());
			break;
			case _SS_CHOOSE:
				if( poseGroup.size() != 0 )
				{
					fullWnd->ShowImg( w, h, wb, poseGroup.at(0).getBinaryImg()->getData() );		// The full window always shows the first pose in the pose group
					//fullWnd->showHandJoint( dlg->getSkeleton()->getJointPosAt( XN_SKEL_LEFT_HAND ), dlg->getSkeleton()->getJointPosAt( XN_SKEL_RIGHT_HAND ), colorType[2] );
				}
				else
				{
					fullWnd->ShowImg( w, h, wb, kinect->getRGBImg()->getData());
					//fullWnd->showHandJoint( dlg->getSkeleton()->getJointPosAt( XN_SKEL_LEFT_HAND ), dlg->getSkeleton()->getJointPosAt( XN_SKEL_RIGHT_HAND ), colorType[2] );
				}
				break;
			case _SS_RESEMBLE:
				//
				if(poseGroup.size() == 0) break;
				for( int j = 0 ; j < h ; ++j )
				{
					pImg = (BYTE*)( dlg->getImg()->getData() + j* dlg->getImg()->widthBytes() );
					pBinImg = (BYTE*)( poseGroup.at(0).getBinaryImg()->getData() + j*poseGroup.at(0).getBinaryImg()->widthBytes() );
					pRGBImg = (BYTE*)( kinect->getRGBImg()->getData() + j* kinect->getRGBImg()->widthBytes() );
					for( int i = 0; i < w; ++i, pImg +=3, pBinImg +=3, pRGBImg +=3 )
					{
						if( (*pBinImg) == 0 )
						{
							*pImg = *pRGBImg;
							*(pImg+1) = *(pRGBImg+1);
							*(pImg+2) = *(pRGBImg+2);
						}
						else
						{
							*pImg = 0;
							*(pImg+1) =0;
							*(pImg+2) = 0;
						}
					}
				}

				fullWnd->ShowImg( w, h, wb, dlg->getImg()->getData() );	
				//for(int i=0;i<nUsers; ++i)

					//if(dlg->getSkeleton()->getUserSkelPosAt(i).isTracking)
						//fullWnd->showSkeletonWithConfidence( dlg->getSkeleton()->getUserSkelPosAt(i),colorType[0], colorType[1]  );

				break;
			case _SS_SHOW:
				if( dlg->getPoseUtils()->evalDist( gDistScore ) != _BAD )
					fullWnd->ShowImg( w, h, wb, poseGroup.at(0).getRGBImg()->getData() );
				//if score = bad
				// show the latest movement
				fullWnd->showHandJoint( dlg->getSkeleton()->getJointPosAt( XN_SKEL_LEFT_HAND ), dlg->getSkeleton()->getJointPosAt( XN_SKEL_RIGHT_HAND ), colorType[2] );

				break;
			case _SS_RECORD:
				fullWnd->ShowImg( w, h, wb, kinect->getRGBImg()->getData() );
				for(int i=0;i<nUsers; ++i)
				{
					if(dlg->getSkeleton()->getUserSkelPosAt(i).isTracking)
						fullWnd->showSkeleton( dlg->getSkeleton()->getUserSkelPosAt(i).jointPos,colorType[i]);
				}
				break;
			case _SS_END:
				getTimeofDay( &timeStamp, NULL );
				tCurrent = timeStamp.tv_sec + (timeStamp.tv_usec/100000.0);
				if( (tCurrent - tEndStart) > 5 )
					fullWnd->ShowImg( w, h, wb, kinect->getRGBImg()->getData() );
	}

	fullWnd->Invalidate();

	//show TestWnd

	return 0;
	//	if(p!=NULL) delete [] p ;
		
}


//check for hand operations
bool isPoseMatching(openNIKinectSkeletonData* skeleton, BaseBuf* img)
{
	int nOffsetX =0;
	int nOffsetY =0;
	BYTE* p = new BYTE[3];
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	int sum = 0;
	int hit = 0;
	for(int i=0;i<4;++i)
	{
		if(skeleton->getUserSkelPosAt(i).isTracking)
		{
			for(int j=1;j<MAXJOINT;++j)
			{
				nOffsetX = skeleton->getUserSkelPosAt(i).jointPos[j].x;
				nOffsetY = skeleton->getUserSkelPosAt(i).jointPos[j].y;
				if(nOffsetX<0)
					nOffsetX =0;
				if(nOffsetX>640)
					nOffsetX = 640;
				if(nOffsetY<0)
					nOffsetY =0;
				if(nOffsetY>480)
					nOffsetY = 480;
				img->getPixelAt(nOffsetY, nOffsetX, p);
				if(p[0]!=255&&p[1]!=255&&p[2]!=255)
					hit +=1;
				sum +=1;
			}
		}
	}

	if((float)hit/sum > 0.85 )
		return true;
	else 
		return false;
}

DWORD WINAPI KinectThread(LPVOID lpParam){
	KinectMeDlg* dlg = (KinectMeDlg*)(lpParam);
	KinectOpenNI* kinect = dlg->getKinect();
	openNIKinectSkeletonData* skeleton = dlg->getSkeleton();
	//


	Rect ROI;
	ROI.x = 0;
	ROI.y = 0;
	ROI.width = 640;
	ROI.height = 480;
	int frameID = 0;
	float handX = 0.0f;
	float handLastX = 0.0f;
	bool isAdd = false;
	while(g_RunKinect){

		kinect->update();

		skeleton->update();

		if( !dlg->getBG()->isBG() )
			dlg->getBG()->init( kinect->getRGBImg(), kinect->getDepthImg() );
		//controller
		switch(gnSystemStatus){
			case _SS_REST:
					gSystemHelp = "Welcome to Pose it World";
					dlg->getOglStatusView()->setSystemStatus( gSystemHelp );
					if(skeleton->update())
						dlg->OnChoose();
				break;
			case _SS_CHOOSE:
				getTimeofDay(&timeStamp, NULL);
				tCurrent = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
				if( (tCurrent - tChooseStart) > 10 )		//after the player is detected, he/she is required to wait for generating a pose.
				{
					if( poseGroup.size() == 0 )
						dlg->OnRecord();
					else
					{
						dlg->OnResemble();
					}
				}
				break;
			case _SS_RESEMBLE:
				getTimeofDay(&timeStamp, NULL );
				tCurrent = timeStamp.tv_sec + (timeStamp.tv_usec/100000.0);
				dlg->getOglStatusView()->setTimer( 30 -( tCurrent - tResembleStart ) );
				if( ( tCurrent - tResembleStart ) < 30 )
				{
					//Calculate the score
					ksPose newPose; 
					for( int j = 1; j< MAXJOINT ; j++ )
					{	
						newPose.setJointPosAt( dlg->getSkeleton()->getJoint3DPosAt(j).x, 
												dlg->getSkeleton()->getJoint3DPosAt(j).y,
												dlg->getSkeleton()->getJoint3DPosAt(j).z,
												1, j );
					}
					gDistScore = dlg->getPoseUtils()->getDistBetween( newPose, *dlg->getPose() );
					if( dlg->getPoseUtils()->evalDist( gDistScore ) == _EXCELLENT )
						dlg->OnShow();
				}
				else	// the score is good or bad
				{
					dlg->OnShow();
				}

				break;
			case _SS_SHOW:
				getTimeofDay( &timeStamp, NULL );
				tCurrent = timeStamp.tv_sec + (timeStamp.tv_usec/100000.0);
			    handX = dlg->getSkeleton()->getJointPosAt( XN_SKEL_RIGHT_HAND ).x;

				if( (tCurrent - tShowStart ) > 10 )
				{
					if(  fabs(handX-handLastX) > 20 )
					{
						if( dlg->getPoseUtils()->evalDist( gDistScore ) != _EXCELLENT )
						{
							cout << "Thanks for playing, have a good day!" << endl;
							dlg->OnEnd();
							isAdd = false;
						}
						if( dlg->getPoseUtils()->evalDist( gDistScore ) == _EXCELLENT )
						{
							dlg->OnRecord();
						}
					}
				}
				handLastX =  handX;
				break;
			case _SS_RECORD:

				getTimeofDay( &timeStamp, NULL );
				tCurrent = timeStamp.tv_sec + (timeStamp.tv_usec/100000.0);
				dlg->getOglStatusView()->setTimer( 15 - ( tCurrent - tRecordStart ) );
				if( (tCurrent - tRecordStart) > 15)
				{
					ksPose newPose = ksPose( kinect->getRGBImg()->width(), kinect->getRGBImg()->height() );
					dlg->getBG()->removeBG( 
						kinect->getRGBImg(), 
						kinect->getDepthImg(), 
						dlg->getBG()->getRgbBG(), 
						dlg->getBG()->getDepthBG(),
						dlg->getRemovalImg(),
						ROI);
					newPose.setBinaryImg( dlg->getRemovalImg());
					newPose.setRGBImg( kinect->getRGBImg() );
					for( int j = 1; j< MAXJOINT ; j++ )
					{	
						newPose.setJointPosAt( dlg->getSkeleton()->getJoint3DPosAt(j).x, 
												dlg->getSkeleton()->getJoint3DPosAt(j).y,
												dlg->getSkeleton()->getJoint3DPosAt(j).z,
												1, j );
					}
					newPose.setRecord( 0, 0 );
				

					if( poseGroup.size() < 5 )
					{ 
						poseGroup.push_back(newPose);

					}
					else if( poseGroup.size() == 5 )
					{
						poseGroup.erase(poseGroup.begin(), poseGroup.begin()+1);
						poseGroup.push_back( newPose );
					}

					dlg->OnEnd();
				}
				break;
			case _SS_END:
					if( !dlg->getSkeleton()->update() && gnUsers == 0)
						gnSystemStatus = _SS_REST;

					if( isAdd == false)
					{
						if( dlg->getPoseUtils()->evalDist( gDistScore ) == _GOOD )
							poseGroup.at(0).AddaRecord( true );
						else if( dlg->getPoseUtils()->evalDist( gDistScore ) == _BAD )
							poseGroup.at(0).AddaRecord( false );
						isAdd = true;
					}
					break;
			default: break;
		
		}

		HANDLE hShowStreams = CreateThread(NULL, 0, ShowStreams, dlg, 0, NULL);
		WaitForSingleObject(hShowStreams, INFINITE);
		frameID = frameID + 1;
		if(poseGroup.size() > 0 )
		{
			dlg->getOglStatusView()->setRecordBad( poseGroup.at(0).getRecord().nBad );
			dlg->getOglStatusView()->setRecordGood( poseGroup.at(0).getRecord().nGood );
		}

	}
	return 0;
}

void KinectMeDlg::InitKinect()
{
	kinect.init();
	int nWidth, nHeight;

	if(kinect.getRGBImg()!=NULL)
	{
		nWidth = kinect.getRGBImg()->width();
		nHeight = kinect.getRGBImg()->height(); 
	}
	m_pBG = new Background(nWidth,nHeight);	//Is it good to put it here?
	m_pSkeleton = new openNIKinectSkeletonData(kinect.getContext(), kinect.getDepthGenerator(), 24);
	m_fullImg = new Buffer24(nWidth, nHeight, nWidth*3);
	m_removalImg = new Buffer24(nWidth, nHeight, nWidth*3);
	m_modelPose = ksPose( nWidth, nHeight );
	kinect.open();

	g_hKinectThread = CreateThread(NULL, 0, KinectThread, this, 0, NULL);
}

void KinectMeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void KinectMeDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	g_RunKinect = false;
	WaitForSingleObject(g_hKinectThread, 200);
	CDialogEx::OnCancel();
}

void KinectMeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
   switch (nType)
   {
      case SIZE_RESTORED:
      {
         if (m_oglStatsView.m_bIsMaximized && m_oglStatusView.m_bIsMaximized)
         {
           m_oglStatsView.OnSize(nType, cx, cy);
           m_oglStatsView.m_bIsMaximized = false;
           m_oglStatusView.OnSize(nType, cx, cy);
           m_oglStatusView.m_bIsMaximized = false;
         }
 
         break;
      }
 
      case SIZE_MAXIMIZED:
      {
         m_oglStatsView.OnSize(nType, cx, cy);
         m_oglStatsView.m_bIsMaximized = true;
         m_oglStatusView.OnSize(nType, cx, cy);
         m_oglStatusView.m_bIsMaximized = true;
 
         break;
      }
   }
}

void KinectMeDlg::OnChoose(){
	getTimeofDay(&timeStamp, NULL);
	tChooseStart = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);	
	gnSystemStatus = _SS_CHOOSE;
	
	gSystemHelp = "Randomly Choose a pose";
	m_oglStatusView.setSystemStatus( gSystemHelp );
	if(poseGroup.size() == 0 ) return;
	int nPoseId = rand() % poseGroup.size();
	//switch poseGroup.at(0) and poseGroup.at(nPoseId);
	vector<ksPose> tempGroup = poseGroup;
	poseGroup.at(0) = tempGroup.at(nPoseId);
	poseGroup.at(nPoseId) = tempGroup.at(0);
	tempGroup.empty();
}

void KinectMeDlg::OnResemble(){
	getTimeofDay(&timeStamp, NULL);
	tResembleStart = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
	gnSystemStatus = _SS_RESEMBLE;
	
	gSystemHelp = "Pose it Time, resemble the pose to see who is it";
	m_oglStatusView.setSystemStatus( gSystemHelp );
	//store the modelPose
	//1. get poseVector size
	int nPoseId = 0;
	m_modelPose.setBinaryImg( poseGroup.at(nPoseId).getBinaryImg() );
	m_modelPose.setRGBImg( poseGroup.at(nPoseId).getRGBImg() );
	for( int idx = 1; idx < MAXJOINT; idx++ )
	{
		m_modelPose.setJointPosAt( poseGroup.at(nPoseId).getJointPosAt(idx), idx);
	}
	m_modelPose.setRecord( poseGroup.at(nPoseId).getRecord().nGood, poseGroup.at(nPoseId).getRecord().nBad );
	cout << " The system status is " << gnSystemStatus << endl;	
}

void KinectMeDlg::OnShow(){
	getTimeofDay(&timeStamp, NULL);
	tShowStart = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
	gnSystemStatus = _SS_SHOW;
	gSystemHelp = "Please wave your hand to continue";
	m_oglStatusView.setSystemStatus( gSystemHelp );
	cout << " The system status is " << gnSystemStatus << endl;
}

void KinectMeDlg::OnRecord(){
	getTimeofDay(&timeStamp, NULL);
	tRecordStart = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
	gnSystemStatus = _SS_RECORD;
	cout << " The system status is " << gnSystemStatus << endl;
	gSystemHelp = "Please leave your amazing pose";
	m_oglStatusView.setSystemStatus( gSystemHelp );
}

void KinectMeDlg::OnEnd(){
	getTimeofDay(&timeStamp, NULL);
	tRecordStart = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
	gnSystemStatus = _SS_END;
	gSystemHelp = "Thank you for coming, good bye!";
	m_oglStatusView.setSystemStatus( gSystemHelp );
	cout << " The system status is " << gnSystemStatus << endl;
}