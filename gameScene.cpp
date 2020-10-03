#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{
	UI->init();
	
	_player = new player;
	_player->init();
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	loadMap("mapData/map0.map");
	_player->setTileAd(tile);
	
	
	cam = RectMakeCenter(0, 0, WINSIZEX, WINSIZEY);
	checkArea = RectMakeCenter(WINSIZEX/2, WINSIZEY/2, 100, 100);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	PARTICLE->init();
	EFFECT->init();

	enemy = new enemyManager;
	enemy->init(tile);

	//_golem = new golem; //테스트중 (몬스터골렘)
	//_golem->init(tile, "golem",{ 820,320 },3.f); //테스트중 (몬스터골렘)


	//_golem2 = new golem; //테스트중 (몬스터골렘2)
	//_golem2->init(tile, "golem", { 220,320 }, 3.f); //테스트중 (몬스터골렘2)

	

	return S_OK;
}

void gameScene::release()
{
	_player->release();
	SAFE_DELETE(_player);

	/*_golem->release();
	SAFE_DELETE(_golem);

	_golem2->release();
	SAFE_DELETE(_golem2);*/
	enemy->release();
	SAFE_DELETE(enemy);
}

void gameScene::update()
{
	UI->update();
	_player->update();
	/*_golem->update();
	_golem2->update()*/;
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
			if (colCheck(_player->getBlaze()->getBullet()[i].rc, enemy->getMinion()[j]->getRC()))
			{
				_player->getBlaze()->removeBomb(i);
				//enemy->removeMinion(j);
				enemy->getMinion()[j]->setHp(0);
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
	
	EFFECT->render(getMemDC());

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

	PARTICLE->render(getMemDC(), CAMERAMANAGER->getRect());

	enemy->render();
	if(!isRender) _player->render();

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	//uiImg->render(getMemDC());


	
	UI->render(getMemDC(), 50, 50);

	viewText();
	
	//_golem->render(); //테스트중 (몬스터골렘)
	//_golem2->render(); //테스트중 (몬스터골렘)

	
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