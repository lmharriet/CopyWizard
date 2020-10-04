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
            frameIndex.x = 1;
            frameIndex.y = 0;
        }
        else
        {
            frameIndex.x = 0;
            frameIndex.y = 0;
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);

        break;
    case STATEIMAGE::WALK:
        stateImage(4, 2, 0, 1);

        break;
    case STATEIMAGE::ATK:
        if (isLeft)
        {
            frameIndex.y = 3;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex.x--;
                if (frameIndex.x < 3)
                {
                    frameIndex.x = 3;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex.x = 5;
                    }
                }
            }
        }
        else
        {
            frameIndex.y = 3;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex.x++;
                if (frameIndex.x > 2)
                {
                    frameIndex.x = 2;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex.x = 0;
                    }
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);

        break;
    case STATEIMAGE::DIE:
        if (isLeft)
        {
            if (frameIndex.y != 7)
            frameIndex.y = 6;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                    frameIndex.x--;
               

                if (frameIndex.x < 3 && frameIndex.y == 7) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
                {
                    
                    frameIndex.x = 3;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
                else if (frameIndex.x < 0) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
                {
                    frameIndex.x = 5;
                    frameIndex.y = 7;
                   
                }
            }
        }
        else
        {
            if(frameIndex.y !=5 )
            frameIndex.y = 4;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                
                frameIndex.x++;
                
                if (frameIndex.x > 2 && frameIndex.y == 5) //ªÁ∏¡ ¿Œµ¶Ω∫ 6
                {
                    
                    frameIndex.x = 2;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
                else if (frameIndex.x > 5) // ªÁ∏¡ ¿Œµ¶Ω∫ 5
                {
                    frameIndex.x = 0;
                    frameIndex.y = 5;
                    
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);
        coinDrop(1, 10);
        break;

    }
}

void knight::stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R)
{
    if (isLeft)
    {
        frameIndex.y = indexY_L;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndex.x--;
            if (frameIndex.x < indexX_R)
                frameIndex.x = indexX_L;
        }
    }
    else
    {
        frameIndex.y = indexY_R;
        count++;
        if (count % 10 == 0)
        {
            count = 0;
            frameIndex.x++;
            if (frameIndex.x > indexX_L)
                frameIndex.x = indexX_R;

        }
    }

    img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);


}