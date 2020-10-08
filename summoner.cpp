#include "stdafx.h"
#include "summoner.h"



void summoner::addInit()
{
    atkTime = 0;
    randomTime = RANDOM->range(90, 120);
    isCasting = false;
    skillImg = IMAGEMANAGER->findImage("stoneFly");

    kind = MONSTERKIND::SUMMONER;
    atk = 30;
    armour = 10;
    speed = 0.f;
    hp = 50;
    img = IMAGEMANAGER->findImage("summoner");
    isKnockBack = true;
    isRanger = true;
}


void summoner::update()
{

    atkTime++;
    if (!isATK && !isDie && !isHit) 
    {
        state = STATEIMAGE::IDLE;
        
        if ((rc.left + (rc.right-rc.left) / 2) < playerRC.left)
        {
            atkDirection[MONSTER_LEFT] = false;
            atkDirection[MONSTER_RIGHT] = true;
        }
        else
        {
            atkDirection[MONSTER_LEFT] = true;
            atkDirection[MONSTER_RIGHT] = false;
        }
        if ((rc.top + (rc.bottom - rc.top) / 2) < playerRC.top)
        {
            atkDirection[MONSTER_UP] = false;
            atkDirection[MONSTER_DOWN] = true;
        }
        else
        {
            atkDirection[MONSTER_UP] = true;
            atkDirection[MONSTER_DOWN] = false;
        }
    }
    
    if (!isDie && atkTime%randomTime==0 && isFindWayOn && !isATK && !isHit)
    {
        state = STATEIMAGE::ATK;
        isATK = true;
    }
    
    if (isATK&& !isHit)
    {
        if (atkTime % (randomTime+30)==0)
        {
            randomTime = RANDOM->range(90, 120);
            atkTime = 0;
            isFxAppear = true;
        }
    }
}


void summoner::render()
{
   // FrameRect(getMemDC(), rc, RGB(255, 255, 255));
   // FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
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
    case STATEIMAGE::HIT:
        stateHIT({ 4,7 }, { 0,7 });
        break;

    }
}

void summoner::stateIDLE()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::IDLE].x = 0;
        frameIndexL[STATEIMAGE::IDLE].y = 0;
        frameIndexL[STATEIMAGE::ATK].x = 0;
        frameIndexL[STATEIMAGE::ATK].y = 0;
        frameIndexL[STATEIMAGE::DIE].x = 4;
        frameIndexL[STATEIMAGE::DIE].y = 6;
        
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
    }
    else if(atkDirection[MONSTER_RIGHT])
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 2;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 2;
        frameIndexR[STATEIMAGE::DIE].x = 0;
        frameIndexR[STATEIMAGE::DIE].y = 4;

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
    }
   
}

void summoner::stateATK()
{
    if (!isCasting)
        skillImg->frameRender(getMemDC(), cul.x + 20, cul.y - 72, 0, 0);
   
    if (atkDirection[MONSTER_LEFT])
    {
        int atkTime = 10;

        if (frameIndexL[STATEIMAGE::ATK].y != 1)
            frameIndexL[STATEIMAGE::ATK].y = 0;
        count++;
        if (count % atkTime == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::ATK].x++;
            if (frameIndexL[STATEIMAGE::ATK].y == 1 && frameIndexL[STATEIMAGE::ATK].x == 1)
                isCasting = true;

            if (frameIndexL[STATEIMAGE::ATK].y == 1 && frameIndexL[STATEIMAGE::ATK].x > 4)
            {
                frameIndexL[STATEIMAGE::ATK].x = 4;
                delay++;
               
                if (delay > 3)
                {
                    isATK = false;
                    isCasting = false;
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
            {
                frameIndexR[STATEIMAGE::ATK].x--;

                if (frameIndexR[STATEIMAGE::ATK].x == 3)
                    isCasting = true;
             
                if (frameIndexR[STATEIMAGE::ATK].x < 0)
                {
                    frameIndexR[STATEIMAGE::ATK].x = 0;
                    delay++;
                  
                    if (delay > 3)
                    {
                        isATK = false;
                        isCasting = false;
                        delay = 0;
                        frameIndexR[STATEIMAGE::ATK].x = 0;
                        frameIndexR[STATEIMAGE::ATK].y = 2;
                    }
                }
            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::ATK].x, frameIndexR[STATEIMAGE::ATK].y);
    }
   
}

void summoner::stateDIE()
{
    if (atkDirection[MONSTER_LEFT])
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
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
    }
    else if(atkDirection[MONSTER_RIGHT])
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
    img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    }

    
    coinDrop(1, 10);
}



