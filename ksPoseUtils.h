#pragma once
#include "ksPose.h"
#include "StructDefine.h"
#include <XnCppWrapper.h>

using namespace xn;
enum DIST
{
	_DEFAULT, 
	_BAD,
	_GOOD,
	_EXCELLENT,
};
//Show XN_SKEL_
//define weight for each joint
	//XN_SKEL_HEAD			= 1,
	//XN_SKEL_NECK			= 2,
	//XN_SKEL_TORSO			= 3,
	//XN_SKEL_WAIST			= 4,

	//XN_SKEL_LEFT_COLLAR		= 5,
	//XN_SKEL_LEFT_SHOULDER	= 6,
	//XN_SKEL_LEFT_ELBOW		= 7,
	//XN_SKEL_LEFT_WRIST		= 8,
	//XN_SKEL_LEFT_HAND		= 9,
	//XN_SKEL_LEFT_FINGERTIP	=10,

	//XN_SKEL_RIGHT_COLLAR	=11,
	//XN_SKEL_RIGHT_SHOULDER	=12,
	//XN_SKEL_RIGHT_ELBOW		=13,
	//XN_SKEL_RIGHT_WRIST		=14,
	//XN_SKEL_RIGHT_HAND		=15,
	//XN_SKEL_RIGHT_FINGERTIP	=16,

	//XN_SKEL_LEFT_HIP		=17,
	//XN_SKEL_LEFT_KNEE		=18,
	//XN_SKEL_LEFT_ANKLE		=19,
	//XN_SKEL_LEFT_FOOT		=20,

	//XN_SKEL_RIGHT_HIP		=21,
	//XN_SKEL_RIGHT_KNEE		=22,
	//XN_SKEL_RIGHT_ANKLE		=23,
	//XN_SKEL_RIGHT_FOOT		=24
//a JOINTNUM*2 array[jointIdx][weight];
//define max distance for each joint couples
#define MAXDIST 500
class ksPoseUtils
{
public:
	ksPoseUtils(void){};
	~ksPoseUtils(void){};

public:
	float	getDistBetween( ksPose & , ksPose & );
	int		evalDist( float dist );
private:
	float	calcConfidenceAt( int, ksPose &, ksPose & );		//a simply dist/MaxDist range from 0~1
	float	calcEuclideanDist( POINT3D, POINT3D );
};

