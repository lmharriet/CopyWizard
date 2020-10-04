#include "stdafx.h"
#include "dropManager.h"

HRESULT dropManager::init()
{
    IMAGEMANAGER->addFrameImage("coin", "Images/item/coin.bmp", 40, 20, 4, 2);
    return S_OK;
}

void dropManager::update()
{
    time++;

    coinGenerator();
}

void dropManager::render(HDC hdc)
{
    vector<tagCoin>::iterator iter = vCoin.begin();
    for (iter; iter != vCoin.end(); ++iter)
    {
        if (time % 5 == 0)iter->currentFrameX++;

        if (iter->currentFrameX == IMAGEMANAGER->findImage("coin")->getMaxFrameX())iter->currentFrameX = 0;

        CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage("coin"), iter->pt.x, iter->pt.y, iter->currentFrameX, iter->frameY);
    }
}

void dropManager::dropPoint(POINT pt, int minCoin, int maxCoin)
{
    int ranCoin = RANDOM->range(minCoin, maxCoin);

    vTransfer.push_back({ ranCoin / 5,ranCoin % 5,pt });
    cout << "pt" << '\n';
}

void dropManager::coinGenerator()
{
    vector<infoTransfer>::iterator iter = vTransfer.begin();
    
    for (iter; iter != vTransfer.end(); ++iter)
    {
        for (int i = 0; i < iter->gCoin; i++)
        {
            POINT pos = iter->pt;
            pos.x += RANDOM->range(-20, 20);
            pos.y += RANDOM->range(-10, 10);

            vCoin.push_back({ pos,RectMakeCenter(pos.x,pos.y,10,10),5,0,1 });
        }

        for (int j = 0; j < iter->sCoin; j++)
        {
            POINT pos = iter->pt;
            pos.x += RANDOM->range(-20, 20);
            pos.y += RANDOM->range(-10, 10);

            vCoin.push_back({ pos,RectMakeCenter(pos.x,pos.y,10,10),1,0,0 });
        }


        vTransfer.pop_back();
        break;
    }
}