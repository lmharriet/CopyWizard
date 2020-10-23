#include "stdafx.h"
#include "ghoulLarge.h"

void ghoulLarge::addInit()
{
    kind = MONSTERKIND::GHOULLARGE;
    atk = 30;
    armour = 0.55f;
    speed = 3.5f;
    hp = 200;
    img = IMAGEMANAGER->findImage("ghoulLarge");
    isKnockBack = false;
    isRanger = false;

    speedUpCount = 0;
    isSpeedUp = false;
    isIdle = false;
    isNoHit = true;
    isDash = false;



    //smallSlashIndex = { 0,0 };
    //isAtkImgCount = false;
    //fxCount = 0;
}

void ghoulLarge::update()
{
    if (isFindWayOn && !isIceState) //길찾기 on
    {
        if (!isATK)
        {
            RECT astarRC = RectMake(pos.x + 10, pos.y - (img->getFrameHeight()>>1), img->getFrameWidth(), img->getFrameHeight());
            astar->update(camRC, astarRC, playerRC, &angle);  // 에이스타에서 앵글 받아옴
            if (astar->getFirstTile() && !isDie) // 걸을 때
            {
                state = STATEIMAGE::WALK;
                pos.x += cos(angle) * speed;
                pos.y += -sin(angle) * speed;
                if (rc.left + (img->getFrameWidth() >> 1) < playerRC.left)
                {
                    atkDirection[MONSTER_LEFT] = false;
                    atkDirection[MONSTER_RIGHT] = true;
                    isLeft = false;
                }
                else
                {
                    isLeft = true;
                    atkDirection[MONSTER_LEFT] = true;
                    atkDirection[MONSTER_RIGHT] = false;
                }
                if (rc.top + (img->getFrameHeight() >> 1) > playerRC.top)
                {

                    atkDirection[MONSTER_UP] = true;
                    atkDirection[MONSTER_DOWN] = false;
                }
                else
                {

                    atkDirection[MONSTER_UP] = false;
                    atkDirection[MONSTER_DOWN] = true;
                }
            }
            else if (!isDie )
            {
                state = STATEIMAGE::ATK;
                isATK = true;
                SOUNDMANAGER->play("golemAtk", false);
            }
        }

    }
    else//길찾기 off
    {
        //if(!isHit)
        state = STATEIMAGE::IDLE;
    }

    if (isATK && isDash)
    {
        
        if (!wallCol())
        {
            pos.x += cos(angle) * speed * 3.0f;
            pos.y += -sin(angle) * speed * 3.0f;
        }
    }


}

void ghoulLarge::addRender()
{
    if (isIceState)
        CAMERAMANAGER->AlphaRender(getMemDC(), IMAGEMANAGER->findImage("IceState"), pos.x+20, pos.y-30, 160);
}

void ghoulLarge::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        stateIDLE();
        break;
    case STATEIMAGE::WALK:
        stateWalk();
        break;
    case STATEIMAGE::ATK:
        stateATK();
        break;
    case STATEIMAGE::DIE:
        stateDIE();
        break;
    }
}

void ghoulLarge::stateIDLE()
{
    isATK = false;

    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::IDLE].x = 5;
        frameIndexL[STATEIMAGE::IDLE].y = 0;
        frameIndexL[STATEIMAGE::WALK].x = 5;
        frameIndexL[STATEIMAGE::WALK].y = 1;
        frameIndexL[STATEIMAGE::ATK].x = 5;
        frameIndexL[STATEIMAGE::ATK].y = 7;
        frameIndexL[STATEIMAGE::DIE].x = 5;
        frameIndexL[STATEIMAGE::DIE].y = 9;

        currentFrame = { frameIndexL[STATEIMAGE::IDLE].x,frameIndexL[STATEIMAGE::IDLE].y };
    }
    else
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 0;
        frameIndexR[STATEIMAGE::WALK].x = 0;
        frameIndexR[STATEIMAGE::WALK].y = 0;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 6;
        frameIndexR[STATEIMAGE::DIE].x = 0;
        frameIndexR[STATEIMAGE::DIE].y = 8;

        currentFrame = { frameIndexR[STATEIMAGE::IDLE].x,frameIndexR[STATEIMAGE::IDLE].y };
    }
    bulletDirection[MONSTER_DOWN] = false;
    bulletDirection[MONSTER_UP] = false;
    bulletDirection[MONSTER_LEFT] = false;
    bulletDirection[MONSTER_RIGHT] = false;
    isFxAppear = false;
}

void ghoulLarge::stateATK()
{
    char str[50] = "ghoulATK";
    float soundVolum = 0.0f;


    if (atkDirection[MONSTER_UP] && playerRC.left > rc.left && playerRC.right < rc.right + 40)
    {
        if (delay == 0 && (frameIndexL[STATEIMAGE::ATK].x == 2 || frameIndexR[STATEIMAGE::ATK].x == 3))
        {
            //EFFECT->setEffect("knightSlashUp", { pos.x + 60 ,pos.y + 20 }, true);
            SOUNDMANAGER->play(str, false, soundVolum); // 사운드수정예정
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_UP] = true;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            isFxAppear = true;
            isDash = true;
            delay++;
        }
    }

    else if (atkDirection[MONSTER_DOWN] && playerRC.left > rc.left && playerRC.right < rc.right + 80)
    {
        if (delay == 0 && (frameIndexL[STATEIMAGE::ATK].x == 2 || frameIndexR[STATEIMAGE::ATK].x == 3))
        {
            //EFFECT->setEffect("knightSlashDown", { pos.x + 70,pos.y + 150 }, true);
            SOUNDMANAGER->play(str, false, soundVolum);
            bulletDirection[MONSTER_DOWN] = true;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            isFxAppear = true;
            isDash = true;
            delay++;
        }
    }
    if (delay == 0 && frameIndexL[STATEIMAGE::ATK].x == 2)
    {
        //EFFECT->setEffect("knightSlashL", { pos.x  ,pos.y + 80 }, true);
        SOUNDMANAGER->play(str, false, soundVolum);
        bulletDirection[MONSTER_DOWN] = false;
        bulletDirection[MONSTER_UP] = false;
        bulletDirection[MONSTER_LEFT] = true;
        bulletDirection[MONSTER_RIGHT] = false;
        isFxAppear = true;
        isDash = true;
        delay++;
    }
    else if (delay == 0 && frameIndexR[STATEIMAGE::ATK].x == 3)
    {
        //EFFECT->setEffect("knightSlashR", { pos.x + 110,pos.y + 80 }, true);
        SOUNDMANAGER->play(str, false, soundVolum);
        bulletDirection[MONSTER_DOWN] = false;
        bulletDirection[MONSTER_UP] = false;
        bulletDirection[MONSTER_LEFT] = false;
        bulletDirection[MONSTER_RIGHT] = true;
        isFxAppear = true;
        isDash = true;
        delay++;
    }

    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::ATK].y = 7;
        count++;
        if (count % 6 == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::ATK].x--;

            if (frameIndexL[STATEIMAGE::ATK].x < 0)
            {
                frameIndexL[STATEIMAGE::ATK].x = 0;
                delay++;
                    isDash = false;
                if (delay > 5)
                {
                    isATK = false;
                    isFxAppear = false;
                    delay = 0;
                    frameIndexL[STATEIMAGE::ATK].x = 5;
                }
            }
        }
        currentFrame = { frameIndexL[STATEIMAGE::ATK].x ,frameIndexL[STATEIMAGE::ATK].y };
    }
    else
    {
        frameIndexR[STATEIMAGE::ATK].y = 6;
        count++;
        if (count % 6 == 0)
        {
            count = 0;
            frameIndexR[STATEIMAGE::ATK].x++;

            if (frameIndexR[STATEIMAGE::ATK].x > 5)
            {
                frameIndexR[STATEIMAGE::ATK].x = 5;
                delay++;
                    isDash = false;
                if (delay > 5)
                {
                    isATK = false;
                    isFxAppear = false;
                    delay = 0;
                    frameIndexR[STATEIMAGE::ATK].x = 0;
                }
            }

        }
        currentFrame = { frameIndexR[STATEIMAGE::ATK].x ,frameIndexR[STATEIMAGE::ATK].y };
    }
}

void ghoulLarge::stateDIE()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::DIE].y = 9;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::DIE].x--;
            if (frameIndexL[STATEIMAGE::DIE].x < 0)
            {
                frameIndexL[STATEIMAGE::DIE].x = 0;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;

                }
            }
        }
        currentFrame = { frameIndexL[STATEIMAGE::DIE].x ,frameIndexL[STATEIMAGE::DIE].y };
    }
    else
    {
        frameIndexR[STATEIMAGE::DIE].y = 8;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexR[STATEIMAGE::DIE].x++;
            if (frameIndexR[STATEIMAGE::DIE].x > 5)
            {
                frameIndexR[STATEIMAGE::DIE].x = 5;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
        }
        currentFrame = { frameIndexR[STATEIMAGE::DIE].x ,frameIndexR[STATEIMAGE::DIE].y };
    }

    coinDrop(5, 40);
}

void ghoulLarge::stateWalk()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::ATK].x = 5;
        frameIndexL[STATEIMAGE::ATK].y = 7;

        frameIndexL[WALK].y = 1;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexL[WALK].x--;
            if (frameIndexL[WALK].x < 0)
            {
                SOUNDMANAGER->play("golemWalkL", false, -0.2f);
                frameIndexL[WALK].x = 5;
            }
        }
        currentFrame = { frameIndexL[WALK].x ,frameIndexL[WALK].y };
    }
    else
    {
        frameIndexL[STATEIMAGE::ATK].x = 0;
        frameIndexL[STATEIMAGE::ATK].y = 6;
        frameIndexR[WALK].y = 0;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexR[WALK].x++;
            if (frameIndexR[WALK].x > 5)
            {
                SOUNDMANAGER->play("golemWalkR", false, -0.2f);
                frameIndexR[WALK].x = 0;
            }
        }
        currentFrame = { frameIndexR[WALK].x ,frameIndexR[WALK].y };
    }
}
