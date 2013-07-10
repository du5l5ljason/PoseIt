#include "StdAfx.h"
#include "ksPoseUtils.h"
#include <math.h>
float weightMapArray[JOINTNUM][2] = {
	{XN_SKEL_HEAD, 0.08f},
	{XN_SKEL_NECK, 0.0f},
	{XN_SKEL_TORSO, 0.04f},
	{XN_SKEL_LEFT_SHOULDER, 0.06f}, 
	{XN_SKEL_LEFT_ELBOW, 0.08f},
	{XN_SKEL_LEFT_HAND, 0.08f},
	{XN_SKEL_RIGHT_SHOULDER, 0.06f}, 
	{XN_SKEL_RIGHT_ELBOW, 0.08f},
	{XN_SKEL_RIGHT_HAND, 0.08},
	{XN_SKEL_LEFT_HIP, 0.06f},
	{XN_SKEL_LEFT_KNEE, 0.08f},
	{XN_SKEL_LEFT_FOOT, 0.08f},
	{XN_SKEL_RIGHT_HIP, 0.06f},
	{XN_SKEL_RIGHT_KNEE, 0.08f},
	{XN_SKEL_RIGHT_FOOT, 0.08f}
};

//Implementation
float ksPoseUtils::getDistBetween( ksPose &newPose, ksPose &srcPose )
{
	float dist = 0.0f;
	for( int id = 0 ; id < JOINTNUM ; ++id )
		dist += weightMapArray[id][1] * calcConfidenceAt( weightMapArray[id][0], newPose, srcPose );

	return dist;
}

int ksPoseUtils::evalDist( float dist )
{
	if( dist < 0 || dist > 1)
		dist = 1.0f;
	
	// 1 - bad ;  2 - good ; 3 - excellent
	if( dist >=0.7 && dist <= 1)
		return 1;
	else if( dist > 0.3 && dist < 0.7 )
		return 2;
	else if( dist > 0 && dist <=0.3 )
		return 3;
}

float ksPoseUtils::calcConfidenceAt( int id, ksPose &newPose, ksPose &srcPose )
{
	JOINTPOS newJointPos = newPose.getJointPosAt( id );
	JOINTPOS srcJointPos = srcPose.getJointPosAt( id );

	return calcEuclideanDist( newJointPos.pos, srcJointPos.pos ) / MAXDIST;
}	

float ksPoseUtils::calcEuclideanDist( POINT3D point1, POINT3D point2 )
{
	float dist = sqrt( (point1.x - point2.x)*(point1.x - point2.x) +
					   (point1.y - point2.y)*(point1.y - point2.y) +
					   (point1.z - point2.z)*(point1.z - point2.z) );
	
	return dist<MAXDIST?dist:MAXDIST;
}