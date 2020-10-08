#include "stdafx.h"
#include "knight.h"

void knight::addInit()
{
   
}

void knight::update()
{

    
   /* if (distanceMax > getDistance(pos.x+img->getFrameWidth()*0.5, pos.y+img->getFrameHeight()*1.5, playerRC.left, playerRC.top ))
        isFindWayOn = true;
    else
        isFindWayOn = false;*/


    if (isFindWayOn) //±Ê√£±‚ on
    {

        astar->update(camRC, rc, playerRC, &angle);
        if (astar->getFirstTile() && !isATK && !isDie && !isHit) // ∞…¿ª ∂ß
        {
            state = STATEIMAGE::WALK;
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
            
            if (0 < cos(angle) * speed)
            {
                atkDirection[MONSTER_LEFT] = false;
                atkDirection[MONSTER_RIGHT] = true;
            } 
            
            else
            {
                atkDirection[MONSTER_LEFT] = true;
                atkDirection[MONSTER_RIGHT] = false;
            }
            if (0 < sin(angle) * speed)
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
        else if(!isDie && !isHit) // ∞»¡ˆ æ ∞Ì ¿÷¿ª ∂ß
        {
            state = STATEIMAGE::ATK;
            isATK = true;
        }

    }
    else //±Ê√£±‚ off
    {
        if (!isHit)
        {
            state = STATEIMAGE::IDLE;
        }
    }

    

}

void knight::render()
{
    stateImageRender();
   // astar->render(getMemDC());
    //FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
    //FrameRect(getMemDC(), rc, RGB(255, 255, 255));
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
            if (frameIndexL[STATEIMAGE::WALK].x < indexX_R)
                frameIndexL[STATEIMAGE::WALK].x = indexX_L;
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::WALK].x, frameIndexL[STATEIMAGE::WALK].y);
    }
    else
    {
        frameIndexR[STATEIMAGE::WALK].y = indexY_R;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndexR[STATEIMAGE::WALK].x++;
            if (frameIndexR[STATEIMAGE::WALK].x > indexX_L)
                frameIndexR[STATEIMAGE::WALK].x = indexX_R;

        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::WALK].x, frameIndexR[STATEIMAGE::WALK].y);
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
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
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
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::IDLE].x, frameIndexR[STATEIMAGE::IDLE].y);
    }
}

void knight::stateATK()
{
    if (atkDirection[MONSTER_UP] /*&& atkDirection[LEFT] */ && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 40)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashUp", { pos.x + 60 ,pos.y + 30 }, true);
            delay++;
        }
    }
    /*else if (atkDirection[UP] && atkDirection[RIGHT] && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 40)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashUp", { pos.x + 60 ,pos.y +30 }, true);
            delay++;
        }
    }*/
    if (atkDirection[MONSTER_DOWN] /*&& atkDirection[LEFT] */ && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 80)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashDown", { pos.x + 60,pos.y + 150 }, true);
            delay++;
        }
    }
    /*else if (atkDirection[DOWN] && atkDirection[RIGHT] && playerRC.left > rc.left - 20 && playerRC.right < rc.right + 80)
    {
        if (delay == 0)
        {
            EFFECT->setEffect("knightSlashDown", { pos.x +60,pos.y + 150 }, true);
            delay++;
        }
    }*/
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
                    EFFECT->setEffect("knightSlashL", { pos.x  ,pos.y + 90 }, true);
                }
                frameIndexL[STATEIMAGE::ATK].x = 3;
                delay++;
                if (delay > 5)
                {
                    isATK = false;
                    delay = 0;
                    frameIndexL[STATEIMAGE::ATK].x = 5;
                }
            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::ATK].x, frameIndexL[STATEIMAGE::ATK].y);
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
                    EFFECT->setEffect("knightSlashR", { pos.x + 90,pos.y + 90 }, true);
                }

                frameIndexR[STATEIMAGE::ATK].x = 2;
                delay++;
                if (delay > 5)
                {
                    isATK = false;
                    delay = 0;
                    frameIndexR[STATEIMAGE::ATK].x = 0;

                }
            }

        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::ATK].x, frameIndexR[STATEIMAGE::ATK].y);
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


            if (frameIndexL[STATEIMAGE::DIE].x < 3 && frameIndexL[STATEIMAGE::DIE].y == 7) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
            {

                frameIndexL[STATEIMAGE::DIE].x = 3;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
            else if (frameIndexL[STATEIMAGE::DIE].x < 0) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
            {
                frameIndexL[STATEIMAGE::DIE].x = 5;
                frameIndexL[STATEIMAGE::DIE].y = 7;

            }
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
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

            if (frameIndexR[STATEIMAGE::DIE].x > 2 && frameIndexR[STATEIMAGE::DIE].y == 5) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
            {

                frameIndexR[STATEIMAGE::DIE].x = 2;
                delay++;
                if (delay > 2)
                {
                    isDelete = true;
                }
            }
            else if (frameIndexR[STATEIMAGE::DIE].x > 5) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
            {
                frameIndexR[STATEIMAGE::DIE].x = 0;
                frameIndexR[STATEIMAGE::DIE].y = 5;

            }

        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
    }


    coinDrop(1, 10);
}
