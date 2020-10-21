#include "stdafx.h"
#include "gameScene.h"

gameScene::gameScene() :
	enemy(nullptr), _player(nullptr), bgImg(nullptr), playerImg(nullptr), uiImg(nullptr), _shop(nullptr), _wall(nullptr) {}

HRESULT gameScene::init()
{
	UI->init();
	DROP->init();

	if (_player == nullptr)
		_player = new player;

	_player->init();
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);

	checkArea = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 100);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	PARTICLE->init();
	EFFECT->init();
	if (_wall == nullptr)
		_wall = new wall;

	_wall->getRectAd(&cam);
	_wall->init();
	_wall->getPlayerAd(_player);
	_wall->collisionTile();

	UI->setCoin(PLAYERDATA->getCoin());
	UI->setHp(PLAYERDATA->getHp());


	enemy = make_shared<enemyManager>();
	enemy->init(_wall->getTile(), _wall->getSubTile(), _wall->getCulPt());

	PLAYERDATA->setTile(_wall->getTile());

	_player->setTileAd(_wall->getTile());
	_player->setTileAd0(_wall->getVtile());

	EFFECT->setPortalEffect({ (long)_player->getX(),(long)_player->getY() });
	SOUNDMANAGER->play("portalWarp", false);

	if (_shop == nullptr)
		_shop = new shop;
	_shop->init();


	UNITRENDER->addUnit(0, "playerFrame", "player", { 0,0 });

	UNITRENDER->init();

	tagPortal portal;
	portal.colorIndex = 0;
	portal.curPt = { (long)_player->getX(),(long)_player->getY() };
	portal.endPt = { 0,0 };
	portal.isActive = false;
	portal.isCol = false;
	portal.rc = RectMakeCenter(portal.curPt.x, portal.curPt.y, 30, 60);
	PORTAL->setCenterPortal(portal);

	_chest = new chest;
	_chest->init("silverChest", { -1149,2047 }, 10);


	warpDelay = 0;
	isWarp = false;
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

		//_wall->release();
		//SAFE_DELETE(_wall);
	}
}

void gameScene::update()
{
	enemyCheckCol();
	//portal
	warp();

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

	_wall->update();

	_player->setTileAd0(_wall->getVtile());

	UI->update();
	DROP->update();

	_player->update();
	_player->animation();


	enemy->setPlayerRC(RectMake(_player->getX(), _player->getY(), 100, 100));
	enemy->update();
	_chest->update();

	UNITRENDER->enemyClear();
	enemyUnitRenderInit(); // ������ UNITERENDER->enemyClear�� UNITRENDER->update() ���̿� �־����.
	UNITRENDER->update();

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);

	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY / 2 + 420, 200, 500);

	CAMERAMANAGER->update();
	PARTICLE->pointActive();
	PARTICLE->explosionActive();

	//collision between player skill and enemy

	playerAttack();
	enemyAttack();
	attackChest();

	gainArcana();

	_shop->shopCollider(_player->getRect());
	_shop->buyItem();

	if (INPUT->GetKeyDown('J'))
	{
		SOUNDMANAGER->stop("ingameBGM");
		PLAYERDATA->setCurrentStage(STAGEKIND::MIDDLESTAGE);
		SCENEMANAGER->loadScene("�ε�ȭ��");
	}

	if (INPUT->GetKeyDown(VK_BACK))
	{
		//POINT pt = { (long)_player->getX() ,(long)_player->getY() };

		//DROP->getCoinEffect(1);
		//EFFECT->setEmotionEffect("buyEmote_Nox", { (long)_player->getX() ,(long)_player->getY() - 80 });
		//EFFECT->setAlwaysPoint("curseEffect", pt);
		//EFFECT->setEffect("portal1", pt, false, false, 0, 3);
		//EFFECT->setBackEffect("portal1", pt, 3);
	}


	UNITRENDER->setPlayerRect(_player->getRect());

	EFFECT->alwaysActive();
}

void gameScene::render()
{
	_wall->render2();
	//_wall->render();

	_shop->render();

	EFFECT->pRender(getMemDC());

	EFFECT->alwaysEftRender(getMemDC());
	EFFECT->AfterimageRender(getMemDC());

	EFFECT->backEftRender(getMemDC());

	PORTAL->backRender(getMemDC());

	DROP->cardRender(getMemDC());

	UNITRENDER->render(getMemDC());

	DROP->render(getMemDC());

	PORTAL->render(getMemDC());

	_player->render();

	EFFECT->emotionRender(getMemDC());


	PLAYERDATA->shroudRender(getMemDC());

	PARTICLE->render(getMemDC());
	EFFECT->render(getMemDC());
	EFFECT->dRender(getMemDC());
	enemy->render();

	DAMAGE->render(getMemDC());

	EFFECT->portalRender(getMemDC());

	if (PLAYERDATA->getStat().reducedVisibility)
	{
		IMAGEMANAGER->findImage("glassEffect")->render(getMemDC());
	}

	DROP->coinEffectRender(getMemDC());

	_shop->colRender();

	UI->FbuttonRender(getMemDC());

	UI->render(getMemDC(), 50, 50);
	_player->invenRender();

	viewText();

}

void gameScene::enemyCheckCol()
{
	camInEnemy.clear();
	camInEnemy.shrink_to_fit();

	int size = enemy->getMinion().size();
	for (int i = 0; i < size; i++)
	{
		if (colCheck(cam, enemy->getMinion()[i]->getRC()))
		{
			camInEnemy.push_back(i);
		}
	}
}

void gameScene::checkEnemDie()
{
	int size = camInEnemy.size();
	for (int i = 0; i < size;)
	{
		int num = camInEnemy[i];

		if (enemy->getMinion().size() <= num || enemy->getMinion()[num]->getHp() <= 0)
		{
			//cout << enemy->getMinion().size() << " / " << num << '\n';
			camInEnemy.erase(camInEnemy.begin() + i);
			size = camInEnemy.size();
		}
		else i++;
	}
}

void gameScene::playerAttack()
{
	int sizeA = _player->getBlaze()->getBullet().size();
	checkEnemDie();
	int sizeB = camInEnemy.size();
	int num = 0;
	//blaze
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			num = camInEnemy[j];

			if (0 >= enemy->getMinion()[num]->getHp())continue;
			if (colCheck(_player->getBlaze()->getBullet()[i].rc, enemy->getMinion()[num]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getBlaze()->getBullet()[i].atkPower + RANDOM->range(0, 3), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 0);
				}


				SOUNDMANAGER->play("blazeExp", false);
				PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
					_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);
				enemy->getMinion()[num]->hit(damage, _player->getBlaze()->getBullet()[i].angle, 20.f, 0, criCheck);

				_player->getBlaze()->setCol(i, true);

				break;
			}
		}
	}//end of for

	//meteor
	sizeA = _player->getMeteor()->getColSize();
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			num = camInEnemy[j];

			if (0 >= enemy->getMinion()[num]->getHp())continue;
			if (colCheck(_player->getMeteor()->getColRect(i), enemy->getMinion()[num]->getRC()))
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

				enemy->getMinion()[num]->hit(damage, _player->getMeteor()->getAngle(i),
					30.f, _player->getMeteor()->getSkillNum(), criCheck);

				break;
			}
		}
	}//end of for

	//rush
	sizeA = _player->getDashFire()->getSize();
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			int num = camInEnemy[j];

			if (0 >= enemy->getMinion()[num]->getHp())continue;
			if (colCheck(_player->getDashFire()->getRect(i), enemy->getMinion()[num]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getDashFire()->getAtk(i) + RANDOM->range(0, 5), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, _player->getDashFire()->getSkillNum());
				}

				enemy->getMinion()[num]->hit(damage, 70.f, 20.f, _player->getDashFire()->getSkillNum(), criCheck);
			}
		}
	}//end of for

	//inferno
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeB; i++)
	{
		num = camInEnemy[i];

		long enemyX = enemy->getMinion()[num]->getPos().x;
		long enemyY = enemy->getMinion()[num]->getPos().y;

		if (0 >= enemy->getMinion()[num]->getHp())continue;
		if (colCheck(_player->getInferno()->getInf().rc, enemy->getMinion()[num]->getRC()))
		{
			//����¡ + ���� ���°� �ƴ� �� inferno�� ���� �浹 üũ
			if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(enemy->getMinion()[num]->getRC()))
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
					_player->chargeSkillGauge(damage, 3);
				}

				if (enemy->getMinion()[num]->getMonsterKind() != MONSTERKIND::GOLEM && enemy->getMinion()[num]->getMonsterKind() != MONSTERKIND::GHOULLARGE)
				{
					float angle = getAngle(enemyX + 40, enemyY + 40,
						_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

					float x = enemyX + cosf(angle) * 4.5f;
					float y = enemyY - sinf(angle) * 4.5f;

					enemy->getMinion()[num]->setPt(x, y);
				}
				enemy->getMinion()[num]->hit(damage, 0, 0.f, 3, criCheck);
			}
		}
	}//end of for

	//dragonArc
	if (!_player->getDragon()->getUpgrade())
	{
		sizeA = _player->getDragon()->getSize();
		checkEnemDie();
		sizeB = camInEnemy.size();
		for (int i = 0; i < sizeA; i++)
		{
			for (int j = 0; j < sizeB; j++)
			{
				num = camInEnemy[j];

				if (colCheck(_player->getDragon()->getDragonRC(i), enemy->getMinion()[num]->getRC()))
				{
					bool criCheck = PLAYERDATA->criAppear();

					int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3), criCheck);

					//gauge
					if (PLAYERDATA->getStat().ManaRejection == false)
					{
						_player->chargeSkillGauge(damage, 4);
					}
					enemy->getMinion()[num]->hit(damage, _player->getDragon()->getDragonAngle(i), 10.f, 4, criCheck);
				}
			}
		}
	}

	//upgrade
	sizeA = _player->getDragon()->getcolSize();
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			num = camInEnemy[j];

			if (colCheck(_player->getDragon()->getColRc(i), enemy->getMinion()[num]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 4);
				}
				enemy->getMinion()[num]->hit(damage, _player->getDragon()->getHeadAngle(i), 10.f, 4, criCheck);
			}

		}
	}

	//ice Spear
	sizeA = _player->getSpear()->getSize();
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			num = camInEnemy[j];

			if (colCheck(_player->getSpear()->getSpearRc(i), enemy->getMinion()[num]->getRC()))
			{
				//enemy->getMinion()[j]->setPt(_player->getSpear()->getSpearRc(i).left, _player->getSpear()->getSpearRc(i).top);

				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getSpear()->getAtkPower(i) + RANDOM->range(0, 3), criCheck);
				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 5);
				}
				enemy->getMinion()[num]->hit(damage, _player->getSpear()->getSpearAngle(i), 20.f, 5, criCheck, true);

				_player->getSpear()->setCol(i, true);
			}
		}
	}

	//upgrade
	sizeA = _player->getSpear()->getUpgradeSize();
	checkEnemDie();
	sizeB = camInEnemy.size();
	for (int i = 0; i < sizeA; i++)
	{
		for (int j = 0; j < sizeB; j++)
		{
			num = camInEnemy[j];

			if (colCheck(_player->getSpear()->getUpgradeRC(i), enemy->getMinion()[num]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getSpear()->getUpgradeAtk(i) + RANDOM->range(0, 3), criCheck);
				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 5);
				}
				enemy->getMinion()[num]->hit(damage, _player->getSpear()->getUpgradeAngle(i), 30.f, 5, criCheck, true);

				_player->getSpear()->setUpgradeCol(i, true);
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
				int damage = enemy->getBullet()->getBullet()[i].atkPower;

				if (PLAYERDATA->getStat().doubleDamage)
				{
					_player->damage(damage * 2, enemy->getBullet()->getBullet()[i].angle, 4.f);
				}

				else
				{
					_player->damage(damage, enemy->getBullet()->getBullet()[i].angle, 4.f);
				}
			}
			else { PLAYERDATA->setShroud(false); SOUNDMANAGER->play("shieldOFF", false, 1.0f); }

			if (enemy->getBullet()->getBullet()[i].kind == MONSTERKIND::SUMMONER)
			{
				PARTICLE->crashRockParticlePlay(_player->getX(), _player->getY());
				SOUNDMANAGER->play("summonerAtk", false, -0.18f);
			}
			enemy->getBullet()->removeBullet(i);
		}
		else
		{
			i++;
		}
	}
}

void gameScene::enemyUnitRenderInit()
{
	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		if (!enemy->getMinion()[i]->getAppear()) continue;
		POINT currentFrame = enemy->getMinion()[i]->getCurrentFrame();
		POINT monsterPos = enemy->getMinion()[i]->getPos();
		switch (enemy->getMinion()[i]->getMonsterKind())
		{
		case MONSTERKIND::GOLEM:
			if (enemy->getMinion()[i]->getHit() && !enemy->getMinion()[i]->getDie())
				UNITRENDER->enemyInit(4, currentFrame, monsterPos);//����Ʈ��
			else
				UNITRENDER->enemyInit(0, currentFrame, monsterPos);
			break;
		case MONSTERKIND::KNIGHT:
			UNITRENDER->enemyInit(1, currentFrame, monsterPos);
			break;
		case MONSTERKIND::SUMMONER:
			UNITRENDER->enemyInit(2, currentFrame, monsterPos);
			break;
		case MONSTERKIND::GHOUL:
			UNITRENDER->enemyInit(3, currentFrame, monsterPos);
			break;
		case MONSTERKIND::GHOULLARGE:
			if (enemy->getMinion()[i]->getHit() && !enemy->getMinion()[i]->getDie())
				UNITRENDER->enemyInit(6, currentFrame, monsterPos); // ghoulLarge hit
			else
				UNITRENDER->enemyInit(5, currentFrame, monsterPos);
			break;
		}

	}
}

void gameScene::attackChest()

{
	if (_chest->getHp() <= 0)return;
	//blaze
	for (int i = 0; i < _player->getBlaze()->getSize(); i++)
	{

		if (colCheck(_player->getBlaze()->getBullet()[i].rc, _chest->getRc()))
		{
			bool criCheck = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getBlaze()->getBullet()[i].atkPower + RANDOM->range(0, 5), criCheck);

			_chest->damaged(_chest->getPos(), damage, 0, criCheck);

			_player->getBlaze()->setCol(i, true);

			break;
		}
	}
	//meteor
	for (int i = 0; i < _player->getMeteor()->getColSize(); i++)
	{
		if (colCheck(_player->getMeteor()->getColRect(i), _chest->getRc()))
		{
			//ũ��Ƽ��?
			bool criCheck = PLAYERDATA->criAppear();

			//������ ���
			int damage = PLAYERDATA->damageCul(_player->getMeteor()->getAtkPower(i) + RANDOM->range(0, 5), criCheck);

			_chest->damaged(_chest->getPos(), damage, _player->getMeteor()->getSkillNum(), criCheck);

			break;
		}
	}

	//rush
	for (int i = 0; i < _player->getDashFire()->getSize(); i++)
	{
		if (colCheck(_player->getDashFire()->getRect(i), _chest->getRc()))
		{
			bool criCheck = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getDashFire()->getAtk(i) + RANDOM->range(0, 5), criCheck);

			_chest->damaged(_chest->getPos(), damage, _player->getDashFire()->getSkillNum(), criCheck);
		}

	}//end of for

	//infertno
	if (colCheck(_player->getInferno()->getInf().rc, _chest->getRc()))
	{
		//����¡ + ���� ���°� �ƴ� �� inferno�� ���� �浹 üũ
		if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_chest->getRc()))
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

			_chest->damaged(_chest->getPos(), damage, _player->getInferno()->getSkillNum(), criCheck);

		}
	}


	//dragonArc
	if (!_player->getDragon()->getUpgrade())
	{
		for (int i = 0; i < _player->getDragon()->getSize(); i++)
		{
			if (colCheck(_player->getDragon()->getDragonRC(i), _chest->getRc()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3), criCheck);

				_chest->damaged(_chest->getPos(), damage, _player->getDragon()->getSkillNum(), criCheck);
			}

		}
	}

	//upgrade
	for (int i = 0; i < _player->getDragon()->getcolSize(); i++)
	{
		if (colCheck(_player->getDragon()->getColRc(i), _chest->getRc()))
		{
			bool criCheck = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3), criCheck);

			_chest->damaged(_chest->getPos(), damage, _player->getDragon()->getSkillNum(), criCheck);

		}

	}

	//ice Spear
	for (int i = 0; i < _player->getSpear()->getSize(); i++)
	{
		if (colCheck(_player->getSpear()->getSpearRc(i), _chest->getRc()))
		{
			bool criCheck = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getAtkPower(i) + RANDOM->range(0, 3), criCheck);

			_chest->damaged(_chest->getPos(), damage, _player->getSpear()->getSkillNum(), criCheck);

			_player->getSpear()->setCol(i, true);
		}
	}

	//upgrade
	for (int i = 0; i < _player->getSpear()->getUpgradeSize(); i++)
	{
		if (colCheck(_player->getSpear()->getUpgradeRC(i), _chest->getRc()))
		{
			bool criCheck = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getUpgradeAtk(i) + RANDOM->range(0, 3), criCheck);

			_chest->damaged(_chest->getPos(), damage, _player->getSpear()->getSkillNum(), criCheck);

			_player->getSpear()->setUpgradeCol(i, true);
		}
	}
}

void gameScene::gainArcana()
{
	for (int i = 0; i < DROP->getCardVec().size(); i++)
	{
		if (colCheck(_player->getRect(), DROP->getCardRect(i)))
		{
			if (INPUT->GetKeyDown('F'))
			{
				_player->setNewSignatrue(DROP->getCardVec()[i].arcanaName, DROP->getCardVec()[i].coolTime);

				DROP->delCard(i);

			}
		}
	}
}

void gameScene::viewText()
{
	//char str[126];
	//sprintf(str, "%.f, %.f", _player->getX(), _player->getY());
	//TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));
}

void gameScene::soundInit()
{
	//sound 

	isIngameBGM = true;
	fadeIn = 0.f;

	SOUNDMANAGER->stop("mapToolBGM");
	SOUNDMANAGER->stop("titleBGM");
	SOUNDMANAGER->stop("bossBGM");
	SOUNDMANAGER->play("ingameBGM", true);
}

void gameScene::warp()
{
	//portal
	PORTAL->update(_player->getRect());

	if (PORTAL->getCenterCol())
	{
		UI->fadeIn();

		PORTAL->setCenterCol(false);

		POINT warp = PORTAL->getCenterPortalEndPt();
		POINT fixPt = { warp.x,warp.y + 150 };

		_player->setX(warp.x);
		_player->setY(fixPt.y);
		_player->reposRect();

		string name = "portal";
		name += to_string(PORTAL->getCenterColor());
		EFFECT->setBackEffect(name, fixPt, 3);
		SOUNDMANAGER->play("portalIN", false);
		_player->setIdleDelay(50);


		//cout << "�߾�->N" << name << '\n';
	}

	for (int i = 0; i < 3; i++)
	{
		if (PORTAL->getPortalCol(i) == false)continue;
		UI->fadeIn();
		PORTAL->resetCol();
		POINT warp = PORTAL->getPortalEndPt(i);

		POINT fixPt = warp;
		fixPt.y += 200;

		_player->setX(warp.x);
		_player->setY(fixPt.y);
		_player->reposRect();

		string name = "portal";
		name += to_string(PORTAL->getPortalColor(i));
		EFFECT->setBackEffect(name, fixPt, 3);
		SOUNDMANAGER->play("portalIN", false);
		//cout << "N->�߾�" << name << '\n';

		PORTAL->setCenterColor(PORTAL->getPortalColor(i));

		PORTAL->setCenterEndPt(PORTAL->getPortalcurPt(i));
		PORTAL->setCenterActive(true);
		_player->setIdleDelay(50);

	}

	if (isWarp == false && colCheck(_player->getRect(), PORTAL->getWarpScene()))
	{
		UI->setActiveButton(true);
		UI->setPoint({ (LONG)PORTAL->getWarpSceneX(),(LONG)PORTAL->getWarpSceneY() });
	}
	else UI->setActiveButton(false);

	if (isWarp == false && colCheck(_player->getRect(), PORTAL->getWarpScene())
		&& INPUT->GetKeyDown('F'))
	{
		_player->setIdleDelay(50);
		isWarp = true;

		EFFECT->setPortalEffect({ (long)PORTAL->getWarpSceneX(),(long)PORTAL->getWarpSceneY() - 30 });
		SOUNDMANAGER->play("portalWarp", false);
	}

	if (isWarp)
	{
		warpDelay++;

		if (warpDelay > 30)
		{
			SOUNDMANAGER->stop("ingameBGM");
			PLAYERDATA->setCurrentStage(STAGEKIND::MIDDLESTAGE);
			SCENEMANAGER->loadScene("�ε�ȭ��");
		}
	}
}