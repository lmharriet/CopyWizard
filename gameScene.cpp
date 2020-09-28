#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{

	_player = new player;
	_player->init();
	_player->setTileAd(tile);
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	loadMap("mapData/map2.map");

	cam = RectMakeCenter(0, 0, WINSIZEX, WINSIZEY);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	return S_OK;
}

void gameScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void gameScene::update()
{
	_player->update();

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX, WINSIZEY);

}

void gameScene::render()
{
	CAMERAMANAGER->Rectangle(getMemDC(), cam);
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false) continue;

		if (tile[i].keyName != "")
		{
			image* img = IMAGEMANAGER->findImage(tile[i].keyName);

			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
		}
	}

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	uiImg->render(getMemDC());

	_player->render();

	viewText();

}

void gameScene::loadMap(const char* mapFileName)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(mapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void gameScene::viewText()
{
	//
}