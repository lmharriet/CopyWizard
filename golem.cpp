#include "stdafx.h"
#include "golem.h"


void golem::addInit()
{
    smallSlashIndex = { 0,0 };
    hitImg = IMAGEMANAGER->findImage("golemHit");
    kind = MONSTERKIND::GOLEM;
    atk = 50;
    armour = 0.4f;
    speed = 2.f;
    hp = 150;
    img = IMAGEMANAGER->findImage("golem");
    skillImg = IMAGEMANAGER->findImage("smallSlash");
    isKnockBack = false;
    isRanger = false;
    isAtkImgCount = false;

    fxCount = 0;

}

void golem::update()
{
    //거리 계산해서 일정 거리 넘어가면 에이스타 작동안되게 하기.
   
    if (isFindWayOn) //길찾기 on
    {
        
        astar->update(camRC, rc, playerRC, &angle);  // 에이스타에서 앵글 받아옴
        if (astar->getFirstTile() && !isATK && !isDie ) // 걸을 때
        {
            state = STATEIMAGE::WALK;
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
            if (rc.left + img->getFrameWidth() / 2 < playerRC.left)
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
            if (rc.top + img->getFrameHeight() / 2 > playerRC.top)
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
        else if(!isDie &&!isATK)
        {
            state = STATEIMAGE::ATK;
            isATK = true;
            SOUNDMANAGER->play("golemAtk", false);
        }
       
    }
    else//길찾기 off
    { 
        //if(!isHit)
        state = STATEIMAGE::IDLE;
    }

   
   
   
}

void golem::addRender()
{

    if (!isFxAppear && !isAtkImgCount) // 스킬이미지
    {
        smallSlashIndex = { -1,0 };
        if (atkDirection[MONSTER_LEFT])
        {
            smallSlashIndex.x = 3;
        }
           
    }

    if (isAtkImgCount)
    {
        fxCount++;
        /* char str[50];
         sprintf(str, "knight_atk%d", RANDOM->range(4));
         SOUNDMANAGER->play(str, false);*/

        int frameSpeed = 8;

        if (atkDirection[MONSTER_UP] && atkDirection[MONSTER_LEFT] && playerRC.left > rc.left && playerRC.right < rc.right)
        {
            bulletDirection[MONSTER_UP] = true;
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            smallSlashIndex.y = 5;
            if (fxCount % frameSpeed == 0)
            {

                smallSlashIndex.x--;
                if (smallSlashIndex.x < 0)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_UP] = false;
                    fxCount = 0;
                }

            }
            //EFFECT->setEffect("samllSlash", { cul.x + 10, cul.y - 30 });
            skillImg->frameRender(getMemDC(), cul.x + 10, cul.y - 30, smallSlashIndex.x, smallSlashIndex.y);
        }
        else if (atkDirection[MONSTER_UP] && atkDirection[MONSTER_RIGHT] && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 40)
        {
            bulletDirection[MONSTER_UP] = true;
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            smallSlashIndex.y = 4;
            if (fxCount % frameSpeed == 0)
            {
                smallSlashIndex.x++;
                if (smallSlashIndex.x > 2)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_UP] = false;
                    fxCount = 0;
                }
            }

            skillImg->frameRender(getMemDC(), cul.x + 10, cul.y - 30, smallSlashIndex.x, smallSlashIndex.y);
        }

        else if (atkDirection[MONSTER_DOWN] && atkDirection[MONSTER_LEFT] && playerRC.left > rc.left && playerRC.right > rc.right - 30)
        {
            bulletDirection[MONSTER_DOWN] = true;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            smallSlashIndex.y = 3;
            if (fxCount % frameSpeed == 0)
            {
                smallSlashIndex.x--;
                if (smallSlashIndex.x < 0)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_DOWN] = false;
                    fxCount = 0;
                }
            }
            skillImg->frameRender(getMemDC(), cul.x + 10, cul.y + 90, smallSlashIndex.x, smallSlashIndex.y);
        }
        else if (atkDirection[MONSTER_DOWN] && atkDirection[MONSTER_RIGHT] && playerRC.left > rc.left + img->getFrameWidth() / 2 && playerRC.right < rc.right + 80)
        {
            bulletDirection[MONSTER_DOWN] = true;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            smallSlashIndex.y = 2;
            if (fxCount % frameSpeed == 0)
            {
                smallSlashIndex.x++;
                if (smallSlashIndex.x > 2)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_DOWN] = false;
                    fxCount = 0;
                }
            }
            skillImg->frameRender(getMemDC(), cul.x + 10, cul.y + 90, smallSlashIndex.x, smallSlashIndex.y);
        }
        else if (atkDirection[MONSTER_RIGHT])
        {
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = true;
            smallSlashIndex.y = 0;
            if (fxCount % frameSpeed == 0)
            {
                smallSlashIndex.x++;
                if (smallSlashIndex.x > 2)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_RIGHT] = false;
                    fxCount = 0;
                }

            }
            skillImg->frameRender(getMemDC(), cul.x + 80, cul.y + 20, smallSlashIndex.x, smallSlashIndex.y);
        }
        else
        {
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = true;
            bulletDirection[MONSTER_RIGHT] = false;
            smallSlashIndex.y = 1;
            if (fxCount % frameSpeed == 0)
            {
                
                smallSlashIndex.x--;
                if (smallSlashIndex.x < 0)
                {
                    isAtkImgCount = false;
                    bulletDirection[MONSTER_LEFT] = false;
                    fxCount = 0;
                }
            }
            skillImg->frameRender(getMemDC(), cul.x - 80, cul.y + 20, smallSlashIndex.x, smallSlashIndex.y);
        }

    }
}




void golem::stateImageRender()
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

void golem::stateIDLE()
{
    isATK = false;
    
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::IDLE].x = 5;
        frameIndexL[STATEIMAGE::IDLE].y = 0;
        frameIndexL[STATEIMAGE::WALK].x = 5;
        frameIndexL[STATEIMAGE::WALK].y = 2;
        frameIndexL[STATEIMAGE::ATK].x = 5;
        frameIndexL[STATEIMAGE::ATK].y = 0;
        frameIndexL[STATEIMAGE::DIE].x = 5;
        frameIndexL[STATEIMAGE::DIE].y = 4;

        currentFrame = { frameIndexL[STATEIMAGE::IDLE].x,frameIndexL[STATEIMAGE::IDLE].y };
        /*if (isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);*/
    }
    else
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 0;
        frameIndexR[STATEIMAGE::WALK].x = 0;
        frameIndexR[STATEIMAGE::WALK].y = 1;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 0;
        frameIndexR[STATEIMAGE::DIE].x = 0;
        frameIndexR[STATEIMAGE::DIE].y = 3;
        
        currentFrame = { frameIndexR[STATEIMAGE::IDLE].x,frameIndexR[STATEIMAGE::IDLE].y };

       /* if(isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);*/
    }

}

void golem::stateATK()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[ATK].y = 0;
        count++;
        if (count % 30 == 0)
        {
           // count = 0;
            frameIndexL[ATK].x--;
            if (frameIndexL[ATK].x == 3)
            {
                isFxAppear = true;
                isAtkImgCount = true;
            }
            if (frameIndexL[ATK].x < 3)
            {
                frameIndexL[ATK].x = 3;
                delay++;
                if (delay > 1)
                {
                    isATK = false;
                    delay = 0;
                    frameIndexL[ATK].x = 5;
                }
            }
        }
        currentFrame = { frameIndexL[ATK].x,frameIndexL[ATK].y };
       /* if(isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[ATK].x, frameIndexL[ATK].y);
        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[ATK].x, frameIndexL[ATK].y);*/
    }
    else
    {
        frameIndexR[ATK].y = 0;
        count++;
        if (count % 30 == 0)
        {
            //count = 0;
            frameIndexR[ATK].x++;
            if (frameIndexR[ATK].x == 2)
            {
                isFxAppear = true;
                isAtkImgCount = true;
            }
            if (frameIndexR[ATK].x > 2)
            {
                frameIndexR[ATK].x = 2;
                delay++;
                if (delay > 1)
                {

                    isATK = false;
                    delay = 0;
                    frameIndexR[ATK].x = 0;
                }
            }

        }
        currentFrame = { frameIndexR[ATK].x,frameIndexR[ATK].y };
        /*if(isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[ATK].x, frameIndexR[ATK].y);
        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[ATK].x, frameIndexR[ATK].y);*/
    }




}

void golem::stateDIE()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::DIE].y = 4;
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
        currentFrame = { frameIndexL[STATEIMAGE::DIE].x ,frameIndexL[STATEIMAGE::DIE].y};
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::DIE].y = 3;
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
        //img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    }

    coinDrop(5, 40);
}

void golem::stateWalk()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::ATK].x = 5;
        frameIndexL[STATEIMAGE::ATK].y = 0;
        

        frameIndexL[WALK].y = 2;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexL[WALK].x--;
            if (frameIndexL[WALK].x < 0)
            {
                SOUNDMANAGER->play("golemWalkL", false,-0.2f);
                frameIndexL[WALK].x = 5;
            }
        }
        currentFrame = { frameIndexL[WALK].x ,frameIndexL[WALK].y };
       /* if (isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[WALK].x, frameIndexL[WALK].y);
        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[WALK].x, frameIndexL[WALK].y);*/
    }
    else
    {
        frameIndexL[STATEIMAGE::ATK].x = 0;
        frameIndexL[STATEIMAGE::ATK].y = 0;
        frameIndexR[WALK].y = 1;
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
        /*if (isHit)
            hitImg->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[WALK].x, frameIndexR[WALK].y);

        else
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[WALK].x, frameIndexR[WALK].y);*/
    }

}

