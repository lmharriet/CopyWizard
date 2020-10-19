#include "stdafx.h"
#include "bossScene.h"

bossScene::bossScene() : _player(nullptr), _boss(nullptr) {}

HRESULT bossScene::init()
{
	IMAGEMANAGER->addImage("bossRoom", "map/bossRoom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("frontFrame", "map/frontFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("backFrame", "map/backFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	UI->init();

	_player = new player;
	_player->init();
	_player->setX(752);
	_player->setY(856);
	_player->reposRect();
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXWIDTH * 2, MAXHEIGHT * 2,
		-MAXWIDTH, -MAXHEIGHT, WINSIZEX / 2, WINSIZEY / 2);

	EFFECT->init();


	_boss = new boss;
	_boss->init(752, 288);
	rc = RectMakeCenter(752, 288, 150, 150);
	area = RectMakeCenter(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, WINSIZEX, WINSIZEY);

	isBattle = false;
	cutTime = 0;

	_boss->getPlayerInfo(_player);

	PARTICLE->init();
	DAMAGE->init();

	//UI->setCoin(PLAYERDATA->getCoin());
	//UI->setHp(PLAYERDATA->getHp());

	//sound
	soundInit();
	

	return S_OK;
}

void bossScene::release()
{
	if (_player != nullptr)
	{
		_player->release();
		SAFE_DELETE(_player);

		_boss->release();
		SAFE_DELETE(_boss);

		SOUNDMANAGER->stop("bossBGM");
	}
}

void bossScene::update()
{
	
	_player->getBlaze()->setBossScene(true);


	PLAYERDATA->update();
	//sound 
	if (isBossBGM)
	{
		SOUNDMANAGER->fadeIn("bossBGM", fadeIn);
		fadeIn += 0.002f;
		if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
			isBossBGM = false;
	}
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
	_player->colorCheck(IMAGEMANAGER->findImage("bossRoom"));

	//cutScene check
	bossCutScene();

	if (isBattle) {
		_boss->update();
	}

	PARTICLE->pointActive();
	PARTICLE->explosionActive();


	attackBoss();

	if (INPUT->GetKeyDown('J'))
	{
		SCENEMANAGER->loadScene("시작화면");
	}
}

void bossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		SOUNDMANAGER->play("CutSceneSFX", false,-0.2f);
		SOUNDMANAGER->play("CutSceneVO", false,-0.1f);
		isBattle = true; // 한번만 실행하기 위함
		CAMERAMANAGER->setCutScene
		(
			_player->getX(),                        // 시작 좌표 x
			_player->getY(),                        // 시작 좌표 y
			rc.left + (rc.right - rc.left) / 2,     // 목적 좌표 x
			rc.top + (rc.bottom - rc.top) / 2,      // 목적 좌표 y
			true,                                   // 시작 -> 목적 isLerp?
			true,                                   // 목적 -> 시작 isLerp?
			150,                                    // 컷씬 유지 시간
			50,                                     // 시작지점으로 되돌아오는 시간
			10.f                                    // lerp 속도
		);
	}
}

void bossScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("bossRoom"), 0, 0);
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("backFrame"), 0, 0);

	//CAMERAMANAGER->Rectangle(getMemDC(), area);

	EFFECT->pRender(getMemDC());

	_player->render(1);
	//CAMERAMANAGER->Rectangle(getMemDC(), rc);
	//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("boss"), rc.left, rc.top, 0, 0);

	_boss->render();

	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("frontFrame"), 0, 0);
	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);

	if (isBattle) {
		char str[128];
		wsprintf(str, "EARTH LOAD ATLAS");
		_boss->bossHpInfo(getMemDC(), WINSIZEX / 2 - 226, 100);

		TextOut(getMemDC(), WINSIZEX / 2 - 110, 90, str, strlen(str));
		
	}

	_player->invenRender();
	
	DAMAGE->render(getMemDC());
}

void bossScene::soundInit()
{
	

	isBossBGM = true;
	fadeIn = 0.f;

	

	SOUNDMANAGER->stop("ingameBGM");
	SOUNDMANAGER->stop("mapToolBGM");
	SOUNDMANAGER->stop("titleBGM");

	SOUNDMANAGER->play("bossBGM", true);
	
}

void bossScene::attackBoss()
{
	//blaze
	for (int i = 0; i < _player->getBlaze()->getSize(); i++)
	{
		if (colCheck(_player->getBlaze()->getBullet()[i].rc, _boss->getBossRect()))
		{
			

			bool isCri = PLAYERDATA->criAppear();


			int damage = PLAYERDATA->damageCul(_player->getBlaze()->getBullet()[i].atkPower, isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage,0);
			}


			SOUNDMANAGER->play("blazeExp", false);
			PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
				_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);

			// -> boss damaged
			//_boss->setBossHp(10);
			_boss->damage(damage, 0, 0, 0, isCri);

			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}

			_player->getBlaze()->setCol(i, true);

		}
	}
	//meteor
	for (int i = 0; i < _player->getMeteor()->getColSize(); i++)
	{
		if (colCheck(_player->getMeteor()->getColRect(i), _boss->getBossRect()))
		{
			
			bool isCri = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getMeteor()->getAtkPower(i), isCri);

			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 1);
			}

			//-> boss damaged
			//_boss->setBossHp(10);

			_boss->damage(damage, 0, 0, _player->getMeteor()->getSkillNum(), isCri);
		}
	}

	//searingDash
	for (int i = 0; i < _player->getDashFire()->getSize(); i++)
	{
		if (colCheck(_player->getDashFire()->getRect(i), _boss->getBossRect()))
		{
			

			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getDashFire()->getAtk(i), isCri);


			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage,2);
			}


			//-> boss damaged
			//_boss->setBossHp(10);
			_boss->damage(damage, 0, 0, _player->getDashFire()->getSkillNum(), isCri);
		}
	}

	//inferno

	//게이징 + 무브 상태가 아닐 때 inferno와 보스 충돌 체크


	if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_boss->getBossRect()))
	{
		//충돌되면 그 자리에서 공격
		if (PLAYERDATA->getGaugeTime() < 50)
		{
			PLAYERDATA->setGaugeTime(50);
			_player->getInferno()->setActive(true);

		}
	}
	else if (PLAYERDATA->getGaugeTime() >= 50 &&_player->getInferno()->CheckCollision(_boss->getBossRect()))
	{
		bool isCri = PLAYERDATA->criAppear();
	
		int damage = PLAYERDATA->damageCul(_player->getInferno()->getInf().atkPower, isCri);

		//gauge
		if (PLAYERDATA->getStat().ManaRejection == false)
		{
			_player->chargeSkillGauge(damage,3);
		}
		
		float angle = getAngle(_boss->getBoss().center.x, _boss->getBoss().center.y,
			_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);
		
		float x = _boss->getBoss().center.x + cosf(angle) * 6.f;
		float y = _boss->getBoss().center.y - sinf(angle) * 6.f;
		// set boss center x,y (inferno 공격 당하면 inferno쪽으로 끌려오기)
		_boss->setCenter({ (long)x, (long)y });
		_boss->setRect((int)x, (int)y);
		//-> boss damaged
		//_boss->setBossHp(10);
		
		_boss->damage(damage, 0, 0, 3, isCri);
		//DAMAGE->generator({ (long)_boss->getBossRect().left + 75, (long)_boss->getBossRect().top + 75 }, "numbers", damage);

		if (!_boss->getBossAtack()) {
			_boss->setBossHit(true);
		}
	}

	//dragonArc
	if (!_player->getDragon()->getUpgrade())
	{
		for (int i = 0; i < _player->getDragon()->getSize(); i++)
		{
			if (colCheck(_player->getDragon()->getDragonRC(i), _boss->getBossRect()))
			{
				bool isCri = PLAYERDATA->criAppear();
				int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3),isCri);

				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 4);
				}

				_boss->damage(damage, _player->getDragon()->getDragonAngle(i), 10.f, 4, isCri);

			}
		}

	}
	
	for (int i = 0; i < _player->getDragon()->getcolSize(); i++)
	{
		if (colCheck(_player->getDragon()->getColRc(i), _boss->getBossRect()))
		{
			bool isCri = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3),isCri);

			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 4);
			}
			_boss->damage(damage, _player->getDragon()->getHeadAngle(i), 10.f, 4, isCri);
		}
	}
}