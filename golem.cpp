#include "stdafx.h"
#include "golem.h"

HRESULT golem::init(tagTile* tile, POINT _pos )
{
    img = IMAGEMANAGER->addFrameImage("golem", "wizard/Golem.bmp", 720, 700, 6, 5);
    cul = { 0,0 };
    pos.x = _pos.x;
    pos.y = _pos.y;
    speed = 5.f;
    state = STATEIMAGE::IDLE;
    frameIndex = { 0,0 };
    count = 0;
    isFindWayOn = false; //길찾기 온/오프
    isLeft = false;
    astar = new astarManager;
    astar->init(tile);

    return S_OK;
}

void golem::release()
{
    astar->release();
    SAFE_DELETE(astar);
}

void golem::update()
{
    
    rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
    
    if (distanceMax > getDistance(pos.x, pos.y, playerRC.left + (playerRC.right - playerRC.left) / 2, playerRC.top + (playerRC.bottom - playerRC.top) / 2))
        isFindWayOn = true;
    else
        isFindWayOn = false;
   
   
    if (isFindWayOn) //길찾기 on
    {
        state = STATEIMAGE::WALK;
        astar->update(camRC, rc, playerRC, &angle);
        if (astar->getFirstTile()) // 걸을 때
        {
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
            if (0 < cos(angle) * speed)
                isLeft = false;
            else
                isLeft = true;
        }
        else // 걷지 않고 있을 때
            state = STATEIMAGE::IDLE;
       
    }
    else //길찾기 off
    { 
        state = STATEIMAGE::IDLE;
    }

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);
    
   
   
}

void golem::render()
{
    
    FrameRect(getMemDC(), rc, RGB(0, 0, 0));
    stateImageRender();
  

    //astar->render(getMemDC());
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
        stateImage(5, 2, 0, 1);
        
        break;
    case STATEIMAGE::ATK:
        if (isLeft)
        {
            frameIndex.y = 0;
            count++;
            if (count % 5 == 0)
            {
                count = 0;
                frameIndex.x--;
                if (frameIndex.x < 3)
                    frameIndex.x = 3;
            }
        }
        else
        {
            frameIndex.y = 0;
            count++;
            if (count % 5 == 0)
            {
                count = 0;
                frameIndex.x++;
                if (frameIndex.x > 2)
                    frameIndex.x = 2;

            }
        }

        img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);
        
        break;
    case STATEIMAGE::DIE:
        stateImage(5, 4, 0, 3);

        break;
   
    }
}

void golem::stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R) 
{
    if (isLeft)
    {
        frameIndex.y = indexY_L;
        count++;
        if (count % 5 == 0)
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
        if (count % 5 == 0)
        {
            count = 0;
            frameIndex.x++;
            if (frameIndex.x > indexX_L)
                frameIndex.x = indexX_R;

        }
    }
        
    img->frameRender(getMemDC(), cul.x, cul.y, frameIndex.x, frameIndex.y);

   
}
