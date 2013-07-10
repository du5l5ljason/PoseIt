#include "StdAfx.h"
#include "DrawWnd.h"
#include <XnCppWrapper.h>




void CDrawWnd::showSkeleton(POINT3D* jointPos, int* color)
{
	if(jointPos==NULL)return;

	int jointPair[][2] = { 
		{XN_SKEL_HEAD, XN_SKEL_NECK},					
		{XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER},
		{XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW},
		{XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND},
		{XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER},
		{XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW},
		{XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND},
		{XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO},
		{XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO},
		{XN_SKEL_TORSO, XN_SKEL_LEFT_HIP},
		{XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE},
		{XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT},
		{XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP},
		{XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE},
		{XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT},
		{XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP},
	};
	for( int i = 0 ;i < 16 ;i ++ ) {
		int id1 = jointPair[i][0];
		int id2 = jointPair[i][1];
		showLine(jointPos[id1].x, jointPos[id1].y, jointPos[id2].x, jointPos[id2].y, color);
	}
	
	int jointKey[3] = {
		XN_SKEL_HEAD,
		XN_SKEL_RIGHT_HAND,
		XN_SKEL_LEFT_HAND
	};

	int size1 = 40;
	int size2 = 5;

	//showEllipse(jointPos[jointKey[0]].x-size1, jointPos[jointKey[0]].y-size1,jointPos[jointKey[0]].x+size1, jointPos[jointKey[0]].y+size1, bluecolor);
	//showEllipse(jointPos[jointKey[1]].x-size2, jointPos[jointKey[1]].y-size2,jointPos[jointKey[1]].x+size2, jointPos[jointKey[1]].y+size2, redcolor);
	//showEllipse(jointPos[jointKey[2]].x-size2, jointPos[jointKey[2]].y-size2,jointPos[jointKey[2]].x+size2, jointPos[jointKey[2]].y+size2, redcolor);

}

void CDrawWnd::showRect(Rect rect, int* color)
{
	showLine( rect.x,rect.y, rect.x+rect.width, rect.y, color);
	showLine( rect.x,rect.y, rect.x, rect.y+rect.height, color);
	showLine( rect.x+rect.width, rect.y, rect.x+rect.width, rect.y+rect.height, color);
	showLine( rect.x+rect.width, rect.y+rect.height, rect.x, rect.y+rect.height, color);
}
	
void CDrawWnd::showHandJoint(POINT3D RightPos, int* color)
{
	int size = 10;

	//showLine(RightPos.x-size, RightPos.y, RightPos.x+size, RightPos.y, color);
	//showLine(RightPos.x, RightPos.y-size, RightPos.x, RightPos.y+size, color);
	showEllipse(RightPos.x-size, RightPos.y-size, RightPos.x+size, RightPos.y+size, color);
	//showEllipse(jointPos[jointKey[2]].x-size2, jointPos[jointKey[2]].y-size2,jointPos[jointKey[2]].x+size2, jointPos[jointKey[2]].y+size2, redcolor);
}
void CDrawWnd::showHandJoint(POINT3D leftPos, POINT3D rightPos, int *color)
{
	int size = 10;
	showEllipse(rightPos.x-size, rightPos.y-size, rightPos.x+size, rightPos.y+size, color);
	showEllipse(leftPos.x-size, leftPos.y-size, leftPos.x+size, leftPos.y+size, color);
}