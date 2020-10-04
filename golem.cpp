#include "stdafx.h"
#include "golem.h"


void golem::addInit()
{
    
}

void golem::update()
{
    
    rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
    
    //거리 계산해서 일정 거리 넘어가면 에이스타 작동안되게 하기.
    if (distanceMax > getDistance(pos.x, pos.y, playerRC.left + (playerRC.right - playerRC.left) / 2, playerRC.top + (playerRC.bottom - playerRC.top) / 2))
        isFindWayOn = true;
    else
        isFindWayOn = false;
   
   
    if (isFindWayOn) //길찾기 on
    {
        
        astar->update(camRC, rc, playerRC, &angle);
        if (astar->getFirstTile() && !isATK && !isDie) // 걸을 때
        {
            state = STATEIMAGE::WALK;
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
            if (0 < cos(angle) * speed)
                isLeft = false;
            else
                isLeft = true;
        }
        else if(!isDie)
        {
            state = STATEIMAGE::ATK;
            isATK = true;
        }
       
    }
    else //길찾기 off
    { 
        state = STATEIMAGE::IDLE;
    }

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);
    die();
}

void golem::render()
{
    
   // FrameRect(getMemDC(), rc, RGB(0, 0, 0));
    stateImageRender();
  

   // astar->render(getMemDC());
}

void golem::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        if (isLeft)
        {
            frameIndex.x = 5;
            frameIndex.y = 0;
        }
        else
        {
            frameIndex.x = 0;
            frameIndex.y = 0;
        }
        img->frameRender(getMemDC(), cul.x, cul.y,frameIndex.x,frameIndex.y);
       
        break;
    case STATEIMAGE::WALK:
        if (isLeft)
        {
            frameIndex.y = 2;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex.x--;
                if (frameIndex.x < 0)
                {
                    frameIndex.x = 5;
                }
            }
        }
        else
        {
            frameIndex.y = 1;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex.x++;
                if (frameIndex.x > 5)
                {
                    frameIndex.x = 0;
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);

        break;
    case STATEIMAGE::ATK:
        if (isLeft)
        {
            frameIndex.y = 0;
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
            frameIndex.y = 0;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex.x++;
                if (frameIndex.x > 2)
                {
                    frameIndex.x = 2;
                    delay++;
                    if(delay > 3)
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
            frameIndex.y = 4;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex.x--;
                if (frameIndex.x < 0)
                {
                    frameIndex.x = 0;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
            }
        }
        else
        {
            frameIndex.y = 3;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex.x++;
                if (frameIndex.x > 5)
                {
                    frameIndex.x = 5;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);


        break;
   
    }
}
/*

void golem::stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R) 
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

   
}*/
