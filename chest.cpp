#include "stdafx.h"
#include "chest.h"

HRESULT chest::init(string _keyName, POINT _pos, int _hp)
{
	IMAGEMANAGER->addFrameImage("silverChest", "Images/npc/silverChest.bmp", 936, 212, 6, 1);

	keyName = _keyName;

	coin = 100;
	hp = _hp;
	pos = _pos;
	frameX = 0;

	image* img = IMAGEMANAGER->findImage(keyName);
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());

	UNITRENDER->addUnit(0, "silverChest", "chest", { 0,0 }, _pos.x, _pos.y);
	
	hitCount = 0;

	time = 0;
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
					//아이템, 동전 생성
					DROP->dropPoint(pos, 80, 120, 0);
				}

				frameX++;
				UNITRENDER->setFrameChest({ frameX,0 });
			}
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
