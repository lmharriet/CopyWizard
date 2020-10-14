#include "stdafx.h"
#include "knight.h"

void knight::addInit()
{
    kind = MONSTERKIND::KNIGHT;
    atk = 5;
    armour = 4;
    speed = 6.f;
    hp = 100;
    img = IMAGEMANAGER->findImage("knight");
    skillImg = NULL;
    isKnockBack = true;
    isRanger = false;
}

void knight::update()
{
    rc = RectMake(pos.x, pos.y+30, img->getFrameWidth(), img->getFrameHeight());
    if (isFindWayOn) //±æÃ£±â on
    {

        astar->update(camRC, rc, playerRC, &angle);
        if (rc.left + img->getFrameWidth()/2 < playerRC.left)
        {
            atkDirection[MONSTER_LEFT] = false;
            atkDirection[MONSTER_RIGHT] = true;
        }

        else
        {
            atkDirection[MONSTER_LEFT] = true;
            atkDirection[MONSTER_RIGHT] = false;
        }
        if (rc.top+img->getFrameHeight()/2 > playerRC.top)
        {

            atkDirection[MONSTER_UP] = true;
            atkDirection[MONSTER_DOWN] = false;
        }
        else
        {

            atkDirection[MONSTER_UP] = false;
            atkDirection[MONSTER_DOWN] = true;
        }
        if (astar->getFirstTile() && !isATK && !isDie && !isHit) // °ÉÀ» ¶§
        {
            state = STATEIMAGE::WALK;
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
           
        }
        else if(!isDie && !isHit) // °ÈÁö ¾Ê°í ÀÖÀ» ¶§
        {
            state = STATEIMAGE::ATK;
            isATK = true;
        }

    }
    else //±æÃ£±â off
    {
        if (!isHit && !isDie)
        {
            state = STATEIMAGE::IDLE;
        }
    }

   
    

}

void knight::render()
{
    stateImageRender();
   // astar->render(getMemDC());
  // FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
  // FrameRect(getMemDC(), rc, RGB(255, 255, 255));
}



void knight::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        stateIDLE();
        break;
    case STATEIMAGE::WALK:
        stateImage(4, 2, 0, 1);
        break;
    case STATEIMAGE::ATK:
        stateATK();
        break;
    case STATEIMAGE::DIE:
        stateDIE();
        break;
    case STATEIMAGE::HIT:
        stateHIT({ 5,6 }, { 0,4 });
        break;

    }
}

void knight::stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R)
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::WALK].y = indexY_L;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::WALK].x--;
            if (frameIndexL[STATEIMAGE::WALK].x == 3)
                SOUNDMANAGER->play("knightWalkL", false,-0.25f);

            if (frameIndexL[STATEIMAGE::WALK].x < indexX_R)
                frameIndexL[STATEIMAGE::WALK].x = indexX_L;
        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexL[STATEIMAGE::WALK].x, frameIndexL[STATEIMAGE::WALK].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::WALK].y = indexY_R;

        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexR[STATEIMAGE::WALK].x++;
            if(frameIndexR[STATEIMAGE::WALK].x ==1)
                SOUNDMANAGER->play("knightWalkR", false,-0.25f);

            if (frameIndexR[STATEIMAGE::WALK].x > indexX_L)
                frameIndexR[STATEIMAGE::WALK].x = indexX_R; 

        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexR[STATEIMAGE::WALK].x, frameIndexR[STATEIMAGE::WALK].y);
    }

    


}

void knight::stateIDLE()
{
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::IDLE].x = 1;
        frameIndexL[STATEIMAGE::IDLE].y = 0;
        frameIndexL[STATEIMAGE::WALK].x = 4;
        frameIndexL[STATEIMAGE::WALK].y = 2;
        frameIndexL[STATEIMAGE::ATK].x = 5;
        frameIndexL[STATEIMAGE::ATK].y = 3;
        frameIndexL[STATEIMAGE::DIE].x = 0;
        frameIndexL[STATEIMAGE::DIE].y = 4;
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::IDLE].x = 0;
        frameIndexR[STATEIMAGE::IDLE].y = 0;
        frameIndexR[STATEIMAGE::WALK].x = 0;
        frameIndexR[STATEIMAGE::WALK].y = 1;
        frameIndexR[STATEIMAGE::ATK].x = 0;
        frameIndexR[STATEIMAGE::ATK].y = 3;
        frameIndexR[STATEIMAGE::DIE].x = 5;
        frameIndexR[STATEIMAGE::DIE].y = 6;
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
    }
    bulletDirection[MONSTER_DOWN] = false;
    bulletDirection[MONSTER_UP] = false;
    bulletDirection[MONSTER_LEFT] = false;
    bulletDirection[MONSTER_RIGHT] = false;
    isFxAppear = false;
}

void knight::stateATK()
{
    char str[50];
    sprintf(str, "knightAtk%d", RANDOM->range(4));


    if (atkDirection[MONSTER_UP] && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 40)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashUp", { pos.x + 60 ,pos.y + 30 }, true);
            SOUNDMANAGER->play(str, false, -0.25f);
            bulletDirection[MONSTER_DOWN] = false;
            bulletDirection[MONSTER_UP] = true;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            isFxAppear = true;
            delay++;
        }
    }
    
    if (atkDirection[MONSTER_DOWN] && playerRC.left > rc.left && playerRC.right < rc.right + 80)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashDown", { pos.x + 60,pos.y + 150 }, true);
            SOUNDMANAGER->play(str, false, -0.25f);
            bulletDirection[MONSTER_DOWN] = true;
            bulletDirection[MONSTER_UP] = false;
            bulletDirection[MONSTER_LEFT] = false;
            bulletDirection[MONSTER_RIGHT] = false;
            isFxAppear = true;
            delay++;
        }
    }
   
    if (atkDirection[MONSTER_LEFT])
    {
        frameIndexL[STATEIMAGE::ATK].y = 3;
        count++;
        if (count % 6 == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::ATK].x--;
            if (frameIndexL[STATEIMAGE::ATK].x < 3)
            {
                if (delay == 0)
                {
                    EFFECT->setEffect("knightSlashL", { pos.x  ,pos.y + 120 }, true);
                    SOUNDMANAGER->play(str, false, -0.25f);
                    bulletDirection[MONSTER_DOWN] = false;
                    bulletDirection[MONSTER_UP] = false;
                    bulletDirection[MONSTER_LEFT] = true;
                    bulletDirection[MONSTER_RIGHT] = false;
                    isFxAppear = true;
                }
                frameIndexL[STATEIMAGE::ATK].x = 3;
                delay++;
                if (delay > 5)
                {
                    isATK = false;
                    isFxAppear = false;
                    delay = 0;
                    frameIndexL[STATEIMAGE::ATK].x = 5;
                }
            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexL[STATEIMAGE::ATK].x, frameIndexL[STATEIMAGE::ATK].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::ATK].y = 3;
        count++;
        if (count % 6 == 0)
        {
            count = 0;
            frameIndexR[STATEIMAGE::ATK].x++;
            if (frameIndexR[STATEIMAGE::ATK].x > 2)
            {
                if (delay == 0)
                {
                    EFFECT->setEffect("knightSlashR", { pos.x + 90,pos.y + 120 }, true);
                    SOUNDMANAGER->play(str, false, -0.25f);
                    bulletDirection[MONSTER_DOWN] = false;
                    bulletDirection[MONSTER_UP] = false;
                    bulletDirection[MONSTER_LEFT] = false;
                    bulletDirection[MONSTER_RIGHT] = true;
                    isFxAppear = true;
                }

                frameIndexR[STATEIMAGE::ATK].x = 2;
                delay++;
                if (delay > 5)
                {
                    isATK = false;
                    isFxAppear = false;
                    delay = 0;
                    frameIndexR[STATEIMAGE::ATK].x = 0;

                }
            }

        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexR[STATEIMAGE::ATK].x, frameIndexR[STATEIMAGE::ATK].y);
    }



}

void knight::stateDIE()
{
    if (atkDirection[MONSTER_LEFT])
    {
        if (frameIndexL[STATEIMAGE::DIE].y != 7)
            frameIndexL[STATEIMAGE::DIE].y = 6;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexL[STATEIMAGE::DIE].x--;


            if (frameIndexL[STATEIMAGE::DIE].x < 3 && frameIndexL[STATEIMAGE::DIE].y == 7) //»ç¸Á ÀÎµ¦½º 6
            {

                frameIndexL[STATEIMAGE::DIE].x = 3;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
            else if (frameIndexL[STATEIMAGE::DIE].x < 0) // »ç¸Á ÀÎµ¦½º 5
            {
                frameIndexL[STATEIMAGE::DIE].x = 5;
                frameIndexL[STATEIMAGE::DIE].y = 7;

            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
    }
    else
    {
        if (frameIndexR[STATEIMAGE::DIE].y != 5)
            frameIndexR[STATEIMAGE::DIE].y = 4;
        count++;
        if (count % 10 == 0)
        {
            count = 0;

            frameIndexR[STATEIMAGE::DIE].x++;

            if (frameIndexR[STATEIMAGE::DIE].x > 2 && frameIndexR[STATEIMAGE::DIE].y == 5) //»ç¸Á ÀÎµ¦½º 6
            {

                frameIndexR[STATEIMAGE::DIE].x = 2;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
            else if (frameIndexR[STATEIMAGE::DIE].x > 5) // »ç¸Á ÀÎµ¦½º 5
            {
                frameIndexR[STATEIMAGE::DIE].x = 0;
                frameIndexR[STATEIMAGE::DIE].y = 5;

            }

        }
        img->frameRender(getMemDC(), cul.x, cul.y+30, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    }


    coinDrop(10, 40);
}
