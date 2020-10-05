#include "stdafx.h"
#include "golem.h"


void golem::addInit()
{
    for (int i = 0; i < MAX; i++)
    {
        atkDirection[i] = false;
    }
  
    for (int i = 0; i < STATEMAX; i++)
    {
        frameIndex[i] = { 0,0 };
    }

    smallSlashIndex = { 0,0 };
    isFxAppear = false;
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
            {
                atkDirection[LEFT] = false;
                atkDirection[RIGHT] = true;
            }
            else
            {
                atkDirection[LEFT] = true;
                atkDirection[RIGHT] = false;
            }
            if (0 < sin(angle) * speed)
            {
               
                atkDirection[UP] = true;
                atkDirection[DOWN] = false;
            }
            else
            {
               
                atkDirection[UP] = false;
                atkDirection[DOWN] = true;
            }
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
    stateImageRender();
    
  // FrameRect(getMemDC(), playerRC,RGB(255,255,255));
  // FrameRect(getMemDC(), rc,RGB(255,255,255));
   
}

void golem::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        if (atkDirection[LEFT])
        {
            frameIndex[STATEIMAGE::IDLE].x = 5;
            frameIndex[STATEIMAGE::IDLE].y = 0;
        }
        else
        {
            frameIndex[STATEIMAGE::IDLE].x = 0;
            frameIndex[STATEIMAGE::IDLE].y = 0;
        }
        img->frameRender(getMemDC(), cul.x, cul.y,frameIndex[STATEIMAGE::IDLE].x, frameIndex[STATEIMAGE::IDLE].y);
        
        break;
    case STATEIMAGE::WALK:
        if (atkDirection[LEFT])
        {
            frameIndex[WALK].y = 2;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex[WALK].x--;
                if (frameIndex[WALK].x < 0)
                {
                    frameIndex[WALK].x = 5;
                }
            }
        }
        else
        {
            frameIndex[WALK].y = 1;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex[WALK].x++;
                if (frameIndex[WALK].x > 5)
                {
                    frameIndex[WALK].x = 0;
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[WALK].x, frameIndex[WALK].y);

        break;
    case STATEIMAGE::ATK:
        if (atkDirection[LEFT])
        {
            frameIndex[ATK].y = 0;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex[ATK].x--;
                if (frameIndex[ATK].x == 3)
                {
                    isFxAppear = true;
                }
                if (frameIndex[ATK].x < 3)
                {
                    frameIndex[ATK].x = 3;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndex[ATK].x = 5;
                    }
                }
            }
        }
        else 
        {
            frameIndex[ATK].y = 0;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndex[ATK].x++;
                if (frameIndex[ATK].x == 2)
                {
                    isFxAppear = true;
                 }
                if (frameIndex[ATK].x > 2)
                {
                    frameIndex[ATK].x = 2;
                    delay++;
                    if(delay > 3)
                    {
                        
                        isATK = false;
                        delay = 0;
                        frameIndex[ATK].x = 0;
                    }
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[ATK].x, frameIndex[ATK].y);
        
        if (!isFxAppear )
        {
            smallSlashIndex = { -1,0 };
            if (atkDirection[LEFT])
            {
                smallSlashIndex.x = 3;
            }
        }

        if (isFxAppear )
        {
            int frameSpeed = 6;
            if (atkDirection[UP]&&atkDirection[LEFT]&&playerRC.left>rc.left&&playerRC.right<rc.right)
            {
                smallSlashIndex.y = 5;
                if (count % frameSpeed == 0)
                {
                    smallSlashIndex.x--;
                    if (smallSlashIndex.x <0)
                        isFxAppear = false;
                    
                }
               
                skillImg->frameRender(getMemDC(), cul.x+10, cul.y-30, smallSlashIndex.x, smallSlashIndex.y);
            }
            else if (atkDirection[UP] && atkDirection[RIGHT] && playerRC.left > rc.left && playerRC.right < rc.right)
            {
                smallSlashIndex.y = 4;
                    if (count % frameSpeed == 0)
                    {
                        smallSlashIndex.x++;
                        if (smallSlashIndex.x > 2)
                            isFxAppear = false;
                    }

                    skillImg->frameRender(getMemDC(), cul.x+10, cul.y-30, smallSlashIndex.x, smallSlashIndex.y);
            }

            else if (atkDirection[DOWN]&&atkDirection[LEFT] && playerRC.left > rc.left && playerRC.right < rc.right)
            {
                smallSlashIndex.y = 3;
                if (count % frameSpeed == 0)
                {
                    smallSlashIndex.x--;
                    if (smallSlashIndex.x < 0)
                        isFxAppear = false;
                }
                skillImg->frameRender(getMemDC(), cul.x+10, cul.y+30, smallSlashIndex.x, smallSlashIndex.y);
            }
            else if (atkDirection[DOWN] && atkDirection[RIGHT] && playerRC.left > rc.left && playerRC.right < rc.right)
            {
                smallSlashIndex.y = 2;
                if (count % frameSpeed == 0)
                {
                    smallSlashIndex.x++;
                    if (smallSlashIndex.x > 2)
                        isFxAppear = false;
                }
                skillImg->frameRender(getMemDC(), cul.x+10, cul.y+30, smallSlashIndex.x, smallSlashIndex.y);
            }
            else if (atkDirection[RIGHT])
            {
                smallSlashIndex.y = 0;
                if (count % frameSpeed == 0)
                {
                    smallSlashIndex.x++;
                    if (smallSlashIndex.x > 2)
                        isFxAppear = false;
                }
                skillImg->frameRender(getMemDC(), cul.x + 80, cul.y + 20, smallSlashIndex.x, smallSlashIndex.y);
            }
            else 
            {
                smallSlashIndex.y = 1;
                if (count % frameSpeed == 0)
                {
                    smallSlashIndex.x--;
                    if (smallSlashIndex.x < 0)
                        isFxAppear = false;
                }
                skillImg->frameRender(getMemDC(), cul.x - 60, cul.y + 20, smallSlashIndex.x, smallSlashIndex.y);
            }
           
        }
       
        
        break;
    case STATEIMAGE::DIE:
        if (atkDirection[LEFT])
        {
            frameIndex[STATEIMAGE::DIE].y = 4;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex[STATEIMAGE::DIE].x--;
                if (frameIndex[STATEIMAGE::DIE].x < 0)
                {
                    frameIndex[STATEIMAGE::DIE].x = 0;
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
            frameIndex[STATEIMAGE::DIE].y = 3;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndex[STATEIMAGE::DIE].x++;
                if (frameIndex[STATEIMAGE::DIE].x > 5)
                {
                    frameIndex[STATEIMAGE::DIE].x = 5;
                    delay++;
                    if (delay > 2)
                    {
                        isDelete = true;
                    }
                }

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex[STATEIMAGE::DIE].x, frameIndex[STATEIMAGE::DIE].y);
        coinDrop(1, 10);


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
