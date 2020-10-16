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
	_player->getBlaze()->setBossScene(false);

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

	_wall->update();

	_player->setTileAd0(_wall->getVtile());

	UI->update();
	DROP->update();

	_player->update();
	_player->animation();


	enemy->setPlayerRC(RectMake(_player->getX(), _player->getY(), 100, 100));
	enemy->update();

	UNITRENDER->enemyClear();
	enemyUnitRenderInit(); // 무조건 UNITERENDER->enemyClear와 UNITRENDER->update() 사이에 있어야함.
	UNITRENDER->update();

	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX + 15, WINSIZEY + 15);

	checkArea = RectMake(_player->getX() - 100, _player->getY() - WINSIZEY / 2 + 420, 200, 500);

	CAMERAMANAGER->update();
	PARTICLE->pointActive();
	PARTICLE->explosionActive();

	//collision between player skill and enemy

	playerAttack();
	enemyAttack();

	_shop->shopCollider(_player->getRect());
	_shop->buyItem();

	if (INPUT->GetKeyDown('J'))
	{
		SCENEMANAGER->loadScene("보스방");
	}

	if (INPUT->GetKeyDown(VK_BACK))
	{
		//DROP->getCoinEffect(1);
		EFFECT->setEmotionEffect("buyEmote_Nox", { (long)_player->getX() ,(long)_player->getY() - 80 });
	}

	UNITRENDER->setPlayerRect(_player->getRect());


}

void gameScene::render()
{
	_wall->render2();
	//_wall->render();

	_shop->render();

	DROP->render(getMemDC());
	EFFECT->pRender(getMemDC());




	_player->render();

	UNITRENDER->render(getMemDC());

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



	UI->render(getMemDC(), 50, 50);
	_player->invenRender();

	viewText();

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
					_player->chargeSkillGauge(damage, 0);
				}


				SOUNDMANAGER->play("blazeExp", false);
				PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
					_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);
				enemy->getMinion()[j]->hit(damage, _player->getBlaze()->getBullet()[i].angle, 20.f, 0, criCheck);

				_player->getBlaze()->setCol(i, true);

				break;
			}
		}
	}//end of for

	//meteor
	for (int i = 0; i < _player->getMeteor()->getColSize(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (0 >= enemy->getMinion()[j]->getHp())continue;
			if (colCheck(_player->getMeteor()->getColRect(i), enemy->getMinion()[j]->getRC()))
			{
				//크리티컬?
				bool criCheck = PLAYERDATA->criAppear();

				//데미지 계산
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
	}//end of for

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
	}//end of for


	//inferno
	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		long enemyX = enemy->getMinion()[i]->getPos().x;
		long enemyY = enemy->getMinion()[i]->getPos().y;

		if (0 >= enemy->getMinion()[i]->getHp())continue;
		if (colCheck(_player->getInferno()->getInf().rc, enemy->getMinion()[i]->getRC()))
		{
			//게이징 + 무브 상태가 아닐 때 inferno와 몬스터 충돌 체크
			if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(enemy->getMinion()[i]->getRC()))
			{
				//충돌되면 그 자리에서 공격
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

				float angle = getAngle(enemyX + 40, enemyY + 40,
					_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

				float x = enemyX + cosf(angle) * 4.5f;
				float y = enemyY - sinf(angle) * 4.5f;

				enemy->getMinion()[i]->setPt(x, y);
				enemy->getMinion()[i]->hit(damage, 0, 0.f, 3, criCheck);
			}
		}
	}//end of for

	//dragonArc
	if (!_player->getDragon()->getUpgrade())
	{
		for (int i = 0; i < _player->getDragon()->getSize(); i++)
		{
			for (int j = 0; j < enemy->getMinion().size(); j++)
			{
				if (colCheck(_player->getDragon()->getDragonRC(i), enemy->getMinion()[j]->getRC()))
				{
					bool criCheck = PLAYERDATA->criAppear();

					int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3), criCheck);

					//gauge
					if (PLAYERDATA->getStat().ManaRejection == false)
					{
						_player->chargeSkillGauge(damage, 4);
					}
					enemy->getMinion()[j]->hit(damage, _player->getDragon()->getDragonAngle(i), 10.f, 4, criCheck);
				}
			}
		}
	}

	//upgrade
	for (int i = 0; i < _player->getDragon()->getcolSize(); i++)
	{
		for (int j = 0; j < enemy->getMinion().size(); j++)
		{
			if (colCheck(_player->getDragon()->getColRc(i), enemy->getMinion()[j]->getRC()))
			{
				bool criCheck = PLAYERDATA->criAppear();

				int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3), criCheck);

				//gauge
				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 4);
				}
				enemy->getMinion()[j]->hit(damage, _player->getDragon()->getHeadAngle(i), 10.f,4, criCheck);
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

void gameScene::enemyUnitRenderInit()
{
	for (int i = 0; i < enemy->getMinion().size(); i++)
	{
		POINT currentFrame = enemy->getMinion()[i]->getCurrentFrame();
		POINT monsterPos = enemy->getMinion()[i]->getPos();
		switch (enemy->getMinion()[i]->getMonsterKind())
		{
		case MONSTERKIND::GOLEM:
			if (enemy->getMinion()[i]->getHit() && !enemy->getMinion()[i]->getDie())
				UNITRENDER->enemyInit(4, currentFrame, monsterPos);//골렘히트시
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
		}

	}
}

void gameScene::viewText()
{
	char str[126];
	sprintf(str, "%.f, %.f", _player->getX(), _player->getY());
	TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));
}

void gameScene::soundInit()
{
	//sound 



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

	SOUNDMANAGER->addSound("summonerCasting", "Sound/summoner_casting.mp3");
	SOUNDMANAGER->addSound("summonerFire", "Sound/summoner_fire.mp3");
	SOUNDMANAGER->addSound("summonerAtk", "Sound/summoner_atk.mp3");

	SOUNDMANAGER->stop("mapToolBGM");
	SOUNDMANAGER->stop("titleBGM");
	SOUNDMANAGER->stop("bossBGM");
	SOUNDMANAGER->play("ingameBGM", true);
}