#include "stdafx.h"
#include "bossScene.h"

bossScene::bossScene() : _player(nullptr), _boss(nullptr) {}

HRESULT bossScene::init()
{

	IMAGEMANAGER->addImage("bossRoom", "map/bossRoom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("frontFrame", "map/frontFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("backFrame", "map/backFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
	_player = new player;
	_player->init();
	_player->setX(752);
	_player->setY(856);
	_player->reposRect();
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXWIDTH * 2, MAXHEIGHT * 2,
		-MAXWIDTH, -MAXHEIGHT, WINSIZEX / 2, WINSIZEY / 2);

	EFFECT->init();
	UI->init();

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
		SCENEMANAGER->loadScene("����ȭ��");
	}

}

void bossScene::bossCutScene()
{
	if (!isBattle && colCheck(_player->getRect(), area))
	{
		SOUNDMANAGER->play("CutSceneSFX", false);
		SOUNDMANAGER->play("CutSceneVO", false);
		isBattle = true; // �ѹ��� �����ϱ� ����
		CAMERAMANAGER->setCutScene
		(
			_player->getX(),                        // ���� ��ǥ x
			_player->getY(),                        // ���� ��ǥ y
			rc.left + (rc.right - rc.left) / 2,     // ���� ��ǥ x
			rc.top + (rc.bottom - rc.top) / 2,      // ���� ��ǥ y
			true,                                   // ���� -> ���� isLerp?
			true,                                   // ���� -> ���� isLerp?
			150,                                    // �ƾ� ���� �ð�
			50,                                     // ������������ �ǵ��ƿ��� �ð�
			10.f                                    // lerp �ӵ�
		);
	}
}

void bossScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("bossRoom"), 0, 0);
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("backFrame"), 0, 0);

	//CAMERAMANAGER->Rectangle(getMemDC(), area);

	EFFECT->pRender(getMemDC());
	_player->render();

	//CAMERAMANAGER->Rectangle(getMemDC(), rc);
	//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("boss"), rc.left, rc.top, 0, 0);

	_boss->render();

	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("frontFrame"), 0, 0);
	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);

	DAMAGE->render(getMemDC());
}

void bossScene::soundInit()
{
	SOUNDMANAGER->stop("ingameBGM");
	SOUNDMANAGER->stop("mapToolBGM");
	SOUNDMANAGER->stop("titleBGM");

	SOUNDMANAGER->play("bossBGM", true);

	isBossBGM = true;
	fadeIn = 0.f;

	SOUNDMANAGER->addSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
	SOUNDMANAGER->addSound("bossDrillVO", "Sound/bossDrillVO.mp3");
	SOUNDMANAGER->addSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
	SOUNDMANAGER->addSound("bossJumpVO", "Sound/bossJumpVO.mp3");
	SOUNDMANAGER->addSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
	SOUNDMANAGER->addSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
	SOUNDMANAGER->addSound("bossPunchVO", "Sound/bossPunchVO.mp3");
	SOUNDMANAGER->addSound("bossWallSFX", "Sound/bossWallSFX.mp3");
	SOUNDMANAGER->addSound("bossWallVO", "Sound/bossWallVO.mp3");
	SOUNDMANAGER->addSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
	SOUNDMANAGER->addSound("CutSceneVO", "Sound/CutSceneVO.mp3");

	
}

void bossScene::attackBoss()
{
	//blaze
	for (int i = 0; i < _player->getBlaze()->getSize(); i++)
	{
		if (colCheck(_player->getBlaze()->getBullet()[i].rc, _boss->getBossRect()))
		{
			int damage = _player->getBlaze()->getBullet()[i].atkPower + RANDOM->range(0, 3);

			bool isCri = PLAYERDATA->criAppear();

			if (isCri) //critical damage
				damage = (float)damage * (PLAYERDATA->getStat().damage + PLAYERDATA->getStat().criDamage);
			//normal damage
			else damage = (float)damage * PLAYERDATA->getStat().damage;

			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage,0);
			}


			SOUNDMANAGER->play("blazeExp", false);
			PARTICLE->explosionGenerate("explosionParticle", _player->getBlaze()->getBullet()[i].x + 20,
				_player->getBlaze()->getBullet()[i].y + 20, 12, 50, 2.f, 1, true);

			// -> boss damaged
			_boss->setBossHp(10);
			DAMAGE->generator({ (long)_boss->getBossRect().left + 75, (long)_boss->getBossRect().top + 75 }, "numbers", 10);
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
			int damage = _player->getMeteor()->getAtkPower(i) + RANDOM->range(0, 5);
			bool isCri = PLAYERDATA->criAppear();


			if (isCri) damage = (float)damage * PLAYERDATA->getStat().criDamage;
			else damage = (float)damage * PLAYERDATA->getStat().damage;
			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage, 1);
			}

			//-> boss damaged
			_boss->setBossHp(10);
			DAMAGE->generator({ (long)_boss->getBossRect().left + 75, (long)_boss->getBossRect().top + 75 }, "numbers", 10);
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
			int damage = _player->getDashFire()->getAtk(i) + RANDOM->range(0, 5);

			bool isCri = PLAYERDATA->criAppear();


			if (isCri) //critical damage
				damage = (float)damage * (PLAYERDATA->getStat().damage + PLAYERDATA->getStat().criDamage);
			
			//normal damage
			else damage = (float)damage * PLAYERDATA->getStat().damage;

			//gauge
			if (PLAYERDATA->getStat().ManaRejection == false)
			{
				_player->chargeSkillGauge(damage,2);
			}


			//-> boss damaged
			_boss->setBossHp(10);
			DAMAGE->generator({ (long)_boss->getBossRect().left + 75, (long)_boss->getBossRect().top + 75 }, "numbers", 10);
			if (!_boss->getBossAtack()) {
				_boss->setBossHit(true);
			}
		}
	}

	//inferno

	//����¡ + ���� ���°� �ƴ� �� inferno�� ���� �浹 üũ


	if (!_player->getInferno()->getActive() && _player->getInferno()->CheckCollision(_boss->getBossRect()))
	{
		//�浹�Ǹ� �� �ڸ����� ����
		if (PLAYERDATA->getGaugeTime() < 50)
		{
			PLAYERDATA->setGaugeTime(50);
			_player->getInferno()->setActive(true);

		}
	}
	else if (PLAYERDATA->getGaugeTime() >= 50 &&_player->getInferno()->CheckCollision(_boss->getBossRect()))
	{
		int damage = _player->getInferno()->getInf().atkPower;

		bool isCri = PLAYERDATA->criAppear();

		

		if (isCri) damage = (float)damage * (PLAYERDATA->getStat().damage + PLAYERDATA->getStat().criDamage);
		else damage = (float)damage * PLAYERDATA->getStat().damage;

		//gauge
		if (PLAYERDATA->getStat().ManaRejection == false)
		{
			_player->chargeSkillGauge(damage,0);
		}
		float angle = getAngle(_boss->getBoss().center.x, _boss->getBoss().center.y,
			_player->getInferno()->getInf().x, _player->getInferno()->getInf().y);
		
		float x = _boss->getBoss().center.x + cosf(angle) * 6.f;
		float y = _boss->getBoss().center.y - sinf(angle) * 6.f;
		// set boss center x,y (inferno ���� ���ϸ� inferno������ ��������)
		_boss->setCenter({ (long)x, (long)y });
		_boss->setRect((int)x, (int)y);
		//-> boss damaged
		_boss->setBossHp(10);
		DAMAGE->generator({ (long)_boss->getBossRect().left + 75, (long)_boss->getBossRect().top + 75 }, "numbers", 10);

		if (!_boss->getBossAtack()) {
			_boss->setBossHit(true);
		}
	}

}