#include "StdAfx.h"
#include "openNIKinectSkeletonData.h"
#include "cv.h"
#include "highgui.h"
UserGenerator g_UserGenerator;
XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";
int gnUsers = 0;
#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
		return;														\
	}
#define CALIBRATION_FILE_NAME "E:\\UserCalibration.bin"

// Save calibration to file
void SaveCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);

	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who is already calibrate
		// Save user's calibration to file
		g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], CALIBRATION_FILE_NAME);
		break;
	}
}
// Load calibration from file
void LoadCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who isn't calibrated or currently in pose
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
		if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;

		// Load user's calibration from file
		XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], CALIBRATION_FILE_NAME);
		if (rc == XN_STATUS_OK)
		{
			// Make sure state is coherent
			g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
			g_UserGenerator.GetSkeletonCap().StartTracking(aUserIDs[i]);
		}
		break;
	}
}
// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	gnUsers = gnUsers + 1;
	printf("New User %d\n", nId);
	if( gnUsers > 1 ) return;
	XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(nId, CALIBRATION_FILE_NAME);
	if (rc == XN_STATUS_OK)
	{
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
		printf("Load calibration file\n");
	}
	else
	{
		printf("request calibration\n");
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, true);
	}
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("Lost user %d\n", nId);
	gnUsers = gnUsers - 1;
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose %s detected for user %d\n", strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	printf("Calibration started for user %d\n", nId);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		SaveCalibration();
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		printf("Failure");
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, true);

	}
}


openNIKinectSkeletonData::openNIKinectSkeletonData(Context* context, DepthGenerator* depthGen, int jointCount) : 
	 KinectSkeletonData(jointCount), m_pContext(context), m_pDepthGen(depthGen){
	
	m_pUserGen = &g_UserGenerator;
	XnStatus nRetVal = m_pContext->FindExistingNode(XN_NODE_TYPE_USER, *m_pUserGen);
	if (nRetVal != XN_STATUS_OK)
	{
		nRetVal = m_pUserGen->Create(*m_pContext);
		CHECK_RC(nRetVal, "Find user generator");
	}

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected,hUserExit, hUserReEnter;
	if (!m_pUserGen->IsCapabilitySupported(XN_CAPABILITY_SKELETON))
	{
		printf("Supplied user generator doesn't support skeleton\n");
		return;
	}
	nRetVal = m_pUserGen->RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	CHECK_RC(nRetVal, "Register to user callbacks");
	nRetVal = m_pUserGen->GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
	CHECK_RC(nRetVal, "Register to calibration start");
	nRetVal = m_pUserGen->GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
	CHECK_RC(nRetVal, "Register to calibration complete");
	nRetVal = m_pUserGen->GetSkeletonCap().SetSmoothing(XnFloat(0.15f));

	m_pSkelPos = new UserSkelPos[MAXUSER];
	for( int i=0; i< MAXUSER; ++i)
	{
		m_pSkelPos[i].nUserId = i+1;
		for(int j = 0; j< MAXJOINT; ++j)
		{
			m_pSkelPos[i].jointPos[j].x = 0.0f;
			m_pSkelPos[i].jointPos[j].y = 0.0f;
			m_pSkelPos[i].jointPos[j].z = 0.0f;
			m_pSkelPos[i].isTracking = false;
		}
	}

	for( int i=1; i< MAXJOINT; ++i)
	{
		m_jointPos[i].x = 0.0f;
		m_jointPos[i].y = 0.0f;
		m_jointPos[i].z = 0.0f;
		m_joint3DPos[i].x = 0.0f;
		m_joint3DPos[i].y = 0.0f;
		m_joint3DPos[i].z = 0.0f;
		m_jointConf[i] = 0.0f;
	}
	m_pUserGen->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
}

bool openNIKinectSkeletonData::update() {
	XnUserID aUsers[4];
	XnUInt16 userCount= g_UserGenerator.GetNumberOfUsers();
	nUsers = userCount;


	g_UserGenerator.GetUsers(aUsers,userCount);
	
		
	for( int i=0; i< 1 ; ++i)
	{
		if( g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])) {
			//update skeleton position Data
			m_pSkelPos[i].isTracking = true;
			XnPoint3D pt, pt2;
			for( int j = 1 ; j < MAXJOINT ; j ++ ) {

				XnSkeletonJointPosition pos;
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition( aUsers[i], XnSkeletonJoint(j), pos);
				pt = pos.position;
		
				m_pSkelPos[i].jointPos[j].x = pos.position.X;
				m_pSkelPos[i].jointPos[j].y = pos.position.Y;
				m_pSkelPos[i].jointPos[j].z = pos.position.Z;

				m_joint3DPos[j].x = pt.X;
				m_joint3DPos[j].y = pt.Y;
				m_joint3DPos[j].z = pt.Z;
				m_pDepthGen->ConvertRealWorldToProjective(1, &pt, &pt2);

				m_pSkelPos[i].jointPos[j].x = pt2.X;
				m_pSkelPos[i].jointPos[j].y = pt2.Y;
				m_pSkelPos[i].jointPos[j].z = 0;
				m_jointPos[j].x = pt2.X;
				m_jointPos[j].y = pt2.Y;
				m_jointPos[j].z = 0;

			}	
			return true;
		}
		else 
				m_pSkelPos[i].isTracking = false;

	}
	return false;

}
