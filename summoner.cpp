#include "stdafx.h"
#include "summoner.h"



void summoner::addInit()
{
    atkTime = 0;

   

}

void summoner::render()
{
    //FrameRect(getMemDC(), rc, RGB(255, 255, 255));
    //FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
    stateImageRender();
}

void summoner::update()
{
    rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
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

    if (!isDie && atkTime%20==0)
    {
        cout << atkTime << endl;
        atkTime = 0;
        state = STATEIMAGE::ATK;
        isATK = true;
    }
    
    

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);

    die();
}

void summoner::stateImageRender()
{
    

    switch (state)
    {
    case STATEIMAGE::IDLE:
        if (isLeft)
        {
            frameIndex[STATEIMAGE::IDLE].x = 0;
            frameIndex[STATEIMAGE::IDLE].y = 0;
        }
        else
        {
            frameIndex[STATEIMAGE::IDLE].x = 0;
            frameIndex[STATEIMAGE::IDLE].y = 2;
        }
        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::IDLE].x, frameIndex[STATEIMAGE::IDLE].y);

        break;
   
    case STATEIMAGE::ATK:
       
        if (isLeft)
        {
            int atkTime = 10;

            if(frameIndex[STATEIMAGE::ATK].y!=1)
                frameIndex[STATEIMAGE::ATK].y = 0;
                count++;
            if (count % atkTime == 0)
            {
                count = 0;
                frameIndex[STATEIMAGE::ATK].x++;
                if (frameIndex[STATEIMAGE::ATK].y == 1 && frameIndex[STATEIMAGE::ATK].x > 4)
                {
                    frameIndex[STATEIMAGE::ATK].x = 4;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex[STATEIMAGE::ATK].x = 0;
                        frameIndex[STATEIMAGE::ATK].y = 0;
                    }
                }
                else if (frameIndex[STATEIMAGE::ATK].x > 4)
                {
                    frameIndex[STATEIMAGE::ATK].x = 0;
                    frameIndex[STATEIMAGE::ATK].y = 1;
                }
            }
        }
        else
        {
            int atkTime = 10;
            if (frameIndex[STATEIMAGE::ATK].y != 3)
                frameIndex[STATEIMAGE::ATK].y = 2;
           
            count++;
            if (count % atkTime == 0)
            {
                count = 0;
                if (frameIndex[STATEIMAGE::ATK].y == 2)
                {
                    frameIndex[STATEIMAGE::ATK].x++;
                    if (frameIndex[STATEIMAGE::ATK].x > 4)
                    {
                        frameIndex[STATEIMAGE::ATK].x = 4;
                        frameIndex[STATEIMAGE::ATK].y = 3;
                    }
                }

                else
                    frameIndex[STATEIMAGE::ATK].x--;
                if (frameIndex[STATEIMAGE::ATK].x < 0)
                {
                    frameIndex[STATEIMAGE::ATK].x = 0;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex[STATEIMAGE::ATK].x = 0;
                        frameIndex[STATEIMAGE::ATK].y = 2;
                    }
                }
            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::ATK].x, frameIndex[STATEIMAGE::ATK].y);

        break;
    case STATEIMAGE::DIE:
        if (isLeft)
        {
            int dieFrameSpeed = 10;
            if (frameIndex[STATEIMAGE::DIE].y != 5)
                frameIndex[STATEIMAGE::DIE].y = 6;
           
            count++;
            if (count % dieFrameSpeed == 0)
            {
                count = 0;
                if (frameIndex[STATEIMAGE::DIE].y == 6)
                {
                    frameIndex[STATEIMAGE::DIE].x--;

                    if (frameIndex[STATEIMAGE::DIE].x < 0)
                    {
                        frameIndex[STATEIMAGE::DIE].y = 5;
                        frameIndex[STATEIMAGE::DIE].x = 3;
                    }
                }
                else
                {
                    frameIndex[STATEIMAGE::DIE].x++;
                    if (frameIndex[STATEIMAGE::DIE].x > 4)
                    {
                        frameIndex[STATEIMAGE::DIE].x = 4;
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
            if (frameIndex[STATEIMAGE::DIE].y != 5)
                frameIndex[STATEIMAGE::DIE].y = 4;

            count++;
            if (count % dieFrameSpeed == 0)
            {
                count = 0;
                
                frameIndex[STATEIMAGE::DIE].x++;

                if (frameIndex[STATEIMAGE::DIE].y==5 && frameIndex[STATEIMAGE::DIE].x>1)
                {
                    frameIndex[STATEIMAGE::DIE].x = 1;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }
                else if (frameIndex[STATEIMAGE::DIE].x > 4)
                {
                    frameIndex[STATEIMAGE::DIE].y = 5;
                    frameIndex[STATEIMAGE::DIE].x = 0;
                }
            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::DIE].x, frameIndex[STATEIMAGE::DIE].y);
        coinDrop(1, 10);
        break;

    }
}



