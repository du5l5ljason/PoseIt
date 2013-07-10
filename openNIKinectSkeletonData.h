#pragma once
#include "KinectSkeletonData.h"
#include "ImgWnd.h"
#include  <XnCppWrapper.h>
#include "StructDefine.h"

using namespace xn;

#define MAXJOINT 25
#define MAXUSER 4
struct UserSkelPos{
	int nUserId;
	POINT3D jointPos[MAXJOINT];
	bool isTracking;
};

class openNIKinectSkeletonData :
	public KinectSkeletonData
{
private:
	Context*			m_pContext;
	DepthGenerator*		m_pDepthGen;
	UserGenerator*		m_pUserGen;
	UserSkelPos*		m_pSkelPos;
	int					nUsers;

public:
	UserSkelPos* getUserSkelPos(){return m_pSkelPos;};
	UserSkelPos getUserSkelPosAt(int nUserId){return m_pSkelPos[nUserId];};
	int getNumberofUsers(){return nUsers;};


public:
	openNIKinectSkeletonData(Context* context, DepthGenerator* depthGen, int jointCount);	

	~openNIKinectSkeletonData(void){
		if(m_pSkelPos!=NULL)
			delete[] m_pSkelPos;
	};

	POINT2D convertJoint2DAt(int idx) { POINT2D pt; return pt; };
	bool update();
};

