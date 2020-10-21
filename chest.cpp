#include "stdafx.h"
#include "chest.h"

HRESULT chest::init(string _keyName, POINT _pos, int _hp)
{
	IMAGEMANAGER->addFrameImage("silverChest", "Images/npc/silverChest.bmp", 936, 212, 6, 1);

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

	UNITRENDER->addUnit(255, "silverChest", "chest", { 0,0 }, _pos.x, _pos.y);
	
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
					//������, ���� ����
					DROP->dropPoint(rePos, 80, 120, 0, { -100,100 }, { -80,80 });
					DROP->dropPointArcana(arcanaKeyName, rePos, arcanaName, arcanaCoolTime);
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
