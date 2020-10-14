#include "stdafx.h"
#include "gameScene.h"

gameScene::gameScene() :
	enemy(nullptr), _player(nullptr), bgImg(nullptr), playerImg(nullptr), uiImg(nullptr), _shop(nullptr) {}

HRESULT gameScene::init()
{



	UI->init();
	DROP->init();

	_player = new player;
	_player->init();
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	//loadMap(0);

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);

	checkArea = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 100);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	PARTICLE->init();
	EFFECT->init();

	//vTile.clear();
	_wall = new wall;

	_wall->getRectAd(&cam);
	_wall->init();
	_wall->getPlayerAd(_player);
	_wall->collisionTile();
	//collisionTile();

	UI->setCoin(PLAYERDATA->getCoin());
	UI->setHp(PLAYERDATA->getHp());

	oneTime = false;
	////culPt
	//culPt = { 0,0 };
	//POINT ptZero = subTile[0].pos;
	//POINT ptCam = { 0,0 };

	//for (int i = 0; i < MAXTILE; i++)
	//{
	//	if (colCheck(cam, subTile[i].rc) == false)continue;
	//	ptCam = subTile[i].pos;
	//	break;
	//}

	//culPt = { ptCam.x - ptZero.x, ptCam.y - ptZero.y };

	enemy = new enemyManager;
	enemy->init(_wall->getTile(), _wall->getSubTile(), _wall->getCulPt());

	PLAYERDATA->setTile(_wall->getTile());

	_player->setTileAd(_wall->getTile());
	_player->setTileAd0(_wall->getVtile());

	EFFECT->setPortalEffect({ (long)_player->getX(),(long)_player->getY() });
	SOUNDMANAGER->play("portalWarp", false);

	//npc
	_shop = new shop;
	_shop->init();

	POINT ptArr[3] = { { -2040,-1509 },{ 2075,-35 },{ 1176,2134 } };
	_shop->generate(ptArr);

	//sound
	soundInit();
	return S_OK;
}

void gameScene::release()
{
	if (_player != nullptr)
	{
		_player->release();
		SAFE_DELETE(_player);

		enemy->release();
		SAFE_DELETE(enemy);

		_wall->release();
		SAFE_DELETE(_wall);
	}
}

void gameScene::update()
{
	_player->getBlaze()->setBossScene(false);

	//����
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
	//collisionTile();
	_wall->update();

	_player->setTileAd0(_wall->getVtile());

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

	_shop->shopCollider(_player->getRect());
	_shop->buyItem();

	if (INPUT->GetKeyDown('J'))
	{
		SCENEMANAGER->loadScene("������");
	}
}

void gameScene::render()
{
	_wall->render();
	//for (int i = 0; i < vTile.size(); i++)
	//{
	//	int num = vTile[i];

	//	if (tile[num].keyName == "" || tile[num].kind != TERRAIN::TILE)continue;

	//	image* img = IMAGEMANAGER->findImage(tile[num].keyName);

	//	CAMERAMANAGER->FrameRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top, tile[num].frame.x, tile[num].frame.y);
	//}

	////deco image render
	//for (int f = 0; f < vTile.size(); f++)
	//{
	//	int i = vTile[f];

	//	if (subTile[i].kind != TERRAIN::DECO) continue;

	//	image* img = IMAGEMANAGER->findImage(subTile[i].keyName);

	//	CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top);
	//}

	//CAMERAMANAGER->Rectangle(getMemDC(), checkArea);
	//Rectangle(getMemDC(), checkArea);
	//_player->render();
	_shop->render();

	DROP->render(getMemDC());
	EFFECT->pRender(getMemDC());

	bool* eRender = new bool[enemy->getMinion().size()];

	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		if (_player->getY() >= enemy->getMinion()[i]->getCenterY())
		{
			enemy->getMinion()[i]->render();
			eRender[i] = true;
		}
	}

	bool isRender = false;

	//CAMERAMANAGER->Rectangle(getMemDC(), cam);

	//string key;
	//image* img;
	//int width, height;
	//for (int f = 0; f < vTile.size(); f++)
	//{
	//	int i = vTile[f];

	//	if (tile[i].kind != TERRAIN::WALL && tile[i].kind != TERRAIN::OBJECT)continue;

	//	int w = MAXTILE_WIDTH;
	//	int w2 = MAXTILE_WIDTH * 2;
	//	int w3 = MAXTILE_WIDTH * 3;
	//	int w4 = MAXTILE_WIDTH * 4;
	//	int w5 = MAXTILE_WIDTH * 5;
	//	int w6 = MAXTILE_WIDTH * 6;

	//	switch (tile[i].kind)
	//	{
	//	case TERRAIN::WALL:
	//		key = tile[i].keyName;

	//		if (key == "topWall") // fix
	//		{
	//			if ((key == tile[i + 1].keyName &&
	//				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
	//				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
	//				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
	//				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&

	//				key == tile[i - w3 + 2].keyName && key == tile[i - w3 + 3].keyName &&
	//				key == tile[i - w3 + 4].keyName && key == tile[i - w3 + 5].keyName &&
	//				key == tile[i - w4 + 2].keyName && key == tile[i - w4 + 3].keyName &&
	//				key == tile[i - w4 + 4].keyName && key == tile[i - w4 + 5].keyName &&

	//				key == tile[i - w4 + 10].keyName && key == tile[i - w4 + 11].keyName &&
	//				key == tile[i - w4 + 12].keyName && key == tile[i - w4 + 13].keyName &&
	//				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&

	//				key == tile[i - w3 + 10].keyName && key == tile[i - w3 + 11].keyName &&
	//				key == tile[i - w3 + 12].keyName && key == tile[i - w3 + 13].keyName &&
	//				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&

	//				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
	//				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
	//				key == tile[i + 14].keyName && key == tile[i + 15].keyName)
	//				== false)
	//			{
	//				continue;
	//			}

	//			height = 8 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);

	//		}

	//		else if (key == "bottomWall") // fix
	//		{
	//			if ((key == tile[i + 1].keyName &&
	//				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
	//				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
	//				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
	//				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&
	//				key == tile[i - w5].keyName && key == tile[i - w5 + 1].keyName &&
	//				key == tile[i - w6].keyName && key == tile[i - w6 + 1].keyName &&

	//				key == tile[i - w + 2].keyName && key == tile[i - w + 3].keyName &&
	//				key == tile[i - w + 4].keyName &&
	//				key == tile[i + 2].keyName && key == tile[i + 3].keyName &&
	//				key == tile[i + 4].keyName &&


	//				key == tile[i - w + 11].keyName && key == tile[i - w + 12].keyName &&
	//				key == tile[i - w + 13].keyName &&
	//				key == tile[i + 11].keyName && key == tile[i + 12].keyName &&
	//				key == tile[i + 13].keyName &&

	//				key == tile[i + 14].keyName && key == tile[i + 15].keyName &&
	//				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
	//				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
	//				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&
	//				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&
	//				key == tile[i - w5 + 14].keyName && key == tile[i - w5 + 15].keyName &&
	//				key == tile[i - w6 + 14].keyName && key == tile[i - w6 + 15].keyName
	//				) == false)
	//			{
	//				continue;
	//			}
	//			height = 8 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
	//		}

	//		else if (key == "wallFrame0" || key == "wallFrame1" || key == "wallFrame2" || key == "wallFrame3")
	//		{
	//			image* img = IMAGEMANAGER->findImage(tile[i].keyName);

	//			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	//		}

	//		else
	//		{
	//			height = 3 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, tile[i].frame.x, tile[i].frame.y);
	//		}

	//		break;
	//	case TERRAIN::OBJECT:
	//		// 2X2�� ������Ʈ�� ó��

	//		//i object, i+1 object, i+w object, i+w+1 object
	//		if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
	//			tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

	//		key = tile[i].keyName;

	//		if (!(tile[i + 1].keyName == key &&
	//			tile[i + MAXTILE_WIDTH].keyName == key &&
	//			tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

	//		if (tile[i].keyName == "flowerbed1")
	//		{
	//			key = "tree0";
	//			width = 2 * TILESIZE;
	//			height = 4 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
	//		}
	//		else if (tile[i].keyName == "flowerbed2")
	//		{
	//			key = "tree1";
	//			width = 2 * TILESIZE;
	//			height = 5 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
	//		}
	//		else if (tile[i].keyName == "pillar1")
	//		{
	//			key = "pillar1";

	//			height = 3 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
	//		}
	//		break;
	//	}
	//}

	enemy->render();

	_player->render();

	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		if (eRender[i] == true)continue;

		enemy->getMinion()[i]->render();
	}

	PLAYERDATA->shroudRender(getMemDC());

	//PARTICLE->render(getMemDC(), CAMERAMANAGER->getRect());
	PARTICLE->render(getMemDC());
	EFFECT->render(getMemDC());
	EFFECT->dRender(getMemDC());

	DAMAGE->render(getMemDC());

	EFFECT->portalRender(getMemDC());

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	//uiImg->render(getMemDC());
	

	UI->render(getMemDC(), 50, 50);
	_player->invenRender();

	viewText();

}

void gameScene::collisionTile()
{
	//vTile.clear();
	//vWall.clear();
	//for (int i = 0; i < MAXTILE; i++)
	//{
	//	if (colCheck(cam, tile[i].rc))
	//	{
	//		vTile.push_back(i);

	//		if (tile[i].kind == TERRAIN::WALL)vWall.push_back(i);
	//	}
	//}

	//_player->setTileAd0(vTile);
	//PLAYERDATA->setTile(vTile);
	//_player->setTileAd1(vWall);
	//PLAYERDATA->setWall(vWall);
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
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getBlaze()->getBullet()[i].atkPower + RANDOM->range(0, 3), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage,0);
				}


				SOUNDMANAGER->play("blazeExp", false);
				PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
					_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);
				enemy->getMinion()[j]->hit(damage, _player->getBlaze()->getBullet()[i].angle, 20.f, 0, criCheck);

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
				//ũ��Ƽ��?
				bool criCheck = PLAYERDATA->criAppear();

				//������ ���
				int damage = PLAYERDATA->damageCul(_player->getMeteor()->getAtkPower(i) + RANDOM->range(0, 5), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, _player->getMeteor()->getSkillNum());
				}

				enemy->getMinion()[j]->hit(damage, _player->getMeteor()->getAngle(i),
					30.f, _player->getMeteor()->getSkillNum(), criCheck);

				break;
			}
		}
	}
	//rush

	for (int i = 0; i < _player->getDashFire()->getSize(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getDashFire()->getRect(i), enemy->getMinion()[j]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getDashFire()->getAtk(i) + RANDOM->range(0, 5), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, _player->getDashFire()->getSkillNum());
				}

				enemy->getMinion()[j]->hit(damage, 70.f, 20.f, _player->getDashFire()->getSkillNum(), criCheck);
			}
		}
	}


	//inferno

	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		long enemyX = enemy->getMinion()[i]->getPos().x;
		long enemyY = enemy->getMinion()[i]->getPos().y;

		if (0 >= enemy->getMinion()[i]->getHp())continue;
		if (colCheck(_player->getInferno()->getInf().rc, enemy->getMinion()[i]->getRC()))
		{
			//����¡ + ���� ���°� �ƴ� �� inferno�� ���� �浹 üũ
			if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(enemy->getMinion()[i]->getRC()))
			{
				//�浹�Ǹ� �� �ڸ����� ����
				if (PLAYERDATA->getGaugeTime() < 50)
				{
					if (SOUNDMANAGER->isPlaySound("RagingInfernoExp") == false)	SOUNDMANAGER->play("RagingInfernoExp", false);

					PLAYERDATA->setGaugeTime(50);
					_player->getInferno()->setActive(true);
				}
			}

			else if (PLAYERDATA->getGaugeTime() >= 50)
			{
				bool criCheck = PLAYERDATA->criAppear();
			
				int damage = PLAYERDATA->damageCul(_player->getInferno()->getInf().atkPower, criCheck);
				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage,3);
				}

				float angle = getAngle(enemyX + 40, enemyY + 40,
					_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

				float x = enemyX + cosf(angle) * 4.5f;
				float y = enemyY - sinf(angle) * 4.5f;

				enemy->getMinion()[i]->setPt(x, y);
				enemy->getMinion()[i]->hit(damage, 0, 0.f, 3, criCheck);
			}
		}
	}
}

void gameScene::enemyAttack()
{
	if (PLAYERDATA->getHp() <= 0)return;

	for (int i = 0; i < enemy->getBullet()->getBullet().size(); )
	{
		if (colCheck(enemy->getBullet()->getRect(i), _player->getRect()))
		{
			if (PLAYERDATA->getShroud() == false)
			{
				_player->damage(enemy->getBullet()->getBullet()[i].atkPower, enemy->getBullet()->getBullet()[i].angle, 4.f);
			}
			else PLAYERDATA->setShroud(false);

			if (enemy->getBullet()->getBullet()[i].kind == MONSTERKIND::SUMMONER)
			{
				PARTICLE->crashRockParticlePlay(_player->getX(), _player->getY());
				SOUNDMANAGER->play("summonerAtk", false, -0.18f);
			}
			enemy->getBullet()->removeBullet(i);
			SOUNDMANAGER->play("playerHit", false, -0.18f);
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
	ReadFile(file, _wall->getTile(), sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);

	file = CreateFile(str1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _wall->getSubTile(), sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void gameScene::viewText()
{
	//
}

void gameScene::soundInit()
{
	//sound 
	SOUNDMANAGER->stop("mapToolBGM");
	SOUNDMANAGER->stop("ingameBGM");
	SOUNDMANAGER->stop("bossBGM");
	SOUNDMANAGER->play("ingameBGM", true);

	isIngameBGM = true;
	fadeIn = 0.f;

	//����
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

	SOUNDMANAGER->addSound("summonerCasting", "Sound/summoner_casting.mp3");
	SOUNDMANAGER->addSound("summonerFire", "Sound/summoner_fire.mp3");
	SOUNDMANAGER->addSound("summonerAtk", "Sound/summoner_atk.mp3");



}
