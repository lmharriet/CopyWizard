#include "stdafx.h"
#include "summoner.h"



void summoner::addInit()
{
    atkTime = 0;
    randomTime = RANDOM->range(90, 120);
    isCasting = false;
    skillImg = IMAGEMANAGER->findImage("stoneFly");

    kind = MONSTERKIND::SUMMONER;
    atk = 15;
    armour = 0;
    speed = 0.f;
    hp = 80;
    img = IMAGEMANAGER->findImage("summoner");
    
    imgSize = 0.f;
    isKnockBack = true;
    isRanger = true;
    isCharging = false;
    fireCount = 0;
}


void summoner::update()
{
    if(isFindWayOn)
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
        PARTICLE->collectingGenerate("stoneX2", pos.x + 55, pos.y - 45, 18, 60.f, 2.f, 50, 20, 5);
        fireCount = atkTime-1 ;
        
    }
    
        
    if (isATK&& !isHit && !isFxAppear )
    {
        fireCount++;
        
        if (fireCount % (randomTime+70)==0)
        {
            randomTime = RANDOM->range(90, 150);
            atkTime = 0;
            fireCount =0;
            isFxAppear = true; // 불렛 발사 시점.
            
        }
    }
}

void summoner::render()
{
    if (isATK)
    {
        if (!isCharging)
        {

            SOUNDMANAGER->play("summonerCasting", false, -0.15f);
            isCharging = true;
        }
        if (!isCasting/*&&!isFxAppear*/)
        {

            imgSize += 0.02f;
            if (imgSize > 1.0f)
            {
                imgSize = 1.0f;

            }

            CAMERAMANAGER->StretchFrameRender(getMemDC(), skillImg, pos.x + 20, pos.y - 72, 0, 0, imgSize);
        }
    }
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
        imgSize = 0.f;
        atkTime = 0;
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
        
        currentFrame = { frameIndexL[STATEIMAGE::IDLE].x ,frameIndexL[STATEIMAGE::IDLE].y};
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
    }
    else if(atkDirection[MONSTER_RIGHT])
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 2;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 2;
        frameIndexR[STATEIMAGE::DIE].x = 0;
        frameIndexR[STATEIMAGE::DIE].y = 4;

        currentFrame = { frameIndexR[STATEIMAGE::IDLE].x ,frameIndexR[STATEIMAGE::IDLE].y };
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
    }
   
}

void summoner::stateATK()
{
    
   
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
            if (frameIndexL[STATEIMAGE::ATK].y == 1 && frameIndexL[STATEIMAGE::ATK].x == 2)
                 isCasting = true;

            if (frameIndexL[STATEIMAGE::ATK].y == 1 && frameIndexL[STATEIMAGE::ATK].x > 4)
            {
                frameIndexL[STATEIMAGE::ATK].x = 4;
                delay++;
               
                if (delay > 3)
                {
                    isATK = false;
                    isCasting = false;
                    isCharging = false;
                    imgSize =0.f;
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
        currentFrame = { frameIndexL[STATEIMAGE::ATK].x ,frameIndexL[STATEIMAGE::ATK].y };
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::ATK].x, frameIndexL[STATEIMAGE::ATK].y);
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

                if (frameIndexR[STATEIMAGE::ATK].x == 2)
                    isCasting = true;
             
                if (frameIndexR[STATEIMAGE::ATK].x < 0)
                {
                    frameIndexR[STATEIMAGE::ATK].x = 0;
                    delay++;
                  
                    if (delay > 3)
                    {
                        isATK = false;
                        isCasting = false;
                        isCharging = false;
                        delay = 0;
                        imgSize = 0.f;
                        frameIndexR[STATEIMAGE::ATK].x = 0;
                        frameIndexR[STATEIMAGE::ATK].y = 2;
                    }
                }
            }
        }
        currentFrame = { frameIndexR[STATEIMAGE::ATK].x ,frameIndexR[STATEIMAGE::ATK].y };
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::ATK].x, frameIndexR[STATEIMAGE::ATK].y);
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
        currentFrame = { frameIndexL[STATEIMAGE::DIE].x ,frameIndexL[STATEIMAGE::DIE].y };
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
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
        currentFrame = { frameIndexR[STATEIMAGE::DIE].x ,frameIndexR[STATEIMAGE::DIE].y };
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    }

    
    coinDrop(30, 50);
}



