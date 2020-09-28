#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{

	_player = new player;
	_player->init();
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	loadMap("mapData/map2.map");
	_player->setTileAd(tile);

	cam = RectMakeCenter(0, 0, WINSIZEX, WINSIZEY);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);
	PARTICLE->init();


	
	_golem = new golem; //Å×½ºÆ®Áß (¸ó½ºÅÍ°ñ·½)
	_golem->init(); //Å×½ºÆ®Áß (¸ó½ºÅÍ°ñ·½)

	return S_OK;
}

void gameScene::release()
{
	//_player->release();
	//SAFE_DELETE(_player);
}

void gameScene::update()
{
	_player->update();

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX, WINSIZEY);

	//if (INPUT->GetKeyDown(VK_SPACE))PARTICLE->init(
	//	_player->getX(),
	//	_player->getY(),
	//	50);

	if (INPUT->GetKeyDown(VK_SPACE))
	{
		PARTICLE->pointInit(_player->getX(), _player->getY(), 0, 0, 0, 5);
	}
	PARTICLE->pointActive();

	if (INPUT->GetKeyDown(VK_BACK))PARTICLE->resetPoint();
}

void gameScene::render()
{
	//CAMERAMANAGER->Rectangle(getMemDC(), cam);
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false) continue;

		if (tile[i].keyName == "") continue;
		image* img = IMAGEMANAGER->findImage(tile[i].keyName);
		switch (tile[i].kind)
		{
		case TERRAIN::TILE:
			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
			break;
		case TERRAIN::WALL:
			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - (3 * TILESIZE), tile[i].frame.x, tile[i].frame.y);
			break;
		}
	}
	PARTICLE->active(getMemDC(), CAMERAMANAGER->getRect());

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	uiImg->render(getMemDC());

	_player->render();

	viewText();


	_golem->render(); //Å×½ºÆ®Áß (¸ó½ºÅÍ°ñ·½)
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