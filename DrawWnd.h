#pragma once
#include "imgwnd.h"
#include "StructDefine.h"


class CDrawWnd :
	public CImgWnd
{
public:
	CDrawWnd(void){};
	~CDrawWnd(void){};

	void showSkeleton(POINT3D* jointPos, int* color);
	void showRect(Rect rect, int* color);
	void showHandJoint(POINT3D RightPos, int* color);
	void showHandJoint(POINT3D leftPos, POINT3D RightPos, int* color);
};

