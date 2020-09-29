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
	checkArea = RectMakeCenter(WINSIZEX/2, WINSIZEY/2, 100, 100);
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
	_golem->update();

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX, WINSIZEY);
	//checkArea = RectMakeCenter(_player->getX(), _player->getY(), 400, 400);
	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY/2 + 420, 200, 500);

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
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false || tile[i].keyName == "" || tile[i].kind != TERRAIN::TILE) continue;

		image* img = IMAGEMANAGER->findImage(tile[i].keyName);
		string key = tile[i].keyName;
		int height = 0;

		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	}

	//CAMERAMANAGER->Rectangle(getMemDC(), checkArea);
	//Rectangle(getMemDC(), checkArea);
	//_player->render();

	bool isRender = false;

	//CAMERAMANAGER->Rectangle(getMemDC(), cam);
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false || tile[i].keyName == "" || tile[i].kind != TERRAIN::WALL) continue;

		if (!isRender && 
			(colCheck(checkArea, tile[i].rc)						  ||
			 colCheck(checkArea, tile[i + 15].rc)					  ||
			 colCheck(checkArea, tile[i - MAXTILE_WIDTH * 4 + 10].rc) ||
			 colCheck(checkArea, tile[i - MAXTILE_WIDTH * 4 + 5].rc))
			)
		{
			isRender = true;
			_player->render();
		}

		image* img = IMAGEMANAGER->findImage(tile[i].keyName);
		string key = tile[i].keyName;
		int height = 0;

		int w = MAXTILE_WIDTH;
		int w2 = MAXTILE_WIDTH * 2;
		int w3 = MAXTILE_WIDTH * 3;
		int w4 = MAXTILE_WIDTH * 4;
		int w5 = MAXTILE_WIDTH * 5;
		int w6 = MAXTILE_WIDTH * 6;

		if (tile[i].keyName == "topWall")
		{
			if ((key == tile[i + 1].keyName &&
				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&

				key == tile[i - w3 + 2].keyName && key == tile[i - w3 + 3].keyName &&
				key == tile[i - w3 + 4].keyName && key == tile[i - w3 + 5].keyName &&
				key == tile[i - w4 + 2].keyName && key == tile[i - w4 + 3].keyName &&
				key == tile[i - w4 + 4].keyName && key == tile[i - w4 + 5].keyName &&

				key == tile[i - w4 + 10].keyName && key == tile[i - w4 + 11].keyName &&
				key == tile[i - w4 + 12].keyName && key == tile[i - w4 + 13].keyName &&
				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&

				key == tile[i - w3 + 10].keyName && key == tile[i - w3 + 11].keyName &&
				key == tile[i - w3 + 12].keyName && key == tile[i - w3 + 13].keyName &&
				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&

				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
				key == tile[i + 14].keyName && key == tile[i + 15].keyName)
				== false)
			{
				continue;
			}

			height = 8 * TILESIZE;

			if (tile[i].rc.bottom > (_player->getY() + 30) && 
				((colCheck(checkArea, tile[i].rc) || colCheck(checkArea, tile[i+15].rc) ||
				  colCheck(checkArea, tile[i-w4+10].rc) || colCheck(checkArea,tile[i-w4+5].rc) )))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
		}
		else if (tile[i].keyName == "bottomWall")
		{

			if ((key == tile[i + 1].keyName &&
				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&
				key == tile[i - w5].keyName && key == tile[i - w5 + 1].keyName &&
				key == tile[i - w6].keyName && key == tile[i - w6 + 1].keyName &&

				key == tile[i - w + 2].keyName && key == tile[i - w + 3].keyName &&
				key == tile[i - w + 4].keyName &&
				key == tile[i + 2].keyName && key == tile[i + 3].keyName &&
				key == tile[i + 4].keyName &&


				key == tile[i - w + 11].keyName && key == tile[i - w + 12].keyName &&
				key == tile[i - w + 13].keyName &&
				key == tile[i + 11].keyName && key == tile[i + 12].keyName &&
				key == tile[i + 13].keyName &&

				key == tile[i + 14].keyName && key == tile[i + 15].keyName &&
				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&
				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&
				key == tile[i - w5 + 14].keyName && key == tile[i - w5 + 15].keyName &&
				key == tile[i - w6 + 14].keyName && key == tile[i - w6 + 15].keyName
				) == false)
			{
				continue;
			}
			height = 8 * TILESIZE;


			if (tile[i].rc.bottom > (_player->getY() + 30) && 
				(colCheck(checkArea, tile[i].rc) || colCheck(checkArea, tile[i + 15].rc)))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
		}
		else
		{
			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - (3 * TILESIZE), tile[i].frame.x, tile[i].frame.y);
		}
	}
	PARTICLE->active(getMemDC(), CAMERAMANAGER->getRect());

	if(!isRender) _player->render();

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	uiImg->render(getMemDC());


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