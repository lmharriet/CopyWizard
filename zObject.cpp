#include "stdafx.h"
#include "zObject.h"

HRESULT zObject::init(int _index, string _keyName, string _type, POINT _frame, float _x, float _y)
{
    x = _x;
    y = _y;

    type = _type;
    Index = _index;
    keyName = _keyName;
    frame = _frame;

    tagTile* tile = PLAYERDATA->_getTile();

    if (type == "wall")
    {
        tile = PLAYERDATA->_getTile();

        rc = tile[_index].rc;
    }

    else if (type == "player")
    {
        image* img = IMAGEMANAGER->findImage(keyName);
        rc = RectMakeCenter(WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, img->getFrameWidth(), img->getFrameHeight());

        tile = nullptr;
    }

    else if (type == "monster")
    {
        image* img = IMAGEMANAGER->findImage(keyName);
        if(keyName == "summoner")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y +100, img->getFrameWidth(), img->getFrameHeight());
        else if(keyName == "knight")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y + 170, img->getFrameWidth(), img->getFrameHeight());
        else if(keyName == "golem" || keyName=="golemHit")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y + 300, img->getFrameWidth(), img->getFrameHeight());
        tile = nullptr;
    }

    return S_OK;
}

void zObject::release()
{
}

void zObject::render(HDC hdc)
{
    image* img = IMAGEMANAGER->findImage(keyName);

    if (type == "wall")
    {
        tagTile* tile = PLAYERDATA->_getTile();

        if (keyName == "wall0" ||
            keyName == "wall1" ||
            keyName == "wall2")
        {
            int height = 3 * TILESIZE;

            CAMERAMANAGER->FrameRender(hdc, img,
                tile[Index].rc.left,
                tile[Index].rc.top - height,
                tile[Index].frame.x, tile[Index].frame.y);
        }

        else if (keyName == "wallFrame0" ||
            keyName == "wallFrame1" ||
            keyName == "wallFrame2" ||
            keyName == "wallFrame3")
        {
            CAMERAMANAGER->FrameRender(hdc, img, tile[Index].rc.left, tile[Index].rc.top, tile[Index].frame.x, tile[Index].frame.y);
        }
    }

    else if (type == "player")
    {
        img->frameRender(hdc, WINSIZEX/2 - 50, WINSIZEY/2 - 50, frame.x, frame.y);
    }

    else if (type == "monster")
    {
        //img->frameRender(hdc, x, y, frame.x, frame.y);
        //RECT monRC=RectMake(x, y, img->getFrameWidth(),img->getFrameHeight());
        //CAMERAMANAGER->Rectangle(hdc, monRC);
        CAMERAMANAGER->FrameRender(hdc, img, x, y, frame.x, frame.y);
        //Rectangle(hdc, x, y, x+img->getFrameWidth(), img->getFrameHeight());
    }
}