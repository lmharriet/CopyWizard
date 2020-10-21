#include "stdafx.h"
#include "finalBossScene.h"

HRESULT finalBossScene::init()
{
	IMAGEMANAGER->addImage("finalbossroom", "resource/boss/final boss room.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalbackframe", "resource/boss/final boss room margenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("finalroom", "resource/boss/finalroom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalframe", "resource/boss/finalroommargenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	UI->init();

	_player = new player;
	_player->init();
	_player->setX(MAXWIDTH / 2);
	_player->setY(MAXHEIGHT - 200);
	_player->reposRect();
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXWIDTH * 2, MAXHEIGHT * 2,
		-MAXWIDTH, -MAXHEIGHT, WINSIZEX / 2, WINSIZEY / 2);

	EFFECT->init();

	_finalBoss = new finalBoss;
	_finalBoss->init(MAXWIDTH / 2, MAXHEIGHT / 2);

	rc = RectMakeCenter(MAXWIDTH / 2, MAXHEIGHT / 2, 150, 150);
	area = RectMakeCenter(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, WINSIZEX, WINSIZEY);

	isBattle = false;
	mapChange = false;
	cutTime = 0;
	mapCount = 0;

	_finalBoss->getPlayerInfo(_player);

	PARTICLE->init();
	DAMAGE->init();

	return S_OK;
}

void finalBossScene::release()
{
	if (_finalBoss != nullptr)
	{
		_player->release();
		SAFE_DELETE(_player);

		_finalBoss->release();
		SAFE_DELETE(_finalBoss);
	}
}

void finalBossScene::update()
{
	_player->getBlaze()->setBossScene(true);

	PLAYERDATA->update();

	//camera update
	CAMERAMANAGER->update();

	//player update
	if (!CAMERAMANAGER->getIsCutScene())
	{
		UI->update();
		_player->other_update();
	}
	else _player->setStateIDLE();


	//pixel collision
	_player->colorCheck(IMAGEMANAGER->findImage("finalbackframe"));

	//cutScene check
	bossCutScene();
	attackSura();
	if (isBattle) {
		_finalBoss->update();
	}

	if (isBattle && !mapChange) {
		mapCount++;
		if (mapCount > 180) {
			CAMERAMANAGER->Shake(50, 50, 10);
			mapCount = 0;
			mapChange = true;
		}
	}

	PARTICLE->pointActive();
	PARTICLE->explosionActive();
}

void finalBossScene::render()
{
	if (!mapChange) {
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalframe"), 0, 0);
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalroom"), 0, 0);
	}
	else {
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbackframe"), 0, 0);
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("finalbossroom"), 0, 0);
	}

	EFFECT->pRender(getMemDC());

	_player->render(1);

	_finalBoss->render();

	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);

	_player->invenRender();

	if (isBattle) {
		char str[128];
		wsprintf(str, "MATER SURA");
		_finalBoss->finalBossHpInfo(getMemDC(), WINSIZEX / 2 - 226, 100);

		TextOut(getMemDC(), WINSIZEX / 2 - 70, 90, str, strlen(str));

	}

	DAMAGE->render(getMemDC());
}

void finalBossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		isBattle = true; // �ѹ��� �����ϱ� ����
		CAMERAMANAGER->setCutScene
		(
			_player->getX(),                        // ���� ��ǥ x
			_player->getY(),                        // ���� ��ǥ y
			rc.left + (rc.right - rc.left) / 2,     // ���� ��ǥ x
			rc.top + (rc.bottom - rc.top) / 2,      // ���� ��ǥ y
			true,                                   // ���� -> ���� isLerp?
			true,                                   // ���� -> ���� isLerp?
			250,                                    // �ƾ� ���� �ð�
			50,                                     // ������������ �ǵ��ƿ��� �ð�
			10.f                                    // lerp �ӵ�
		);
	}
}
void finalBossScene::attackSura()
{
	if (_finalBoss->getFinalBoss().bossHp <= 0) return;
	//blaze
	for (int i = 0; i < _player->getBlaze()->getSize(); i++)
	{
		if (colCheck(_player->getBlaze()->getBullet()[i].rc, _finalBoss->getFinalBoss().rc))
		{
			bool isCri = PLAYERDATA->criAppear();


			int damage = PLAYERDATA->damageCul(_player->getBlaze()->getBullet()[i].atkPower, isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 0);
			}


			SOUNDMANAGER->play("blazeExp", false);
			PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
				_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);

			// -> boss damaged

			_finalBoss->damage(damage, _player->getBlaze()->getBullet()[i].angle,0, isCri);
			_player->getBlaze()->setCol(i, true);
			
			break;

		}
	}
	//meteor
	for (int i = 0; i < _player->getMeteor()->getColSize(); i++)
	{
		if (colCheck(_player->getMeteor()->getColRect(i), _finalBoss->getFinalBoss().rc))
		{

			bool isCri = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getMeteor()->getAtkPower(i), isCri);

			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 1);
			}

			//-> boss damaged

			_finalBoss->damage(damage, _player->getMeteor()->getAngle(i), _player->getMeteor()->getSkillNum(), isCri);
		}
	}

	//searingDash
	for (int i = 0; i < _player->getDashFire()->getSize(); i++)
	{
		if (colCheck(_player->getDashFire()->getRect(i), _finalBoss->getFinalBoss().rc))
		{

			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getDashFire()->getAtk(i), isCri);


			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 2);
			}


			//-> boss damaged

			_finalBoss->damage(damage, 0, _player->getDashFire()->getSkillNum(), isCri);
		}
	}

	//inferno

	//����¡ + ���� ���°� �ƴ� �� inferno�� ���� �浹 üũ


	if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_finalBoss->getFinalBoss().rc))
	{
		//�浹�Ǹ� �� �ڸ����� ����
		if (PLAYERDATA->getGaugeTime() < 50)
		{
			PLAYERDATA->setGaugeTime(50);
			_player->getInferno()->setActive(true);

		}
	}
	else if (PLAYERDATA->getGaugeTime() >= 50 && _player->getInferno()->CheckCollision(_finalBoss->getFinalBoss().rc))
	{
		bool isCri = PLAYERDATA->criAppear();

		int damage = PLAYERDATA->damageCul(_player->getInferno()->getInf().atkPower, isCri);

		//gauge
		if (PLAYERDATA->getStat().ManaRejection == false)
		{
			_player->chargeSkillGauge(damage, 3);
		}

		float angle = getAngle(_finalBoss->getFinalBoss().center.x, _finalBoss->getFinalBoss().center.y,
			_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

		float x = _finalBoss->getFinalBoss().center.x + cosf(angle) * 6.f;
		float y = _finalBoss->getFinalBoss().center.y - sinf(angle) * 6.f;
		 //set boss center x,y (inferno ���� ���ϸ� inferno������ ��������)
		_finalBoss->setCenter({ (long)x, (long)y });
		_finalBoss->setRect((int)x, (int)y);


		//-> boss damaged
		_finalBoss->damage(damage, _player->getInferno()->getInf().angle , _player->getInferno()->getSkillNum(), isCri);
	}

	//dragonArc
	if (!_player->getDragon()->getUpgrade())
	{
		for (int i = 0; i < _player->getDragon()->getSize(); i++)
		{
			if (colCheck(_player->getDragon()->getDragonRC(i), _finalBoss->getFinalBoss().rc))
			{
				bool isCri = PLAYERDATA->criAppear();
				int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3), isCri);

				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 4);
				}

				//bossDamage->

				_finalBoss->damage(damage, _player->getDragon()->getDragonAngle(i), _player->getDragon()->getSkillNum(), isCri);
			}
		}

	}
	//upgrade dragon
	for (int i = 0; i < _player->getDragon()->getcolSize(); i++)
	{
		if (colCheck(_player->getDragon()->getColRc(i), _finalBoss->getFinalBoss().rc))
		{
			bool isCri = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3), isCri);

			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 4);
			}
			//bossDamage ->


			_finalBoss->damage(damage, _player->getDragon()->getHeadAngle(i), _player->getDragon()->getSkillNum(), isCri);
		}
	}



	//ice Spear
	for (int i = 0; i < _player->getSpear()->getSize(); i++)
	{
		if (colCheck(_player->getSpear()->getSpearRc(i), _finalBoss->getFinalBoss().rc))
		{
			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getAtkPower(i) + RANDOM->range(0, 3), isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 5);
			}

			//boss Damage->
			_finalBoss->damage(damage, _player->getSpear()->getSpearAngle(i), 5, isCri);

			_player->getSpear()->setCol(i, true);
		}
	}

	//upgrade
	for (int i = 0; i < _player->getSpear()->getUpgradeSize(); i++)
	{
		if (colCheck(_player->getSpear()->getUpgradeRC(i), _finalBoss->getFinalBoss().rc))
		{
			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getUpgradeAtk(i) + RANDOM->range(0, 3), isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 5);
			}
			
			//->bossDamage


			_finalBoss->damage(damage, _player->getSpear()->getUpgradeAngle(i), 5, isCri);
			_player->getSpear()->setUpgradeCol(i, true);
		}

	}
}