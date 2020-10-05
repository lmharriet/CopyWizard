#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{
	UI->init();
	DROP->init();
	
	_player = new player;
	_player->init();
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	loadMap("mapData/map0.map");
	_player->setTileAd(tile);
	_player->setTileAd0(vTile);
	
	cam = RectMakeCenter(0, 0, WINSIZEX, WINSIZEY);
	checkArea = RectMakeCenter(WINSIZEX/2, WINSIZEY/2, 100, 100);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	PARTICLE->init();
	EFFECT->init();

	enemy = new enemyManager;
	enemy->init(tile);

	//vTile.clear();
	collisionTile();

	return S_OK;
}

void gameScene::release()
{
	_player->release();
	SAFE_DELETE(_player);

	enemy->release();
	SAFE_DELETE(enemy);
}

void gameScene::update()
{
	collisionTile();
	_player->setTileAd0(vTile);

	UI->update();
	DROP->update();

	_player->update();
	
	enemy->update();


	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX, WINSIZEY);
	//checkArea = RectMakeCenter(_player->getX(), _player->getY(), 400, 400);
	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY/2 + 420, 200, 500);

	//if (INPUT->GetKeyDown(VK_SPACE))PARTICLE->init(
	//	_player->getX(),
	//	_player->getY(),
	//	50);

	if (INPUT->GetKeyDown(VK_BACK))
	{
		PARTICLE->pointGenerate(_player->getX(), _player->getY(), 0, 0, 0, 5, 60);
	}
	PARTICLE->pointActive();

	/*if (INPUT->GetKeyDown(VK_BACK))PARTICLE->resetPoint();*/


	//col check ( player -> minion )

	//for (int i = 0; i < enemy->getMinion().size(); i++)
	//{
	//	if (enemy->getMinion()[i]->getFind() == true)continue;

	//	if (colCheck(_player->getRect(), enemy->getMinion()[i]->getArea()))
	//	{
	//		cout << i << "번 째 몬스터가 플레이어를 감지했다 !" << '\n';
	//		enemy->getMinion()[i]->setFind(true);
	//	}
	//}

	//enemy->minionBulletFire(_player->getX(), _player->getY());
	enemy->collision(_player->getRect());

	for (int i = 0; i < _player->getBlaze()->getBullet().size(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getBlaze()->getBullet()[i].rc, enemy->getMinion()[j]->getRC()))
			{
				enemy->getMinion()[j]->hit(10, _player->getBlaze()->getBullet()[i].angle, 30.f);
				EFFECT->setEffect("flameStrike",
					{ (long)_player->getBlaze()->getBullet()[i].x
					,(long)_player->getBlaze()->getBullet()[i].y });
				_player->getBlaze()->removeBomb(i);
				break;
				
			}

		}
	}
}

void gameScene::render()
{
	//_golem->setCamRC(cam);
	//_golem2->setCamRC(cam);
	enemy->setPlayerRC(RectMake(_player->getX(), _player->getY(), 100, 100));

	vector<int>::iterator iter = vTile.begin();

	for (iter; iter != vTile.end(); ++iter)
	{
		if (tile[*iter].keyName == "" || tile[*iter].kind != TERRAIN::TILE)continue;

		image* img = IMAGEMANAGER->findImage(tile[*iter].keyName);
		string key = tile[*iter].keyName;
		int height = 0;

		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top, tile[*iter].frame.x, tile[*iter].frame.y);
	}

	//CAMERAMANAGER->Rectangle(getMemDC(), checkArea);
	//Rectangle(getMemDC(), checkArea);
	//_player->render();
	
	DROP->render(getMemDC());
	EFFECT->pRender(getMemDC());

	bool isRender = false;

	//CAMERAMANAGER->Rectangle(getMemDC(), cam);

	iter = vTile.begin();
	for (iter; iter != vTile.end(); ++iter)
	{
		if (tile[*iter].keyName == "" || tile[*iter].kind != TERRAIN::WALL) continue;

		if (!isRender &&
			(colCheck(checkArea, tile[*iter].rc) ||
				colCheck(checkArea, tile[*iter + 15].rc) ||
				colCheck(checkArea, tile[*iter - MAXTILE_WIDTH * 4 + 10].rc) ||
				colCheck(checkArea, tile[*iter - MAXTILE_WIDTH * 4 + 5].rc))
			)
		{
			isRender = true;
			_player->render();
		}

		image* img = IMAGEMANAGER->findImage(tile[*iter].keyName);
		string key = tile[*iter].keyName;
		int height = 0;

		int w = MAXTILE_WIDTH;
		int w2 = MAXTILE_WIDTH * 2;
		int w3 = MAXTILE_WIDTH * 3;
		int w4 = MAXTILE_WIDTH * 4;
		int w5 = MAXTILE_WIDTH * 5;
		int w6 = MAXTILE_WIDTH * 6;

		if (tile[*iter].keyName == "topWall")
		{
			if ((key == tile[*iter + 1].keyName &&
				key == tile[*iter - w].keyName && key == tile[*iter - w + 1].keyName &&
				key == tile[*iter - w2].keyName && key == tile[*iter - w2 + 1].keyName &&
				key == tile[*iter - w3].keyName && key == tile[*iter - w3 + 1].keyName &&
				key == tile[*iter - w4].keyName && key == tile[*iter - w4 + 1].keyName &&

				key == tile[*iter - w3 + 2].keyName && key == tile[*iter - w3 + 3].keyName &&
				key == tile[*iter - w3 + 4].keyName && key == tile[*iter - w3 + 5].keyName &&
				key == tile[*iter - w4 + 2].keyName && key == tile[*iter - w4 + 3].keyName &&
				key == tile[*iter - w4 + 4].keyName && key == tile[*iter - w4 + 5].keyName &&

				key == tile[*iter - w4 + 10].keyName && key == tile[*iter - w4 + 11].keyName &&
				key == tile[*iter - w4 + 12].keyName && key == tile[*iter - w4 + 13].keyName &&
				key == tile[*iter - w4 + 14].keyName && key == tile[*iter - w4 + 15].keyName &&

				key == tile[*iter - w3 + 12].keyName && key == tile[*iter - w3 + 13].keyName &&
				key == tile[*iter - w3 + 10].keyName && key == tile[*iter - w3 + 11].keyName &&
				key == tile[*iter - w3 + 14].keyName && key == tile[*iter - w3 + 15].keyName &&

				key == tile[*iter - w2 + 14].keyName && key == tile[*iter - w2 + 15].keyName &&
				key == tile[*iter - w + 14].keyName && key == tile[*iter - w + 15].keyName &&
				key == tile[*iter + 14].keyName && key == tile[*iter + 15].keyName)
				== false)
			{
				continue;
			}

			height = 8 * TILESIZE;

			if (tile[*iter].rc.bottom > (_player->getY() + 30) &&
				((colCheck(checkArea, tile[*iter].rc) || colCheck(checkArea, tile[*iter + 15].rc) ||
					colCheck(checkArea, tile[*iter - w4 + 10].rc) || colCheck(checkArea, tile[*iter - w4 + 5].rc))))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top - height);
		}
		else if (tile[*iter].keyName == "bottomWall")
		{

			if ((key == tile[*iter + 1].keyName &&
				key == tile[*iter - w].keyName && key == tile[*iter - w + 1].keyName &&
				key == tile[*iter - w2].keyName && key == tile[*iter - w2 + 1].keyName &&
				key == tile[*iter - w3].keyName && key == tile[*iter - w3 + 1].keyName &&
				key == tile[*iter - w4].keyName && key == tile[*iter - w4 + 1].keyName &&
				key == tile[*iter - w5].keyName && key == tile[*iter - w5 + 1].keyName &&
				key == tile[*iter - w6].keyName && key == tile[*iter - w6 + 1].keyName &&

				key == tile[*iter - w + 2].keyName && key == tile[*iter - w + 3].keyName &&
				key == tile[*iter - w + 4].keyName &&
				key == tile[*iter + 2].keyName && key == tile[*iter + 3].keyName &&
				key == tile[*iter + 4].keyName &&


				key == tile[*iter - w + 11].keyName && key == tile[*iter - w + 12].keyName &&
				key == tile[*iter - w + 13].keyName &&
				key == tile[*iter + 11].keyName && key == tile[*iter + 12].keyName &&
				key == tile[*iter + 13].keyName &&

				key == tile[*iter + 14].keyName && key == tile[*iter + 15].keyName &&
				key == tile[*iter - w + 14].keyName && key == tile[*iter - w + 15].keyName &&
				key == tile[*iter - w2 + 14].keyName && key == tile[*iter - w2 + 15].keyName &&
				key == tile[*iter - w3 + 14].keyName && key == tile[*iter - w3 + 15].keyName &&
				key == tile[*iter - w4 + 14].keyName && key == tile[*iter - w4 + 15].keyName &&
				key == tile[*iter - w5 + 14].keyName && key == tile[*iter - w5 + 15].keyName &&
				key == tile[*iter - w6 + 14].keyName && key == tile[*iter - w6 + 15].keyName
				) == false)
			{
				continue;
			}
			height = 8 * TILESIZE;


			if (tile[*iter].rc.bottom > (_player->getY() + 30) &&
				(colCheck(checkArea, tile[*iter].rc) || colCheck(checkArea, tile[*iter + 15].rc)))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top - height);
		}
		else
		{
			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[*iter].rc.left, tile[*iter].rc.top - (3 * TILESIZE), tile[*iter].frame.x, tile[*iter].frame.y);
		}
	}

	PARTICLE->render(getMemDC(), CAMERAMANAGER->getRect());

	enemy->render();
	if(!isRender) _player->render();
	EFFECT->render(getMemDC());
	EFFECT->dRender(getMemDC());

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	//uiImg->render(getMemDC());


	_player->invenRender();
	UI->render(getMemDC(), 50, 50);

	viewText();
	
	//_golem->render(); //테스트중 (몬스터골렘)
	//_golem2->render(); //테스트중 (몬스터골렘)

	
}

void gameScene::collisionTile()
{
	vTile.clear();
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc)) vTile.push_back(i);
	}
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