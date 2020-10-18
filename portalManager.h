#pragma once
#include "singletonBase.h"

struct tagPortal
{
	POINT curPt;	// 시작점
	POINT endPt;	// 도착점
	int color;

	bool isActive;
};

class portalManager : public singletonBase <portalManager>
{
private:
	tagPortal centerPortal;
	tagPortal portalPt[3];
public:
	HRESULT init();

	void setCenterPortal(tagPortal center) { centerPortal = center; }
	POINT getCenterPortalEndPt() { return centerPortal.endPt; }
	void setCenterActive(bool check) { centerPortal.isActive = check; }

	void setPortal(tagPortal one, tagPortal two, tagPortal three);
};