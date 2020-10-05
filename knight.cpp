#include "stdafx.h"
#include "knight.h"

void knight::addInit()
{
}

void knight::update()
{
    rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());

    
    if (distanceMax > getDistance(pos.x, pos.y, playerRC.left + (playerRC.right - playerRC.left) / 2, playerRC.top + (playerRC.bottom - playerRC.top) / 2))
        isFindWayOn = true;
    else
        isFindWayOn = false;


    if (isFindWayOn) //±Ê√£±‚ on
    {

        astar->update(camRC, rc, playerRC, &angle);
        if (astar->getFirstTile() && !isATK && !isDie) // ∞…¿ª ∂ß
        {
            state = STATEIMAGE::WALK;
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
            if (0 < cos(angle) * speed)
                isLeft = false;
            else
                isLeft = true;
        }
        else if(!isDie) // ∞»¡ˆ æ ∞Ì ¿÷¿ª ∂ß
        {
            state = STATEIMAGE::ATK;
            isATK = true;
        }

    }
    else //±Ê√£±‚ off
    {
        state = STATEIMAGE::IDLE;
    }

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);

    die();
}

void knight::render()
{
    stateImageRender();
   // astar->render(getMemDC());
}



void knight::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        if (isLeft)
        {
            frameIndex[STATEIMAGE::IDLE].x = 1;
            frameIndex[STATEIMAGE::IDLE].y = 0;
        }
        else
        {
            frameIndex[STATEIMAGE::IDLE].x = 0;
            frameIndex[STATEIMAGE::IDLE].y = 0;
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::IDLE].x, frameIndex[STATEIMAGE::IDLE].y);

        break;
    case STATEIMAGE::WALK:
        stateImage(4, 2, 0, 1);

        break;
    case STATEIMAGE::ATK:
        if (isLeft)
        {
            frameIndex[STATEIMAGE::ATK].y = 3;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex[STATEIMAGE::ATK].x--;
                if (frameIndex[STATEIMAGE::ATK].x < 3)
                {
                    frameIndex[STATEIMAGE::ATK].x = 3;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex[STATEIMAGE::ATK].x = 5;
                    }
                }
            }
        }
        else
        {
            frameIndex[STATEIMAGE::ATK].y = 3;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex[STATEIMAGE::ATK].x++;
                if (frameIndex[STATEIMAGE::ATK].x > 2)
                {
                    frameIndex[STATEIMAGE::ATK].x = 2;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex[STATEIMAGE::ATK].x = 0;
                    }
                }

            }
        }

        EFFECT->setEffect("smallSlash", pos);
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::ATK].x, frameIndex[STATEIMAGE::ATK].y);
        break;
    case STATEIMAGE::DIE:
        if (isLeft)
        {
            if (frameIndex[STATEIMAGE::DIE].y != 7)
            frameIndex[STATEIMAGE::DIE].y = 6;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                    frameIndex[STATEIMAGE::DIE].x--;
               

                if (frameIndex[STATEIMAGE::DIE].x < 3 && frameIndex[STATEIMAGE::DIE].y == 7) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
                {
                    
                    frameIndex[STATEIMAGE::DIE].x = 3;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
                else if (frameIndex[STATEIMAGE::DIE].x < 0) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
                {
                    frameIndex[STATEIMAGE::DIE].x = 5;
                    frameIndex[STATEIMAGE::DIE].y = 7;
                   
                }
            }
        }
        else
        {
            if(frameIndex[STATEIMAGE::DIE].y !=5 )
            frameIndex[STATEIMAGE::DIE].y = 4;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                
                frameIndex[STATEIMAGE::DIE].x++;
                
                if (frameIndex[STATEIMAGE::DIE].x > 2 && frameIndex[STATEIMAGE::DIE].y == 5) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
                {
                    
                    frameIndex[STATEIMAGE::DIE].x = 2;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
                else if (frameIndex[STATEIMAGE::DIE].x > 5) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
                {
                    frameIndex[STATEIMAGE::DIE].x = 0;
                    frameIndex[STATEIMAGE::DIE].y = 5;
                    
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::DIE].x, frameIndex[STATEIMAGE::DIE].y);
        coinDrop(1, 10);
        break;

    }
}

void knight::stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R)
{
    if (isLeft)
    {
        frameIndex[STATEIMAGE::WALK].y = indexY_L;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndex[STATEIMAGE::WALK].x--;
            if (frameIndex[STATEIMAGE::WALK].x < indexX_R)
                frameIndex[STATEIMAGE::WALK].x = indexX_L;
        }
    }
    else
    {
        frameIndex[STATEIMAGE::WALK].y = indexY_R;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndex[STATEIMAGE::WALK].x++;
            if (frameIndex[STATEIMAGE::WALK].x > indexX_L)
                frameIndex[STATEIMAGE::WALK].x = indexX_R;

        }
    }

    img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::WALK].x, frameIndex[STATEIMAGE::WALK].y);


}