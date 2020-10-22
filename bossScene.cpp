#include "stdafx.h"
#include "bossScene.h"

bossScene::bossScene() : _player(nullptr), _boss(nullptr) {}

HRESULT bossScene::init()
{
	IMAGEMANAGER->addImage("bossRoom", "map/bossRoom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("frontFrame", "map/frontFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("backFrame", "map/backFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	//UI->init();

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
	DROP->init();
	PORTAL->init();
	//UI->setCoin(PLAYERDATA->getCoin());
	//UI->setHp(PLAYERDATA->getHp());

	_chest = new chest;

	//sound
	soundInit();

	warpDelay = 0;
	isWarp = false;
	gameOver = false;

	UI->fadeIn();
	EFFECT->setPortalEffect({ (long)_player->getX(),(long)_player->getY() });

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
	PORTAL->update(_player->getRect());
	_player->getBlaze()->setBossScene(true);


	PLAYERDATA->update();
	PLAYERDATA->setX(_player->getX());
	PLAYERDATA->setY(_player->getY());
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

	attackChest();

	if (INPUT->GetKeyDown('J'))
	{
		SOUNDMANAGER->stop("bossBGM");
		PLAYERDATA->setCurrentStage(STAGEKIND::FINALSTAGE);
		SCENEMANAGER->loadScene("로딩화면");
	}

	if (_boss->getGameOver() && gameOver == false)
	{
		gameOver = true;
		//상자 생성
		_chest->init("GoldChest", { _boss->getBossRect().left, _boss->getBossRect().top }, 10);
	}

	if (gameOver) _chest->bossScene_update();

	DROP->coinGenerator();

	warp();
}

void bossScene::warp()
{
	if (isWarp == false && colCheck(_player->getRect(), PORTAL->getWarpScene()))
	{
		UI->setActiveButton(true);
		UI->setPoint({ (LONG)PORTAL->getWarpSceneX(),(LONG)PORTAL->getWarpSceneY() });
	}
	else UI->setActiveButton(false);

	if (isWarp == false && colCheck(_player->getRect(), PORTAL->getWarpScene())
		&& INPUT->GetKeyDown('F'))
	{
		isWarp = true;
		
		EFFECT->setPortalEffect({ (long)PORTAL->getWarpSceneX(),(long)PORTAL->getWarpSceneY() - 30 });
		SOUNDMANAGER->play("portalWarp", false);
	}

	if (isWarp)
	{
		warpDelay++;

		if (warpDelay > 30)
		{
			SOUNDMANAGER->stop("bossBGM");
			PLAYERDATA->setCurrentStage(STAGEKIND::FINALSTAGE);
			SCENEMANAGER->loadScene("로딩화면");
		}
	}
}

void bossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		SOUNDMANAGER->play("CutSceneSFX", false, -0.3f);
		SOUNDMANAGER->play("CutSceneVO", false, -0.2f);
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

	PORTAL->bossScene_backRender(getMemDC());

	EFFECT->pRender(getMemDC());

	DROP->cardRender(getMemDC());

	if(warpDelay == 0) _player->render(1);
	//CAMERAMANAGER->Rectangle(getMemDC(), rc);
	//UNITRENDER->render(getMemDC());
	if(gameOver) _chest->render(getMemDC());
	DROP->cardRender(getMemDC());
	_boss->render();
	DROP->render(getMemDC());

	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("frontFrame"), 0, 0);
	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);

	if (isBattle) {
		char str[128];
		wsprintf(str, "EARTH LOAD ATLAS");
		_boss->bossHpInfo(getMemDC(), WINSIZEX / 2 - 226, 100);

		TextOut(getMemDC(), WINSIZEX / 2 - 110, 90, str, strlen(str));

	}

	DROP->coinEffectRender(getMemDC());

	_player->invenRender();

	DAMAGE->render(getMemDC());

	EFFECT->portalRender(getMemDC());

	ptOut(getMemDC(), { 500,500 }, 
		{CAMERAMANAGER->GetAbsoluteX(_ptMouse.x),
		CAMERAMANAGER->GetAbsoluteY(_ptMouse.y)},
		RGB(255, 255, 255));
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
	if (_boss->getBoss().bossHp <= 0) return;
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
				_player->chargeSkillGauge(damage, 0);
			}


			SOUNDMANAGER->play("blazeExp", false);
			PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
				_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);

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

			_boss->damage(damage, 0, 0, _player->getMeteor()->getSkillNum(), isCri);

			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}
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
				_player->chargeSkillGauge(damage, 2);
			}


			_boss->damage(damage, 0, 0, _player->getDashFire()->getSkillNum(), isCri);

			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}
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
	else if (PLAYERDATA->getGaugeTime() >= 50 && _player->getInferno()->CheckCollision(_boss->getBossRect()))
	{
		bool isCri = PLAYERDATA->criAppear();

		int damage = PLAYERDATA->damageCul(_player->getInferno()->getInf().atkPower, isCri);

		//gauge
		if (PLAYERDATA->getStat().ManaRejection == false)
		{
			_player->chargeSkillGauge(damage, 3);
		}

		float angle = getAngle(_boss->getBoss().center.x, _boss->getBoss().center.y,
			_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);

		float x = _boss->getBoss().center.x + cosf(angle) * 6.f;
		float y = _boss->getBoss().center.y - sinf(angle) * 6.f;
		// set boss center x,y (inferno 공격 당하면 inferno쪽으로 끌려오기)
		_boss->setCenter({ (long)x, (long)y });
		_boss->setRect((int)x, (int)y);
		

		_boss->damage(damage, 0, 0, 3, isCri);

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
				int damage = PLAYERDATA->damageCul(_player->getDragon()->getAtkPower(i) + RANDOM->range(0, 3), isCri);

				if (PLAYERDATA->getStat().ManaRejection == false)
				{
					_player->chargeSkillGauge(damage, 4);
				}

				_boss->damage(damage, _player->getDragon()->getDragonAngle(i), 10.f, 4, isCri);

				if (!_boss->getBossAtack()) {
					_boss->setBossHit(true);
				}

			}
		}

	}

	for (int i = 0; i < _player->getDragon()->getcolSize(); i++)
	{
		if (colCheck(_player->getDragon()->getColRc(i), _boss->getBossRect()))
		{
			bool isCri = PLAYERDATA->criAppear();
			int damage = PLAYERDATA->damageCul(_player->getDragon()->getUpgradeAtkPower(i) + RANDOM->range(0, 3), isCri);

			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 4);
			}
			_boss->damage(damage, _player->getDragon()->getHeadAngle(i), 10.f, 4, isCri);

			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}
		}
	}



	//ice Spear
	for (int i = 0; i < _player->getSpear()->getSize(); i++)
	{
		if (colCheck(_player->getSpear()->getSpearRc(i), _boss->getBossRect()))
		{
			//enemy->getMinion()[j]->setPt(_player->getSpear()->getSpearRc(i).left, _player->getSpear()->getSpearRc(i).top);

			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getAtkPower(i) + RANDOM->range(0, 3), isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 5);
			}
			_boss->damage(damage, _player->getSpear()->getSpearAngle(i), 20.f, 5, isCri);
		
			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}
			_player->getSpear()->setCol(i, true);
		}
	}

	//upgrade
	for (int i = 0; i < _player->getSpear()->getUpgradeSize(); i++)
	{
		if (colCheck(_player->getSpear()->getUpgradeRC(i), _boss->getBossRect()))
		{
			bool isCri = PLAYERDATA->criAppear();

			int damage = PLAYERDATA->damageCul(_player->getSpear()->getUpgradeAtk(i) + RANDOM->range(0, 3), isCri);
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 5);
			}
			_boss->damage(damage, _player->getSpear()->getUpgradeAngle(i), 10.f, 5, isCri);
		
			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}

			_player->getSpear()->setUpgradeCol(i, true);
		}

	}


}

void bossScene::attackChest()

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
			//크리티컬?
			bool criCheck = PLAYERDATA->criAppear();

			//데미지 계산
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
		//게이징 + 무브 상태가 아닐 때 inferno와 몬스터 충돌 체크
		if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_chest->getRc()))
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