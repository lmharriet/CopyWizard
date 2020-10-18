#include "stdafx.h"
#include "portalManager.h"

HRESULT portalManager::init()
{
	centerPortal.color = 0;
	centerPortal.curPt = { 0,0 };
	centerPortal.endPt = { 0,0 };
	centerPortal.isActive = false;

	return S_OK;
}

void portalManager::setPortal(tagPortal one, tagPortal two, tagPortal three)
{
	tagPortal arr[] = { one,two,three };
	
	for (int i = 0; i < 3; i++)portalPt[i] = arr[i];
}
