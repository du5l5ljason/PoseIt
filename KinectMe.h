
// KinectMe.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// KinectMeApp:
// See KinectMe.cpp for the implementation of this class
//

class KinectMeApp : public CWinApp
{
public:
	KinectMeApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern KinectMeApp theApp;