#include "stdafx.h"
#include "dropManager.h"

HRESULT dropManager::init()
{
    IMAGEMANAGER->addFrameImage("coin", "Images/item/coinV3.bmp", 80, 60, 4, 3);
    IMAGEMANAGER->addFrameImage("healBall", "Images/item/healing ball.bmp", 300, 50, 6, 1);

    IMAGEMANAGER->addImage("bronzeCoin", "Images/effect/bronzeCoin.bmp", 21, 12, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("silverCoin", "Images/effect/silverCoin.bmp", 21, 12, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("goldCoin", "Images/effect/goldCoin.bmp", 21, 12, true, RGB(255, 0, 255));

    dtime = 0;
    cTime = 0;
    return S_OK;
}

void dropManager::update()
{
    coinGenerator();
}

void dropManager::render(HDC hdc)
{
    dtime++;

    //coin render
    image* img = IMAGEMANAGER->findImage("coin");
    vector<tagCoin>::iterator iter = vCoin.begin();
    for (iter; iter != vCoin.end(); ++iter)
    {
        //플레이어 쪽으로 이동하기
        float angle = getAngle(iter->pt.x, iter->pt.y, PLAYERDATA->getX(), PLAYERDATA->getY());

        iter->pt.x += cosf(angle) * iter->speed;
        iter->pt.y += -sinf(angle) * iter->speed;

        //rc 그리기
        iter->rc = RectMakeCenter(iter->pt.x, iter->pt.y, 20, 20);

        CAMERAMANAGER->FrameRender(hdc, img, 
            iter->pt.x - img->getFrameWidth() / 2, 
            iter->pt.y - img->getFrameHeight() / 2,
            iter->currentFrameX, iter->frameY);

        if(iter->speed < 10.f) iter->speed *= 1.06f;

        if (dtime % 8 == 0)
        {
            iter->currentFrameX++;
        }

        if (iter->currentFrameX == IMAGEMANAGER->findImage("coin")->getMaxFrameX())iter->currentFrameX = 0;
       // CAMERAMANAGER->Ellipse(hdc, iter->rc);
    }
   
    //ball render
    img = IMAGEMANAGER->findImage("healBall");
    for (int i = 0; i < vHealBall.size(); i++)
    {
        CAMERAMANAGER->FrameRender(hdc, img,
            vHealBall[i].pt.x - img->getFrameWidth() / 2,
            vHealBall[i].pt.y - img->getFrameHeight() / 2,
            vHealBall[i].currentFrameX, 0);

        if (dtime % 12 == 0)
        {
            vHealBall[i].currentFrameX++;
        }

        if (vHealBall[i].currentFrameX == img->getMaxFrameX())vHealBall[i].currentFrameX = 0;
    }
}

void dropManager::cardRender(HDC hdc)
{
    //arcana card render
    int size = vArcana.size();
    for (int i = 0; i < size; i++)
    {
        if (vArcana[i].size < 0.96f)vArcana[i].size += 0.05f;
        CAMERAMANAGER->StretchRender(hdc, IMAGEMANAGER->findImage(vArcana[i].keyName),
            vArcana[i].pos.x, vArcana[i].pos.y, vArcana[i].size);
    }
}

void dropManager::coinEffectRender(HDC hdc)
{
    //하나씩 전달
    if (saveCoinView.size() > 0)
    {
        cTime++;

        if (cTime % 10 == 0)
        {
            coinView.push_back(saveCoinView[0]);
            saveCoinView.erase(saveCoinView.begin());
            cTime = 0;
        }
    }

    //coin drop render
    for (int i = 0; i < coinView.size();)
    {
        //렌더
        image* img = IMAGEMANAGER->findImage(coinView[i].keyName);
        img->alphaRender(hdc, coinView[i].pt.x, coinView[i].pt.y, coinView[i].curOpacity);

        //이동
        coinView[i].pt.y -= 2;

        //투명도 변경
        coinView[i].curOpacity-=5;

        //삭제
        if (coinView[i].curOpacity == 0.f)
        {
            coinView.erase(coinView.begin() + i);
        }
        else i++;
    }
}

void dropManager::dropPoint(POINT pt, int minCoin, int maxCoin, float healBallpercentage, POINT rangeX, POINT rangeY)
{
    //코인 생성
    int ranCoin = RANDOM->range(minCoin, maxCoin);

    //gcoin = 10단위
    //scoin = 5단위
    //bcoin = 1단위
    int gCoin = ranCoin / 10;
    ranCoin -= (gCoin * 10);
    int sCoin = ranCoin / 5;
    ranCoin -= (sCoin * 5);
    int bCoin = ranCoin;

    vTransfer.push_back({ gCoin,sCoin,bCoin,pt,(float)rangeX.x,(float)rangeX.y,(float)rangeY.x,(float)rangeY.y });

    //포션 생성 (후에 플레이어 포션 드랍율을 매개변수로 받아와 포션 드랍 유/무를 정한다)
    float sum = 0.1f + PLAYERDATA->getStat().potionDropChance;
    //최대 100%
    if (sum > 1.0f)sum = 1.0f;

    int persentage = sum * 100;

    if (checkPercentage(persentage))
    {
        tagHealBall ball;
        ball.pt = pt;
        ball.heal = 50;
        ball.currentFrameX = 0;
        ball.rc = RectMakeCenter(pt.x, pt.y, 50, 50);
        vHealBall.push_back(ball);
    }
}

void dropManager::dropPointArcana(string keyName, POINT pt, string arcanaName, int coolTime)
{
    tagDropArcana arcana;
    arcana.keyName = keyName;
    arcana.pos = pt;
    image* img = IMAGEMANAGER->findImage(keyName);
    arcana.rc = RectMakeCenter(pt.x - img->getWidth() / 2,
        pt.y - img->getHeight() / 2, 
        img->getWidth(),
        img->getHeight());
    arcana.arcanaName = arcanaName;
    arcana.size = 0;
    arcana.coolTime = coolTime;

    vArcana.push_back(arcana);
}

bool dropManager::checkPercentage(int persentage)
{
    int ran = RANDOM->range(1, 100);

    if (ran <= persentage) return true;
    
    return false;
}

void dropManager::coinGenerator()
{
    vector<infoTransfer>::iterator iter = vTransfer.begin();
    
    for (iter; iter != vTransfer.end();)
    {
        for (int i = 0; i < iter->gCoin; i++)
        {
            POINT pos = iter->pt;
            pos.x += RANDOM->range(iter->minX, iter->maxX);
            pos.y += RANDOM->range(iter->minY, iter->maxY);

            vCoin.push_back({ pos,RectMakeCenter(pos.x,pos.y,20,20),10,
            0.2f,
            0,2 });
        }

        for (int j = 0; j < iter->sCoin; j++)
        {
            POINT pos = iter->pt;
            pos.x += RANDOM->range(iter->minX, iter->maxX);
            pos.y += RANDOM->range(iter->minY, iter->maxY);

            vCoin.push_back({ pos,RectMakeCenter(pos.x,pos.y,20,20),5,
            0.2f,
            0,1 });
        }       
        
        for (int j = 0; j < iter->bCoin; j++)
        {
            POINT pos = iter->pt;
            pos.x += RANDOM->range(iter->minX, iter->maxX);
            pos.y += RANDOM->range(iter->minY, iter->maxY);

            vCoin.push_back({ pos,RectMakeCenter(pos.x,pos.y,20,20),1,
            0.2f,
            0,0 });
        }


        iter = vTransfer.erase(iter);
    }
}

void dropManager::getCoinEffect(int money)
{
    tagCoinView cView;

    cView.curOpacity = 255;
    cView.speed = 3.f;
    cView.pt = { WINSIZEX / 2 - 22,WINSIZEY - 50 };

    switch (money)
    {
    case 1:
        cView.keyName = "bronzeCoin";
        break;
    case 5:
        cView.keyName = "silverCoin";
        break;
    case 10:
        cView.keyName = "goldCoin";
        break;
    }

    saveCoinView.push_back(cView);
}