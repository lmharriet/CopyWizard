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

	loadMap(0);

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);

	checkArea = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 100);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	PARTICLE->init();
	EFFECT->init();
	//cout << "x : " <<_player->getX()<< endl;
	//cout << "y : " <<_player->getY()<< endl;

	//vTile.clear();
	collisionTile();

	UI->setCoin(PLAYERDATA->getCoin());
	UI->setHp(PLAYERDATA->getHp());

	oneTime = false;

	//culPt
	culPt = { 0,0 };
	POINT ptZero = subTile[0].pos;
	POINT ptCam = { 0,0 };

	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, subTile[i].rc) == false)continue;
		ptCam = subTile[i].pos;
		break;
	}

	culPt = { ptCam.x - ptZero.x, ptCam.y - ptZero.y };

	enemy = new enemyManager;
	enemy->init(tile, subTile, culPt);



	
	//벽 타입만 저장
	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].kind == TERRAIN::WALL)
		{
			vWall.push_back(i);
		}
	}

	PLAYERDATA->setTile(tile);
	PLAYERDATA->setWall(vWall);
	
	_player->setTileAd(tile);
	_player->setTileAd0(vTile);
	_player->setTileAd1(vWall);


	//sound
	soundInit();
	return S_OK;
}

void gameScene::release()
{
	/*_player->release();
	SAFE_DELETE(_player);

	enemy->release();
	SAFE_DELETE(enemy);*/
}

void gameScene::update()
{
	//사운드
	if (isIngameBGM)
	{
		SOUNDMANAGER->fadeIn("ingameBGM", fadeIn);
		fadeIn += 0.002f;
		if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
			isIngameBGM = false;
	}
	UI->setCoin(PLAYERDATA->getCoin());
	UI->setHp(PLAYERDATA->getHp());
	PLAYERDATA->setX(_player->getX());
	PLAYERDATA->setY(_player->getY());

	atkCount++;
	collisionTile();
	_player->setTileAd0(vTile);

	UI->update();
	DROP->update();

	_player->update();

	enemy->setPlayerRC(RectMake(_player->getX(), _player->getY(), 100, 100));
	enemy->update();


	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);
	//checkArea = RectMakeCenter(_player->getX(), _player->getY(), 400, 400);
	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY / 2 + 420, 200, 500);

	CAMERAMANAGER->update();
	PARTICLE->pointActive();
	PARTICLE->explosionActive();

	//enemy->minionBulletFire(_player->getX(), _player->getY());
	//enemy->collision(_player->getRect());

	//collision between player skill and enemy
	playerAttack();
	enemyAttack();

}

void gameScene::render()
{
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

	bool enemyProRender = false;
	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		if (_player->getY() >= enemy->getMinion()[i]->getCenterY())
		{
			enemy->getMinion()[i]->render();
		}
	}

	bool isRender = false;

	//CAMERAMANAGER->Rectangle(getMemDC(), cam);

	//object image render
	for (int f = 0; f < vTile.size(); f++)
	{
		int i = vTile[f];

		if (!colCheck(tile[i].rc, cam) || (subTile[i].kind != TERRAIN::DECO)) continue;

		image* img = IMAGEMANAGER->findImage(subTile[i].keyName);

		CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top);
	}

	string key;
	image* img;
	int width, height;
	for (int f = 0; f < vTile.size(); f++)
	{
		int i = vTile[f];

		if (!colCheck(tile[i].rc, cam) ||
			(tile[i].kind != TERRAIN::WALL && tile[i].kind != TERRAIN::OBJECT))continue;

		if (!isRender &&
			(colCheck(checkArea, tile[i].rc) ||
				colCheck(checkArea, tile[i + 15].rc) ||
				colCheck(checkArea, tile[i - MAXTILE_WIDTH * 4 + 10].rc) ||
				colCheck(checkArea, tile[i - MAXTILE_WIDTH * 4 + 5].rc))
			)
		{
			isRender = true;
			_player->render();
		}

		int w = MAXTILE_WIDTH;
		int w2 = MAXTILE_WIDTH * 2;
		int w3 = MAXTILE_WIDTH * 3;
		int w4 = MAXTILE_WIDTH * 4;
		int w5 = MAXTILE_WIDTH * 5;
		int w6 = MAXTILE_WIDTH * 6;

		switch (tile[i].kind)
		{
		case TERRAIN::WALL:
			key = tile[i].keyName;

			if (key == "topWall") // fix
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

				img = IMAGEMANAGER->findImage(key);

				//CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
				
				if (tile[i].rc.bottom > (_player->getY() + 30) &&
				((colCheck(checkArea, tile[i].rc) || colCheck(checkArea, tile[i + 15].rc) ||
				colCheck(checkArea, tile[i - w4 + 10].rc) || colCheck(checkArea, tile[i - w4 + 5].rc))))
				{
					CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, 150);
				}
				else CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
				
			}

			else if (key == "bottomWall") // fix
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

				img = IMAGEMANAGER->findImage(key);

				//CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
				if (tile[i].rc.bottom > (_player->getY() + 30) &&
					(colCheck(checkArea, tile[i].rc) || colCheck(checkArea, tile[i + 15].rc)))
				{
					CAMERAMANAGER->AlphaRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, 150);
				}
				else CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
			}

			else if (key == "wallFrame0" || key == "wallFrame1" || key == "wallFrame2")
			{
				float scale = (float)(tile[i].rc.right - tile[i].rc.left) / TILESIZE;

				image* img = IMAGEMANAGER->findImage(tile[i].keyName);

				//img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
				
				CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
				//img->frameRender(getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y, scale);
			}

			else if (key == "shopWall0" || key == "shopWall1")
			{

				height = 5 * TILESIZE;

				if (key == "shopWall0")
				{
					if ((key == tile[i + 1].keyName && key == tile[i - w].keyName && key == tile[i - w2].keyName &&
						key == tile[i - w3].keyName && key == tile[i - w4].keyName && key == tile[i - w5].keyName) == false)
					{
						continue;
					}

					img = IMAGEMANAGER->findImage(key);

					CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
				}

				if (key == "shopWall1")
				{
					//예외 처리
					if ((key == tile[i + 1].keyName && key == tile[i + 1 - w].keyName && key == tile[i + 1 - w2].keyName &&
						key == tile[i + 1 - w3].keyName && key == tile[i + 1 - w4].keyName && key == tile[i + 1 - w5].keyName) == false)
					{
						continue;
					}


					img = IMAGEMANAGER->findImage(key);

					CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
				}
			}

			else if (key == "shopWall2" || key == "shopWall3")
			{
				if ((key == tile[i + 1].keyName &&
					key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
					key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName) == false)
				{
					continue;
				}
				height = 2 * TILESIZE;

				img = IMAGEMANAGER->findImage(key);

				CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
			}

			else
			{
				height = 3 * TILESIZE;

				img = IMAGEMANAGER->findImage(key);

				CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, tile[i].frame.x, tile[i].frame.y);
			}

			break;
		case TERRAIN::OBJECT:
			// 2X2인 오브젝트만 처리

			//i object, i+1 object, i+w object, i+w+1 object
			if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
				tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

			key = tile[i].keyName;

			if (!(tile[i + 1].keyName == key &&
				tile[i + MAXTILE_WIDTH].keyName == key &&
				tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

			if (tile[i].keyName == "flowerbed1")
			{
				key = "tree0";
				width = 2 * TILESIZE;
				height = 4 * TILESIZE;

				img = IMAGEMANAGER->findImage(key);

				CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
			}
			else if (tile[i].keyName == "flowerbed2")
			{
				key = "tree1";
				width = 2 * TILESIZE;
				height = 5 * TILESIZE;

				img = IMAGEMANAGER->findImage(key);

				CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
			}
			else if (tile[i].keyName == "pillar1")
			{
				key = "pillar1";

				height = 3 * TILESIZE;

				img = IMAGEMANAGER->findImage(key);

				CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
			}
			break;
		}
	}

	for (int f = 0; f < vTile.size(); f++)
	{
		int i = vTile[f];

		if (tile[i].kind != TERRAIN::OBJECT) continue;

		// 1x? 이거나 다른 특수한 오브젝트 처리

		string key;
		int height;
		if (tile[i].keyName == "pillar0") // 가로 : 1
		{
			key = "pillar0";

			height = 3 * TILESIZE;
			image* img = IMAGEMANAGER->findImage(key);

			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
		}
		else if (tile[i].keyName == "bossDoor")
		{
			if (tile[i + 5].keyName != "bossDoor")continue;

			key = "bossDoor";

			height = 7 * TILESIZE;
			image* img = IMAGEMANAGER->findImage(key);

			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
		}
	}


	enemy->render();

	if (!isRender) _player->render();
	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		if (_player->getY() < enemy->getMinion()[i]->getCenterY())
		{
			enemy->getMinion()[i]->render();
		}
	}

	//PARTICLE->render(getMemDC(), CAMERAMANAGER->getRect());
	PARTICLE->render(getMemDC());
	EFFECT->render(getMemDC());
	EFFECT->dRender(getMemDC());

	DAMAGE->render(getMemDC());

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	//uiImg->render(getMemDC());


	_player->invenRender();
	UI->render(getMemDC(), 50, 50);

	viewText();





}

void gameScene::collisionTile()
{
	vTile.clear();
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc)) vTile.push_back(i);
	}

	_player->setTileAd0(vTile);
	PLAYERDATA->setTile(vTile);
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

				PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
					_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);
				enemy->getMinion()[j]->hit(_player->getBlaze()->getBullet()[i].atkPower,
					_player->getBlaze()->getBullet()[i].angle, 20.f, 0);

				/*EFFECT->setEffect("flameStrike",
					{ (long)_player->getBlaze()->getBullet()[i].x
					,(long)_player->getBlaze()->getBullet()[i].y });*/

				_player->getBlaze()->setCol(i, true);

				break;

			}
		}
	}

	//meteor
	for (int i = 0; i < _player->getMeteor()->getColSize(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getMeteor()->getColRect(i), enemy->getMinion()[j]->getRC()))
			{
				enemy->getMinion()[j]->hit(_player->getMeteor()->getAtkPower(i),
					_player->getMeteor()->getAngle(i), 30.f, _player->getMeteor()->getSkillNum());

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
				if (0 >= enemy->getMinion()[j]->getHp())continue;

				enemy->getMinion()[j]->hit(_player->getDashFire()->getAtk(i),
					70.f, 20.f, _player->getDashFire()->getSkillNum());

			}
		}
	}


	//inferno

	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		long enemyX = enemy->getMinion()[i]->getPos().x;
		long enemyY = enemy->getMinion()[i]->getPos().y;

		if (colCheck(_player->getInferno()->getInf().rc, enemy->getMinion()[i]->getRC()))
		{
			//게이징 + 무브 상태가 아닐 때 inferno와 몬스터 충돌 체크
			if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(enemy->getMinion()[i]->getRC()))
			{
				//충돌되면 그 자리에서 공격
				if (PLAYERDATA->getGaugeTime() < 50)
				{
					PLAYERDATA->setGaugeTime(50);
					_player->getInferno()->setActive(true);
				}
			}

			else if (PLAYERDATA->getGaugeTime() >= 50)
			{
				float angle = getAngle(enemyX + 40, enemyY + 40,
					_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

				float x = enemyX + cosf(angle) * 4.5f;
				float y = enemyY - sinf(angle) * 4.5f;

				enemy->getMinion()[i]->setPt(x, y);
				enemy->getMinion()[i]->hit(_player->getInferno()->getInf().atkPower, 0, 0.f, 3);
			}
		}
	}
}

void gameScene::enemyAttack()
{
	for (int i = 0; i < enemy->getBullet()->getBullet().size(); )
	{
		if (colCheck(enemy->getBullet()->getRect(i), _player->getRect()))
		{
			_player->damage(enemy->getBullet()->getBullet()[i].atkPower, enemy->getBullet()->getBullet()[i].angle);
			if (enemy->getBullet()->getBullet()[i].kind == MONSTERKIND::SUMMONER)
			{
				PARTICLE->crashRockParticlePlay(_player->getX(), _player->getY());
			}
			enemy->getBullet()->removeBullet(i);
			SOUNDMANAGER->play("playerHit",false);
		}
		else
		{
			i++;
		}
	}
}

void gameScene::loadMap(int index)
{
	char str[50], str1[50];

	sprintf(str, "mapData/map%d.map", index);
	sprintf(str1, "mapData/object%d.map", index);

	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);

	file = CreateFile(str1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, subTile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void gameScene::viewText()
{
	//
}

void gameScene::soundInit()
{
	isIngameBGM = true;
	fadeIn = 0.f;

	//몬스터
	SOUNDMANAGER->addSound("golemAtk", "Sound/golem_atk.mp3");
	SOUNDMANAGER->addSound("golemWalkL", "Sound/golem_walkL.mp3");
	SOUNDMANAGER->addSound("golemWalkR", "Sound/golem_walkR.mp3");
	SOUNDMANAGER->addSound("golemDie", "Sound/golem_die.mp3");
	SOUNDMANAGER->addSound("golemHit", "Sound/golem_hit.mp3");
	SOUNDMANAGER->addSound("knightAtk0", "Sound/knight_atk0.mp3");
	SOUNDMANAGER->addSound("knightAtk1", "Sound/knight_atk1.mp3");
	SOUNDMANAGER->addSound("knightAtk2", "Sound/knight_atk2.mp3");
	SOUNDMANAGER->addSound("knightAtk3", "Sound/knight_atk3.mp3");
	SOUNDMANAGER->addSound("knightWalkL", "Sound/knight_walkL.mp3");
	SOUNDMANAGER->addSound("knightWalkR", "Sound/knignt_walkR.mp3");
	SOUNDMANAGER->addSound("knightDie", "Sound/knight_die.mp3");
	SOUNDMANAGER->addSound("knightHit0", "Sound/knight_hit0.mp3");
	SOUNDMANAGER->addSound("knightHit1", "Sound/knight_hit1.mp3");

	
}
