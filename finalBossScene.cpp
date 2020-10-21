#include "stdafx.h"
#include "finalBossScene.h"

HRESULT finalBossScene::init()
{
	IMAGEMANAGER->addImage("finalbossroom", "resource/boss/final boss room.bmp", 2048, 2048, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalbackframe", "resource/boss/final boss room margenta.bmp", 2048, 2048, true, RGB(255, 0, 255));

	_finalBoss = new finalBoss;
	_finalBoss->init(752, 288);
	return S_OK;
}

void finalBossScene::release()
{
	if (_finalBoss != nullptr)
	{
		_finalBoss->release();
		SAFE_DELETE(_finalBoss);
	}
}

void finalBossScene::update()
{
	_finalBoss->update();
}

void finalBossScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbackframe"), 0, 0);
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbossroom"), 0, 0);
	_finalBoss->render();
}
