#pragma once
#include "singletonBase.h"

struct tagPortal
{
	RECT rc;

	POINT curPt;	// 시작점
	POINT endPt;	// 도착점
	
	int colorIndex;	//  purple : 0, green : 1, red : 2

	bool isActive;
	bool isCol;
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
	bool getCenterCol() { return centerPortal.isCol; }
	void setCenterCol(bool check) { centerPortal.isCol = check; }

	void setCenterActive(bool check) { centerPortal.isActive = check; }

	void setPortal(tagPortal one, tagPortal two, tagPortal three);
	bool getPortalCol(int index) { return portalPt[index].isCol; }
	POINT getPortalEndPt(int index) { return portalPt[index].endPt; }
	void resetCol() { for (int i = 0; i < 3; i++)portalPt[i].isCol = false; }
	int getPortalColor(int index) { return portalPt[index].colorIndex; }

	void render(HDC hdc);
	void update(RECT rc);
};