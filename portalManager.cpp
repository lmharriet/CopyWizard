#include "stdafx.h"
#include "portalManager.h"

HRESULT portalManager::init()
{
	centerPortal.curPt = { 0,0 };
	centerPortal.endPt = { 0,0 };
	centerPortal.isActive = false;
	centerPortal.isCol = false;
	centerPortal.rc = RectMakeCenter(centerPortal.curPt.x, centerPortal.curPt.y, 300, 300);
	return S_OK;
}

void portalManager::setPortal(tagPortal one, tagPortal two, tagPortal three)
{
	tagPortal arr[] = { one,two,three };
	
	for (int i = 0; i < 3; i++)portalPt[i] = arr[i];
}

void portalManager::render(HDC hdc)
{
	if (centerPortal.isActive)
	{
		CAMERAMANAGER->Rectangle(hdc, centerPortal.rc);
	}

	image* img = IMAGEMANAGER->findImage("portalFrame");
	for (int i = 0; i < 3; i++)
	{
		//CAMERAMANAGER->Rectangle(hdc, portalPt[i].rc);

		CAMERAMANAGER->AlphaFrameRender(hdc, img,
			portalPt[i].curPt.x - img->getFrameWidth()/2,
			portalPt[i].curPt.y - img->getFrameHeight()/2,
			portalPt[i].colorIndex, 0, 70);
	}
}

void portalManager::update(RECT rc)
{
	if (centerPortal.isActive)
	{
		if (colCheck(rc, centerPortal.rc))
		{
			centerPortal.isCol = true;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (colCheck(rc, portalPt[i].rc)) portalPt[i].isCol = true;
	}
}