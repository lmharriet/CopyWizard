#include "stdafx.h"
#include "effectManager.h"

HRESULT effectManager::init()
{
    addImage();
    time = 0;
    return S_OK;
}

void effectManager::release()
{
}

void effectManager::render(HDC hdc)
{
    time++;
    vector<tagDashEffect>::iterator iter;
    for (iter = vDashEft.begin(); iter != vDashEft.end();)
    {
        POINT position = { CAMERAMANAGER->GetRelativeX(iter->pos.x),CAMERAMANAGER->GetRelativeY(iter->pos.y) };

        IMAGEMANAGER->findImage(iter->keyName)->frameRender(hdc, position.x, position.y, iter->imgCount, iter->frameY);

        if (!iter->flipImg)
        {
            if (time % 5 == 0)iter->imgCount++;

            if (iter->imgCount == iter->maxFrame)
            {
                iter = vDashEft.erase(iter);
            }
            else iter++;
        }
        else
        {
            if (time % 5 == 0)iter->imgCount--;

            if (iter->imgCount == 0)
            {
                iter = vDashEft.erase(iter);
            }
            else iter++;
        }
    }
}

void effectManager::addImage()
{
    //dash
    IMAGEMANAGER->addFrameImage("dashVertical", "Images/effect/dashVertical.bmp", 385, 44, 5, 2);
    IMAGEMANAGER->addFrameImage("dashHorizon", "Images/effect/dashHorizon.bmp", 112, 154, 5, 2);

    IMAGEMANAGER->addFrameImage("dashLURU", "Images/effect/dashLURU.bmp", 395, 158, 5, 2);
    IMAGEMANAGER->addFrameImage("dashLDRD", "Images/effect/dashLDRD.bmp", 395, 158, 5, 2);

    //other
}

void effectManager::dashEffect(MOVE direction, POINT pos)
{
    switch (direction)
    {
    case MOVE::LEFT:
        setDash("dashHorizon", 0, false, { pos.x,pos.y - 25 });
        break;
    case MOVE::LEFT_TOP:
        setDash("dashLURU", 0, false, { pos.x,pos.y });
        break;
    case MOVE::RIGHT:
        setDash("dashHorizon", 1, true, { pos.x,pos.y - 25 });
        break;
    case MOVE::RIGHT_TOP:
        setDash("dashLURU", 1, true, { pos.x - 35,pos.y - 35 });
        break;
    case MOVE::UP:
        setDash("dashVertical", 0, false, { pos.x - 35,pos.y });
        break;
    case MOVE::DOWN:
        setDash("dashVertical", 1, false, { pos.x - 35,pos.y });
        break;
    case MOVE::LEFT_DOWN:
        setDash("dashLDRD", 0, false, { pos.x - 35,pos.y - 35 });
        break;
    case MOVE::RIGHT_DOWN:
        setDash("dashLDRD", 1, true, { pos.x,pos.y });
        break;
    default:
        return;
    }
}

void effectManager::setDash(string keyName, int frameY, bool flip, POINT pt)
{
    tagDashEffect effect;

    effect.keyName = keyName;
    effect.frameY = frameY;
    effect.flipImg = flip;
    effect.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();

    if (!flip) effect.imgCount = 0;
    else effect.imgCount = effect.maxFrame;

    effect.pos = pt;

    vDashEft.push_back(effect);
}