#include "stdafx.h"
#include "finalBossScene.h"

HRESULT finalBossScene::init()
{
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
	_finalBoss->render();
}
