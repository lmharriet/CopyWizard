#include "stdafx.h"
#include "golem.h"

HRESULT golem::init(tagTile* tile )
{
    img = IMAGEMANAGER->addFrameImage("golem", "wizard/Golem.bmp", 720, 700, 6, 5);
    cul = { 0,0 };
    pos.x = 849;
    pos.y = 360;
    speed = 5.f;
    isFindWayOn = false; //길찾기 온/오프

    astar = new astarManager;
    //astar->setTileNode(tile);
    astar->init(tile);
    //astar->setCam(cam);

    return S_OK;
}

void golem::release()
{
    astar->release();
    SAFE_DELETE(astar);
}

void golem::update()
{
   
    astar->setCam(camRC);
    cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
    cul.y = CAMERAMANAGER->GetRelativeY(pos.y);
    
    //if (!isFindWayOn) //길찾기 오프
    //{
    //  angle = getAngle(cul.x, cul.y, 853, 360);
    //  if(pos.x !=849)    pos.x += cos(angle) * speed;
    //  if(pos.y !=360)    pos.y += -sin(angle) * speed;
    //}
    //else { //길찾기 온
    //
    //    
    //}
    rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
    astar->setStartRC(rc);
    astar->setEndRC(playerRC);
    astar->update();
    

}

void golem::render()
{
    
    FrameRect(getMemDC(), rc, RGB(0, 0, 0));
    img->frameRender(getMemDC(), cul.x, cul.y);

    astar->render(getMemDC());
}
