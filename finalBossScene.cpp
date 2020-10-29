
#include "stdafx.h"
#include "finalBossScene.h"

HRESULT finalBossScene::init()
{
	
	IMAGEMANAGER->addImage("finalbossroom", "resource/boss/final boss room.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalbackframe", "resource/boss/final boss room margenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("finalroom", "resource/boss/finalroom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("finalframe", "resource/boss/finalroommargenta.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));

	//UI->init();

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

	UI->fadeIn();
	EFFECT->setPortalEffect({ (long)_player->getX(),(long)_player->getY() });

	_chest = new chest;
	isEnd = false;
	endCutTime = 0;
	isCutScene = false;
	saveY = 0;

	IMAGEMANAGER->addImage("boxHead", "Images/npc/boxHead.bmp", 213, 168, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("boxBottom", "Images/npc/boxBottom.bmp", 213, 168, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("carrot", "Images/npc/carrot.bmp", 50, 45, true, RGB(255, 0, 255));

	//sound
	isBossBGM = false;
	fadeIn = 0.f;
	sTime = 0;
	
	SOUNDMANAGER->play("portalWarp", false);
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
	if (_finalBoss->getStageChange())
	{
		if (isBossBGM)
		{
			
			SOUNDMANAGER->fadeIn("bossBGM", fadeIn);
			fadeIn += 0.002f;
			if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
				isBossBGM = false;
		}
	}
	else
	{
		if (!isBossBGM)
		{
			SOUNDMANAGER->play("castleAmb", false,-0.38f);
			isBossBGM = true;
		}
	}

	_player->getBlaze()->setBossScene(true);

	PLAYERDATA->update();

	//camera update
	CAMERAMANAGER->update();

	//player update
	if (!CAMERAMANAGER->getIsCutScene())
	{
		UI->update();

		if(endCutTime == 0) _player->other_update();

		else
		{
			CAMERAMANAGER->MovePivotLerp(bottomBox.x + 80, saveY + 60, 1.5f);
		}
	}
	else _player->setStateIDLE();


	//pixel collision
	if (!mapChange) {
		_player->colorCheck(IMAGEMANAGER->findImage("finalframe"));
	}
	else {
		_player->colorCheck(IMAGEMANAGER->findImage("finalbackframe"));
	}

	//cutScene check
	bossCutScene();
	attackSura();
	if (isBattle) {
		_finalBoss->update();
	}

	if (isEnd == false && _finalBoss->getGameOver())
	{
		isEnd = true;
		_player->setStateIDLE();
		pFrame = { 1,0 };
		boxRc = RectMakeCenter(_finalBoss->getX(), _finalBoss->getY() - 60, 300, 300);
		//				_finalBoss->getX() - IMAGEMANAGER->findImage("endingBox")->getFrameWidth() / 2,
		//_finalBoss->getY() - 100 - IMAGEMANAGER->findImage("endingBox")->getFrameHeight() / 2,
		topBox = { _finalBoss->getX() - IMAGEMANAGER->findImage("boxHead")->getWidth() / 2,
			_finalBoss->getY() - 100 - IMAGEMANAGER->findImage("boxBottom")->getHeight() / 2 };

		bottomBox = { _finalBoss->getX() - IMAGEMANAGER->findImage("boxHead")->getWidth() / 2,
			_finalBoss->getY() - 100 - IMAGEMANAGER->findImage("boxBottom")->getHeight() / 2 };

		saveY = _finalBoss->getY() - 100 - IMAGEMANAGER->findImage("boxBottom")->getHeight() / 2;
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

	if (isCutScene && sTime < 250)sTime++;

	endCutScene();
}

void finalBossScene::endCutScene()
{
	if (isEnd == false)return;

	if (colCheck(_player->getRect(), boxRc))
	{
		endCutTime++;

		if (endCutTime % 15 == 0 && boxFrameX < 5)
		{
			boxFrameX++;
			_player->setStateIDLE();
		}
	}

	if (endCutTime > 100)
	{
		_player->frameAnimation(2, 0, 1);
	}

	if (endCutTime == 200)
	{
		UI->fadeOut();
	}

	if (endCutTime == 300)SCENEMANAGER->loadScene("시작화면");
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


	//

	//

	PORTAL->backRender(getMemDC());

	PORTAL->render(getMemDC());




	/*
	

	*/



	EFFECT->render(getMemDC());
	_player->attackCircleRender();

	if(endCutTime == 0) _player->render(1);

	else
	{
		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"),
			_player->getX() - 50, _player->getY() - 50, pFrame.x, pFrame.y);
	}

	if(isBattle == false || sTime >= 250) PLAYERDATA->shroudRender(getMemDC());

	_finalBoss->render();

	if (isEnd)
	{
		//CAMERAMANAGER->Rectangle(getMemDC(), boxRc);
		if (boxFrameX < 4)
		{
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("endingBox"),
				_finalBoss->getX() - IMAGEMANAGER->findImage("endingBox")->getFrameWidth() / 2,
				_finalBoss->getY() - 100 - IMAGEMANAGER->findImage("endingBox")->getFrameHeight() / 2,
				boxFrameX, 0);
		}

		else
		{
			//topBox,bottomBox
			CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("boxBottom"), bottomBox.x, bottomBox.y);
			CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("carrot"), bottomBox.x + 80, bottomBox.y + 60);
			CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("boxHead"), topBox.x, topBox.y);

			if (topBox.y > (saveY - 100))topBox.y--;
		}
	}

	PARTICLE->render(getMemDC());

	if (PLAYERDATA->getStat().reducedVisibility)
	{
		IMAGEMANAGER->findImage("glassEffect")->render(getMemDC());
	}

	UI->render(getMemDC(), 50, 50);

	_player->invenRender();

	if (isBattle && isEnd == false) {
		char str[128];
		wsprintf(str, "MASTER SURA");
		_finalBoss->finalBossHpInfo(getMemDC(), WINSIZEX / 2 - 226, 100);

		TextOut(getMemDC(), WINSIZEX / 2 - 70, 90, str, strlen(str));
	}

	EFFECT->portalRender(getMemDC());

	DAMAGE->render(getMemDC());
}

void finalBossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		isBattle = true; // 한번만 실행하기 위함
		CAMERAMANAGER->setCutScene
		(
			_player->getX(),                        // 시작 좌표 x
			_player->getY(),                        // 시작 좌표 y
			rc.left + (rc.right - rc.left) / 2,     // 목적 좌표 x
			rc.top + (rc.bottom - rc.top) / 2,      // 목적 좌표 y
			true,                                   // 시작 -> 목적 isLerp?
			true,                                   // 목적 -> 시작 isLerp?
			250,                                    // 컷씬 유지 시간
			50,                                     // 시작지점으로 되돌아오는 시간
			10.f                                    // lerp 속도
		);
		isCutScene = true;
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

	//게이징 + 무브 상태가 아닐 때 inferno와 보스 충돌 체크


	if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_finalBoss->getFinalBoss().rc))
	{
		//충돌되면 그 자리에서 공격
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
		 //set boss center x,y (inferno 공격 당하면 inferno쪽으로 끌려오기)
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