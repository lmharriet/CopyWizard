#include "stdafx.h"
#include "golem.h"


void golem::addInit()
{
    for (int i = 0; i < MAX; i++)
    {
        atkDirection[i] = false;
    }
  
 
    smallSlashIndex = { 0,0 };
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
    
   //FrameRect(getMemDC(), playerRC,RGB(255,255,255));
   //FrameRect(getMemDC(), rc,RGB(255,255,255));
   
}

void golem::stateImageRender()
{
    switch (state)
    {
    case STATEIMAGE::IDLE:
        if (atkDirection[LEFT])
        {
            frameIndexL[STATEIMAGE::IDLE].x = 5;
            frameIndexL[STATEIMAGE::IDLE].y = 0;
            frameIndexL[STATEIMAGE::WALK].x = 5;
            frameIndexL[STATEIMAGE::WALK].y = 2;
            frameIndexL[STATEIMAGE::ATK].x = 0;
            frameIndexL[STATEIMAGE::ATK].y = 0;
            frameIndexL[STATEIMAGE::DIE].x = 5;
            frameIndexL[STATEIMAGE::DIE].y = 4;
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
        }
        img->frameRender(getMemDC(), cul.x, cul.y,frameIndexL[STATEIMAGE::IDLE].x, frameIndexL[STATEIMAGE::IDLE].y);
        
        break;
    case STATEIMAGE::WALK:
        if (atkDirection[LEFT])
        {
            frameIndexL[WALK].y = 2;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndexL[WALK].x--;
                if (frameIndexL[WALK].x < 0)
                {
                    frameIndexL[WALK].x = 5;
                }
            }
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[WALK].x, frameIndexL[WALK].y);
        }
        else
        {
            frameIndexR[WALK].y = 1;
            count++;
            if (count % 10 == 0)
            {
                count = 0;
                frameIndexR[WALK].x++;
                if (frameIndexR[WALK].x > 5)
                {
                    frameIndexR[WALK].x = 0;
                }

            }
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[WALK].x, frameIndexR[WALK].y);
        }

       

        break;
    case STATEIMAGE::ATK:
        if (atkDirection[LEFT])
        {
            frameIndexL[ATK].y = 0;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndexL[ATK].x--;
                if (frameIndexL[ATK].x == 3)
                {
                    isFxAppear = true;
                }
                if (frameIndexL[ATK].x < 3)
                {
                    frameIndexL[ATK].x = 3;
                    delay++;
                    if (delay > 3)
                    {
                        isATK = false;
                        delay = 0;
                        frameIndexL[ATK].x = 5;
                    }
                }
            }
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[ATK].x, frameIndexL[ATK].y);
        }
        else 
        {
            frameIndexR[ATK].y = 0;
            count++;
            if (count % 30 == 0)
            {
                count = 0;
                frameIndexR[ATK].x++;
                if (frameIndexR[ATK].x == 2)
                {
                    isFxAppear = true;
                 }
                if (frameIndexR[ATK].x > 2)
                {
                    frameIndexR[ATK].x = 2;
                    delay++;
                    if(delay > 3)
                    {
                        
                        isATK = false;
                        delay = 0;
                        frameIndexR[ATK].x = 0;
                    }
                }

            }
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[ATK].x, frameIndexR[ATK].y);
        }

       
        
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
            if (atkDirection[UP]&&atkDirection[LEFT]&&playerRC.left>rc.left-20&&playerRC.right<rc.right+40)
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
            else if (atkDirection[UP] && atkDirection[RIGHT] && playerRC.left > rc.left-20 && playerRC.right < rc.right+40)
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

            else if (atkDirection[DOWN]&&atkDirection[LEFT] && playerRC.left > rc.left-20 && playerRC.right < rc.right+80)
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
            else if (atkDirection[DOWN] && atkDirection[RIGHT] && playerRC.left > rc.left-20 && playerRC.right < rc.right+80)
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
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::DIE].x, frameIndexL[STATEIMAGE::DIE].y);
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
            img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::DIE].x, frameIndexR[STATEIMAGE::DIE].y);
        }

        
        coinDrop(1, 10);


        break;
   
    }
}