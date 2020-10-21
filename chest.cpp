#include "stdafx.h"
#include "chest.h"

HRESULT chest::init(string _keyName, POINT _pos, int _hp)
{
	IMAGEMANAGER->addFrameImage("silverChest", "Images/npc/silverChest.bmp", 936, 212, 6, 1);
	IMAGEMANAGER->addFrameImage("GoldChest", "Images/npc/goldChest.bmp", 936, 226, 6, 1);

	IMAGEMANAGER->addImage("iceSpearSkill", "Images/item/iceSpearSkill.bmp", 32, 49, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("dragonSkill", "Images/item/dragonSkill.bmp", 32, 49, true, RGB(255, 0, 255));

	keyName = _keyName;

	coin = 100;
	hp = _hp;
	pos = _pos;
	frameX = 0;

	image* img = IMAGEMANAGER->findImage(keyName);
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());

	string tmp[2][3] = { { "iceSpearSkill","skill_iceSpear","360" },{"dragonSkill","skill_dragonArc","300"} };

	int ran = RANDOM->range(0, 1);

	switch (ran)
	{
	case 0:
		arcanaKeyName = tmp[0][0];
		arcanaName = tmp[0][1];
		arcanaCoolTime = stoi(tmp[0][2]);
		break;
	case 1:
		arcanaKeyName = tmp[1][0];
		arcanaName = tmp[1][1];
		arcanaCoolTime = stoi(tmp[1][2]);
		break;
	}

	UNITRENDER->addUnit(255, _keyName, "chest", { 0,0 }, _pos.x, _pos.y);
	
	hitCount = 0;

	time = 0;
	opacity = 255;
	isDie = false;
	return S_OK;
}

void chest::update()
{
	hitCount++;
	
	image* img = IMAGEMANAGER->findImage(keyName);
	if (hp <= 0) isDie = true;

	if (isDie)
	{
		time++;
		if (frameX != img->getMaxFrameX())
		{
			if (time % 5 == 0)
			{
				if (frameX == 2)
				{
					POINT rePos = { pos.x - 30, pos.y - 30 };
					//아이템, 동전 생성
					DROP->dropPoint(rePos, 80, 120, 0, { -100,100 }, { -80,80 });
					DROP->dropPointArcana(arcanaKeyName, rePos, arcanaName, arcanaCoolTime);

					//포탈 생성
					PORTAL->initWarp(-1149, 2247);
					EFFECT->setPortalEffect({ (long)PORTAL->getWarpSceneX(),(long)PORTAL->getWarpSceneY() - 30 });
					SOUNDMANAGER->play("portalWarp", false);
				}

				frameX++;
				UNITRENDER->setFrameChest({ frameX,0 });
			}
		}

		else
		{
			if(opacity > 0) opacity -= 17;

			UNITRENDER->setIndexChest(opacity);
		}
	}
}

void chest::bossScene_update()
{
	hitCount++;

	image* img = IMAGEMANAGER->findImage(keyName);
	if (hp <= 0) isDie = true;

	if (isDie)
	{
		time++;
		if (frameX != img->getMaxFrameX())
		{
			if (time % 5 == 0)
			{
				if (frameX == 2)
				{
					POINT rePos = { pos.x - 30, pos.y - 30 };
					//아이템, 동전 생성
					DROP->dropPoint(rePos, 80, 120, 0, { -100,100 }, { -80,80 });
					//DROP->dropPointArcana(arcanaKeyName, rePos, arcanaName, arcanaCoolTime);

					//포탈 생성
					PORTAL->initWarp(746, 543);
					EFFECT->setPortalEffect({ (long)PORTAL->getWarpSceneX(),(long)PORTAL->getWarpSceneY() - 30 });
					SOUNDMANAGER->play("portalWarp", false);
				}

				frameX++;
				//UNITRENDER->setFrameChest({ frameX,0 });
			}
		}

		else
		{
			if (opacity > 0) opacity -= 17;

			UNITRENDER->setIndexChest(opacity);
		}
	}
}

void chest::damaged(POINT pt, int value, int skillNum ,bool criCheck)
{

	switch (skillNum)
	{
	case 0:
		if(hitCount%2==0)
			hp -= value;
		break;
	case 1:
		if (hitCount % 10 == 0)
			hp -= value;
		break;
	case 2:
		if (hitCount % 15 == 0)
			hp -= value;
		break;
	case 3:
		if (hitCount % 20 == 0)
			hp -= value;
		break;
	case 4:
		if (hitCount % 7 == 0)
			hp -= value;
		break;
	case 5:
		if (hitCount % 15 == 0)
			hp -= value;
		break;
	}

	DAMAGE->generator(pt, "numbers", value, true, criCheck);
}

void chest::render(HDC hdc)
{
	//IMAGEMANAGER->findImage(keyName);
	CAMERAMANAGER->AlphaFrameRender(hdc, IMAGEMANAGER->findImage(keyName),
		pos.x - IMAGEMANAGER->findImage(keyName)->getFrameWidth()/2,
		pos.y - IMAGEMANAGER->findImage(keyName)->getFrameHeight()/2,
		frameX, 0, opacity);
	//CAMERAMANAGER->Rectangle(hdc, rc);
}