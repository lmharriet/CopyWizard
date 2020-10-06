#include "stdafx.h"
#include "summoner.h"



void summoner::addInit()
{
    atkTime = 0;

}


void summoner::update()
{
    rc = RectMake(cul.x, cul.y, img->getFrameWidth(), img->getFrameHeight());

    if (distanceMax > getDistance(pos.x, pos.y, playerRC.left + (playerRC.right - playerRC.left) / 2, playerRC.top + (playerRC.bottom - playerRC.top) / 2))
        isFindWayOn = true;
    else
        isFindWayOn = false;

    atkTime++;
    if (!isATK && !isDie) 
    {
        state = STATEIMAGE::IDLE;
        
        if ((rc.left + (rc.right-rc.left) / 2) < (playerRC.left + (playerRC.right - playerRC.left) / 2))
        {
            isLeft = false;
        }
        else
            isLeft = true;
    }

    if (!isDie && atkTime%150==0 && isFindWayOn )
    {
        atkTime = 0;
        state = STATEIMAGE::ATK;
        isATK = true;
        isFxAppear = true;
        
    }
    
    

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);

    die();
}


void summoner::render()
{
    FrameRect(getMemDC(), rc, RGB(255, 255, 255));
    FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
    stateImageRender();
}
void summoner::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        stateIDLE();
        break;
    case STATEIMAGE::ATK:
        stateATK();
        break;
    case STATEIMAGE::DIE:
        stateDIE();
        break;

    }
}

void summoner::stateIDLE()
{
    if (isLeft)
    {
        frameIndexL[STATEIMAGE::IDLE].x = 0;
        frameIndexL[STATEIMAGE::IDLE].y = 0;
        frameIndexL[STATEIMAGE::ATK].x = 0;
        frameIndexL[STATEIMAGE::ATK].y = 0;
        frameIndexL[STATEIMAGE::DIE].x = 4;
        frameIndexL[STATEIMAGE::DIE].y = 6;
        
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 2;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 2;
        frameIndexR[STATEIMAGE::DIE].x = 0;
        frameIndexR[STATEIMAGE::DIE].y = 4;
    }
    img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
}

void summoner::stateATK()
{

    if (isLeft)
    {
        int atkTime = 10;

        if (frameIndexL[STATEIMAGE::ATK].y != 1)
            frameIndexL[STATEIMAGE::ATK].y = 0;
        count++;
        if (count % atkTime == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::ATK].x++;
            if (frameIndexL[STATEIMAGE::ATK].y == 1 && frameIndexL[STATEIMAGE::ATK].x > 4)
            {
                frameIndexL[STATEIMAGE::ATK].x = 4;
                delay++;
                if (delay > 3)
                {
                    isATK = false;
                    isBulletFire = true;
                    delay = 0;
                    frameIndexL[STATEIMAGE::ATK].x = 0;
                    frameIndexL[STATEIMAGE::ATK].y = 0;
                }
            }
            else if (frameIndexL[STATEIMAGE::ATK].x > 4)
            {
                frameIndexL[STATEIMAGE::ATK].x = 0;
                frameIndexL[STATEIMAGE::ATK].y = 1;
            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::ATK].x, frameIndexL[STATEIMAGE::ATK].y);
    }
    else
    {
        int atkTime = 10;
        if (frameIndexR[STATEIMAGE::ATK].y != 3)
            frameIndexR[STATEIMAGE::ATK].y = 2;

        count++;
        if (count % atkTime == 0)
        {
            count = 0;
            if (frameIndexR[STATEIMAGE::ATK].y == 2)
            {
                frameIndexR[STATEIMAGE::ATK].x++;
                if (frameIndexR[STATEIMAGE::ATK].x > 4)
                {
                    frameIndexR[STATEIMAGE::ATK].x = 4;
                    frameIndexR[STATEIMAGE::ATK].y = 3;
                }
            }

            else
                frameIndexR[STATEIMAGE::ATK].x--;
            if (frameIndexR[STATEIMAGE::ATK].x < 0)
            {
                frameIndexR[STATEIMAGE::ATK].x = 0;
                delay++;
                    isBulletFire = true;
                if (delay > 3)
                {
                    isATK = false;
                    delay = 0;
                    frameIndexR[STATEIMAGE::ATK].x = 0;
                    frameIndexR[STATEIMAGE::ATK].y = 2;
                }
            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::ATK].x, frameIndexR[STATEIMAGE::ATK].y);
    }
}

void summoner::stateDIE()
{
    if (isLeft)
    {
        int dieFrameSpeed = 10;
        if (frameIndexL[STATEIMAGE::DIE].y != 5)
            frameIndexL[STATEIMAGE::DIE].y = 6;

        count++;
        if (count % dieFrameSpeed == 0)
        {
            count = 0;
            if (frameIndexL[STATEIMAGE::DIE].y == 6)
            {
                frameIndexL[STATEIMAGE::DIE].x--;

                if (frameIndexL[STATEIMAGE::DIE].x < 0)
                {
                    frameIndexL[STATEIMAGE::DIE].y = 5;
                    frameIndexL[STATEIMAGE::DIE].x = 3;
                }
            }
            else
            {
                frameIndexL[STATEIMAGE::DIE].x++;
                if (frameIndexL[STATEIMAGE::DIE].x > 4)
                {
                    frameIndexL[STATEIMAGE::DIE].x = 4;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
            }

        }
    }
    else
    {
        int dieFrameSpeed = 10;
        if (frameIndexR[STATEIMAGE::DIE].y != 5)
            frameIndexR[STATEIMAGE::DIE].y = 4;

        count++;
        if (count % dieFrameSpeed == 0)
        {
            count = 0;

            frameIndexR[STATEIMAGE::DIE].x++;

            if (frameIndexR[STATEIMAGE::DIE].y == 5 && frameIndexR[STATEIMAGE::DIE].x > 1)
            {
                frameIndexR[STATEIMAGE::DIE].x = 1;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
            else if (frameIndexR[STATEIMAGE::DIE].x > 4)
            {
                frameIndexR[STATEIMAGE::DIE].y = 5;
                frameIndexR[STATEIMAGE::DIE].x = 0;
            }
        }
    }

    img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    
    coinDrop(1, 10);
}



