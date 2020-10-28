#include "stdafx.h"
#include "lobbyScene.h"

HRESULT lobbyScene::init()
{
	PLAYERDATA->init();

	UI->init();
	PARTICLE->init();
	EFFECT->init();
	ITEM->init();
	DAMAGE->init();

	addImage();
	
	_player = new player;
	_player->init();
	_player->setX(1184);
	_player->setY(1176);
	image* img = IMAGEMANAGER->findImage("lobbyRoom");

	CAMERAMANAGER->init(_player->getX(), _player->getY(), img->getWidth() * 2, img->getHeight() * 2,
		-img->getWidth(), -img->getHeight(), WINSIZEX / 2, WINSIZEY / 2);

	rc = RectMakeCenter(1184, 1400, 100, 100);

	return S_OK;
}

void lobbyScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void lobbyScene::update()
{
	UI->update();
	CAMERAMANAGER->update();
	PLAYERDATA->update();
	_player->other_update();
	if (INPUT->GetKeyDown('J'))
	{
		SCENEMANAGER->loadScene("로딩화면");
	}
	
	image* img = IMAGEMANAGER->findImage("lobbyRoom");
	_player->colorCheck(img);

	PARTICLE->explosionActive();
	PARTICLE->pointActive();

	warpToGameScene();
}

void lobbyScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("lobbyRoom"),0,0);
	CAMERAMANAGER->Rectangle(getMemDC(), rc);
	_player->attackCircleRender();
	EFFECT->render(getMemDC());
	_player->render(1);
	PARTICLE->render(getMemDC());
	UI->render(getMemDC(),50,50);
	

	viewText();
}

void lobbyScene::addImage()
{
	IMAGEMANAGER->addImage("lobbyRoom", "resource/lobby/dummy.bmp", 1181*2, 834*2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
	IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
	IMAGEMANAGER->addFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
	IMAGEMANAGER->addFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
	IMAGEMANAGER->addFrameImage("playerSpearFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);

}

void lobbyScene::viewText()
{
	char str[126];
	sprintf(str, "%.f, %.f", _player->getX(), _player->getY());
	TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));
}

void lobbyScene::warpToGameScene()
{
	if (colCheck(_player->getRect(), rc) && INPUT->GetKeyDown('F'))
	{
		SCENEMANAGER->loadScene("로딩화면");
	}
}