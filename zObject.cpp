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
    image* img = IMAGEMANAGER->findImage(keyName);

    if (type == "wall")
    {
        tile = PLAYERDATA->_getTile();

        rc = tile[_index].rc;
    }

    else if (type == "player")
    {
        rc = RectMakeCenter(WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, img->getFrameWidth(), img->getFrameHeight());

        tile = nullptr;
    }

    else if (type == "monster")
    {
        if(keyName == "summoner")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y + 100, img->getFrameWidth(), img->getFrameHeight());
        else if(keyName == "knight")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y + 170, img->getFrameWidth(), img->getFrameHeight());
        else if(keyName == "golem" || keyName == "golemHit")
            rc = RectMakeCenter(x - img->getFrameWidth()/2, y + 300, img->getFrameWidth(), img->getFrameHeight());
        else if(keyName == "ghoul")
            rc = RectMakeCenter(x - img->getFrameWidth() / 2, y + 170, img->getFrameWidth(), img->getFrameHeight());
        tile = nullptr;
    }

    else if (type == "npc")
    {
        rc = RectMake(x - img->getWidth() + img->getWidth()/2, y + img->getHeight()/2 - 50, img->getWidth(), img->getHeight() / 2);
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
        CAMERAMANAGER->FrameRender(hdc, img, x, y, frame.x, frame.y);
    }

    else if (type == "npc")
    {
        //CAMERAMANAGER->Rectangle(hdc, rc);
        CAMERAMANAGER->Render(hdc, img, x - img->getWidth()/2, rc.top - img->getHeight()/2);
    }
}