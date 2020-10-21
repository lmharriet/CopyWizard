#include "stdafx.h"
#include "finalBossScene.h"

HRESULT finalBossScene::init()
{
	IMAGEMANAGER->addImage("finalbossroom", "resource/boss/final boss room.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalbackframe", "resource/boss/final boss room margenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("finalroom", "resource/boss/finalroom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalframe", "resource/boss/finalroommargenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	UI->init();

	_player = new player;
	_player->init();
	_player->setX(MAXWIDTH / 2);
	_player->setY(MAXHEIGHT - 200);
	_player->reposRect();
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXWIDTH * 2, MAXHEIGHT * 2,
		-MAXWIDTH, -MAXHEIGHT, WINSIZEX / 2, WINSIZEY / 2);

	EFFECT->init();

	_finalBoss = new finalBoss;
	_finalBoss->init(MAXWIDTH / 2, MAXHEIGHT / 2);

	rc = RectMakeCenter(MAXWIDTH / 2, MAXHEIGHT / 2, 150, 150);
	area = RectMakeCenter(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, WINSIZEX, WINSIZEY);

	isBattle = false;
	mapChange = false;
	cutTime = 0;
	mapCount = 0;

	_finalBoss->getPlayerInfo(_player);

	PARTICLE->init();
	DAMAGE->init();

	return S_OK;
}

void finalBossScene::release()
{
	if (_finalBoss != nullptr)
	{
		_player->release();
		SAFE_DELETE(_player);

		_finalBoss->release();
		SAFE_DELETE(_finalBoss);
	}
}

void finalBossScene::update()
{
	_player->getBlaze()->setBossScene(true);

	PLAYERDATA->update();

	//camera update
	CAMERAMANAGER->update();

	//player update
	if (!CAMERAMANAGER->getIsCutScene())
	{
		UI->update();
		_player->other_update();
	}
	else _player->setStateIDLE();


	//pixel collision
	_player->colorCheck(IMAGEMANAGER->findImage("finalbackframe"));

	//cutScene check
	bossCutScene();

	if (isBattle) {
		_finalBoss->update();
	}

	if (isBattle && !mapChange) {
		mapCount++;
		if (mapCount > 180) {
			CAMERAMANAGER->Shake(50, 50, 10);
			mapCount = 0;
			mapChange = true;
		}
	}

	PARTICLE->pointActive();
	PARTICLE->explosionActive();
}

void finalBossScene::render()
{
	if (!mapChange) {
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalframe"), 0, 0);
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalroom"), 0, 0);
	}
	else {
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbackframe"), 0, 0);
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbossroom"), 0, 0);
	}

	EFFECT->pRender(getMemDC());

	_player->render(1);

	_finalBoss->render();

	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);

	_player->invenRender();

	if (isBattle) {
		char str[128];
		wsprintf(str, "MATER SURA");
		_finalBoss->finalBossHpInfo(getMemDC(), WINSIZEX / 2 - 226, 100);

		TextOut(getMemDC(), WINSIZEX / 2 - 70, 90, str, strlen(str));

	}

	DAMAGE->render(getMemDC());
}

void finalBossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		isBattle = true; // 한번만 실행하기 위함
		CAMERAMANAGER->setCutScene
		(
			_player->getX(),                        // 시작 좌표 x
			_player->getY(),                        // 시작 좌표 y
			rc.left + (rc.right - rc.left) / 2,     // 목적 좌표 x
			rc.top + (rc.bottom - rc.top) / 2,      // 목적 좌표 y
			true,                                   // 시작 -> 목적 isLerp?
			true,                                   // 목적 -> 시작 isLerp?
			250,                                    // 컷씬 유지 시간
			50,                                     // 시작지점으로 되돌아오는 시간
			10.f                                    // lerp 속도
		);
	}
}
