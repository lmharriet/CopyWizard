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

    time = 0;
    isDie = false;
    return S_OK;
}

void chest::update()
{
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