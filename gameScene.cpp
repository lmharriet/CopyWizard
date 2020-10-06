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

	//PARTICLE->init();
	PARTICLE0->init();
	EFFECT->init();

	enemy = new enemyManager;
	enemy->init(tile);

	//vTile.clear();
	collisionTile();

	UI->setCoin(PLAYERDATA->getCoin());
	UI->setHp(PLAYERDATA->getHp());
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


	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX+15, WINSIZEY+15);
	//checkArea = RectMakeCenter(_player->getX(), _player->getY(), 400, 400);
	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY/2 + 420, 200, 500);

	//if (INPUT->GetKeyDown(VK_SPACE))PARTICLE->init(
	//	_player->getX(),
	//	_player->getY(),
	//	50);

	if (INPUT->GetKeyDown(VK_BACK))
	{
		PARTICLE0->pointGenerate(_player->getX(), _player->getY(), 2, 50, 15, 20.f, 0.5f, 20);
		//PARTICLE->pointGenerate(_player->getX(), _player->getY(), 0, 0, 0, 5, 60);
	}
	PARTICLE0->pointActive();

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

	//collision between player skill and enemy
	playerAttack();

}

void gameScene::render()
{
	//_golem->setCamRC(cam);
	//_golem2->setCamRC(cam);
	enemy->setPlayerRC(RectMake(_player->getX(), _player->getY(), 100, 100));

	for (int i = 0; i < vTile.size(); i++)
	{
		int num = vTile[i];

		if (tile[num].keyName == "" || tile[num].kind != TERRAIN::TILE)continue;

		image* img = IMAGEMANAGER->findImage(tile[num].keyName);

		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top, tile[num].frame.x, tile[num].frame.y);
	}

	//CAMERAMANAGER->Rectangle(getMemDC(), checkArea);
	//Rectangle(getMemDC(), checkArea);
	//_player->render();
	
	DROP->render(getMemDC());
	EFFECT->pRender(getMemDC());

	bool isRender = false;

	//CAMERAMANAGER->Rectangle(getMemDC(), cam);

	for (int i = 0; i < vTile.size(); i++)
	{
		int num = vTile[i];

		if (tile[num].keyName == "" || tile[num].kind != TERRAIN::WALL) continue;

		if (!isRender &&
			(colCheck(checkArea, tile[num].rc) ||
				colCheck(checkArea, tile[num + 15].rc) ||
				colCheck(checkArea, tile[num - MAXTILE_WIDTH * 4 + 10].rc) ||
				colCheck(checkArea, tile[num - MAXTILE_WIDTH * 4 + 5].rc))
			)
		{
			isRender = true;
			_player->render();
		}

		image* img = IMAGEMANAGER->findImage(tile[num].keyName);
		string key = tile[num].keyName;
		int height = 0;

		int w = MAXTILE_WIDTH;
		int w2 = MAXTILE_WIDTH * 2;
		int w3 = MAXTILE_WIDTH * 3;
		int w4 = MAXTILE_WIDTH * 4;
		int w5 = MAXTILE_WIDTH * 5;
		int w6 = MAXTILE_WIDTH * 6;

		if (tile[num].keyName == "topWall")
		{
			if ((key == tile[num + 1].keyName &&
				key == tile[num - w].keyName && key == tile[num - w + 1].keyName &&
				key == tile[num - w2].keyName && key == tile[num - w2 + 1].keyName &&
				key == tile[num - w3].keyName && key == tile[num - w3 + 1].keyName &&
				key == tile[num - w4].keyName && key == tile[num - w4 + 1].keyName &&

				key == tile[num - w3 + 2].keyName && key == tile[num - w3 + 3].keyName &&
				key == tile[num - w3 + 4].keyName && key == tile[num - w3 + 5].keyName &&
				key == tile[num - w4 + 2].keyName && key == tile[num - w4 + 3].keyName &&
				key == tile[num - w4 + 4].keyName && key == tile[num - w4 + 5].keyName &&

				key == tile[num - w4 + 10].keyName && key == tile[num - w4 + 11].keyName &&
				key == tile[num - w4 + 12].keyName && key == tile[num - w4 + 13].keyName &&
				key == tile[num - w4 + 14].keyName && key == tile[num - w4 + 15].keyName &&

				key == tile[num - w3 + 12].keyName && key == tile[num - w3 + 13].keyName &&
				key == tile[num - w3 + 10].keyName && key == tile[num - w3 + 11].keyName &&
				key == tile[num - w3 + 14].keyName && key == tile[num - w3 + 15].keyName &&

				key == tile[num - w2 + 14].keyName && key == tile[num - w2 + 15].keyName &&
				key == tile[num - w + 14].keyName && key == tile[num - w + 15].keyName &&
				key == tile[num + 14].keyName && key == tile[num + 15].keyName)
				== false)
			{
				continue;
			}

			height = 8 * TILESIZE;

			if (tile[num].rc.bottom > (_player->getY() + 30) &&
				((colCheck(checkArea, tile[num].rc) || colCheck(checkArea, tile[num + 15].rc) ||
					colCheck(checkArea, tile[num - w4 + 10].rc) || colCheck(checkArea, tile[num - w4 + 5].rc))))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[num].rc.left, tile[num].rc.top - height);
		}
		else if (tile[num].keyName == "bottomWall")
		{

			if ((key == tile[num + 1].keyName &&
				key == tile[num - w].keyName && key == tile[num - w + 1].keyName &&
				key == tile[num - w2].keyName && key == tile[num - w2 + 1].keyName &&
				key == tile[num - w3].keyName && key == tile[num - w3 + 1].keyName &&
				key == tile[num - w4].keyName && key == tile[num - w4 + 1].keyName &&
				key == tile[num - w5].keyName && key == tile[num - w5 + 1].keyName &&
				key == tile[num - w6].keyName && key == tile[num - w6 + 1].keyName &&

				key == tile[num - w + 2].keyName && key == tile[num - w + 3].keyName &&
				key == tile[num - w + 4].keyName &&
				key == tile[num + 2].keyName && key == tile[num + 3].keyName &&
				key == tile[num + 4].keyName &&


				key == tile[num - w + 11].keyName && key == tile[num - w + 12].keyName &&
				key == tile[num - w + 13].keyName &&
				key == tile[num + 11].keyName && key == tile[num + 12].keyName &&
				key == tile[num + 13].keyName &&

				key == tile[num + 14].keyName && key == tile[num + 15].keyName &&
				key == tile[num - w + 14].keyName && key == tile[num - w + 15].keyName &&
				key == tile[num - w2 + 14].keyName && key == tile[num - w2 + 15].keyName &&
				key == tile[num - w3 + 14].keyName && key == tile[num - w3 + 15].keyName &&
				key == tile[num - w4 + 14].keyName && key == tile[num - w4 + 15].keyName &&
				key == tile[num - w5 + 14].keyName && key == tile[num - w5 + 15].keyName &&
				key == tile[num - w6 + 14].keyName && key == tile[num - w6 + 15].keyName
				) == false)
			{
				continue;
			}
			height = 8 * TILESIZE;


			if (tile[num].rc.bottom > (_player->getY() + 30) &&
				(colCheck(checkArea, tile[num].rc) || colCheck(checkArea, tile[num + 15].rc)))
			{
				CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top - height, 150);
			}
			else CAMERAMANAGER->Render(getMemDC(), img, tile[num].rc.left, tile[num].rc.top - height);
		}
		else
		{
			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top - (3 * TILESIZE), tile[num].frame.x, tile[num].frame.y);
		}
	}

	//PARTICLE->render(getMemDC(), CAMERAMANAGER->getRect());
	PARTICLE0->render(getMemDC());

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

void gameScene::playerAttack()
{
	//blaze
	for (int i = 0; i < _player->getBlaze()->getBullet().size(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getBlaze()->getBullet()[i].rc, enemy->getMinion()[j]->getRC()))
			{
				enemy->getMinion()[j]->hit(_player->getBlaze()->getBullet()[i].atkPower,
					_player->getBlaze()->getBullet()[i].angle, 20.f);
				EFFECT->setEffect("flameStrike",
					{ (long)_player->getBlaze()->getBullet()[i].x
					,(long)_player->getBlaze()->getBullet()[i].y });
				_player->getBlaze()->removeBomb(i);
				break;

			}
		}
	}
	//meteor
	for (int i = 0; i < _player->getMeteor()->getMeteorVec().size(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getMeteor()->getMeteorVec()[i].rc, enemy->getMinion()[j]->getRC()))
			{
				enemy->getMinion()[j]->hit(_player->getMeteor()->getMeteorVec()[i].atkPower,
					_player->getMeteor()->getMeteorVec()[i].angle, 30.f);
				break;
			}
		}
	}
	//rush
	for (int i = 0; i < _player->getDashFire()->getSize(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (colCheck(_player->getDashFire()->getRect(i), enemy->getMinion()[j]->getRC()))
			{
				//col 되면 일정 간격으로 데미지 5번 

			}
		}

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