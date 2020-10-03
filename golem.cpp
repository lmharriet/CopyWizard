#include "stdafx.h"
#include "golem.h"

HRESULT golem::init(tagTile* tile, POINT _pos )
{
    img = IMAGEMANAGER->addFrameImage("golem", "wizard/Golem.bmp", 720, 700, 6, 5);
    cul = { 0,0 };
    pos.x = _pos.x;
    pos.y = _pos.y;
    speed = 5.f;
    
    isFindWayOn = false; //��ã�� ��/����

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
    
    if (600.f > getDistance(pos.x, pos.y, playerRC.left + (playerRC.right - playerRC.left) / 2, playerRC.top + (playerRC.bottom - playerRC.top) / 2))
        isFindWayOn = true;
    else
        isFindWayOn = false;
    
   
    if (isFindWayOn) //��ã�� on
    {
        astar->update(camRC, rc, playerRC, &angle);
        if (astar->getFirstTile())
        {
            pos.x += cos(angle) * speed;
            pos.y += -sin(angle) * speed;
        }
       
       
    }
    else //��ã�� off
    { 
        
        
    }

    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);
    

}

void golem::render()
{
    
    FrameRect(getMemDC(), rc, RGB(0, 0, 0));
    img->frameRender(getMemDC(), cul.x, cul.y);

    astar->render(getMemDC());
}
