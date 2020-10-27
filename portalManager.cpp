#include "stdafx.h"
#include "portalManager.h"

HRESULT portalManager::init()
{
	IMAGEMANAGER->addImage("sceneWarpStone", "Images/object/sceneWarpStone.bmp", 136, 130, true, RGB(255, 0, 255));

	centerPortal.curPt = { 0,0 };
	centerPortal.endPt = { 0,0 };
	centerPortal.isActive = false;
	centerPortal.colorIndex = 0;
	centerPortal.isCol = false;
	centerPortal.rc = RectMakeCenter(centerPortal.curPt.x, centerPortal.curPt.y, 30, 60);
	return S_OK;
}

HRESULT portalManager::initWarp(float x, float y)
{
	warpScene.keyName = "sceneWarpStone";
	warpScene.x = x;
	warpScene.y = y;
	warpScene.rc = RectMakeCenter(x, y, 136, 130);
	return S_OK;
}

void portalManager::resetSceneWarp() 
{
	warpScene.x = 10000;
	warpScene.y = 10000;
	warpScene.rc = RectMakeCenter(10000, 10000, 0, 0);
}

void portalManager::setPortal(tagPortal one, tagPortal two)
{
	tagPortal arr[] = { one,two};
	
	for (int i = 0; i < 2; i++)portalPt[i] = arr[i];
}

void portalManager::backRender(HDC hdc)
{
	image* stone = IMAGEMANAGER->findImage("warpStone");
	image* sceneStone = IMAGEMANAGER->findImage("sceneWarpStone");

	CAMERAMANAGER->Render(hdc, sceneStone,
		warpScene.x - sceneStone->getWidth()/2,
		warpScene.y - sceneStone->getHeight()/2);

	for (int i = 0; i < 2; i++)
	{
		CAMERAMANAGER->Render(hdc, stone,
			portalPt[i].curPt.x - stone->getWidth() / 2,
			portalPt[i].curPt.y - stone->getHeight() / 2 + 70);
	}
}

void portalManager::bossScene_backRender(HDC hdc)
{
	image* sceneStone = IMAGEMANAGER->findImage("sceneWarpStone");

	CAMERAMANAGER->Render(hdc, sceneStone,
		warpScene.x - sceneStone->getWidth() / 2,
		warpScene.y - sceneStone->getHeight() / 2);
}

void portalManager::render(HDC hdc)
{
	image* img = IMAGEMANAGER->findImage("portalFrame");
	
	if (centerPortal.isActive)
	{
		CAMERAMANAGER->AlphaFrameRender(hdc, img,
			centerPortal.curPt.x - img->getFrameWidth() / 2,
			centerPortal.curPt.y - img->getFrameHeight() / 2,
			centerPortal.colorIndex, 0, 120);
	}

	for (int i = 0; i < 2; i++)
	{
		CAMERAMANAGER->AlphaFrameRender(hdc, img,
			portalPt[i].curPt.x - img->getFrameWidth()/2,
			portalPt[i].curPt.y - img->getFrameHeight()/2,
			portalPt[i].colorIndex, 0, 120);
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

	for (int i = 0; i < 2; i++)
	{
		if (colCheck(rc, portalPt[i].rc)) portalPt[i].isCol = true;
	}
}