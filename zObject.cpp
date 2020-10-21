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
        else if (keyName == "ghoulLarge" || keyName == "ghoulLargeHit")
            rc = RectMakeCenter(x - img->getFrameWidth() / 2, y + 170, img->getFrameWidth(), img->getFrameHeight());
        tile = nullptr;
    }

    else if (type == "npc")
    {
        rc = RectMake(x - img->getWidth() + img->getWidth()/2, y + img->getHeight()/2 - 50, img->getWidth(), img->getHeight() / 2);
    }

    else if (type == "chest")
    {
        rc = RectMake(x - img->getFrameWidth() + img->getFrameWidth() / 2, y + img->getFrameHeight() / 2 - 50, img->getFrameWidth(), img->getFrameHeight() / 2);
    }

    return S_OK;
}

void zObject::release()
{
}

void zObject::render(HDC hdc)
{
    if (type == "wall")
    {
        tagTile* tile = PLAYERDATA->_getTile();

        if (keyName == "wall0" ||
            keyName == "wall1" ||
            keyName == "wall2")
        {
            int height = 3 * TILESIZE;

            CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage(keyName),
                tile[Index].rc.left,
                tile[Index].rc.top - height,
                tile[Index].frame.x, tile[Index].frame.y);
        }

        else if (keyName == "wallFrame0" ||
            keyName == "wallFrame1" ||
            keyName == "wallFrame2" ||
            keyName == "wallFrame3")
        {
            CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage(keyName), tile[Index].rc.left, tile[Index].rc.top, tile[Index].frame.x, tile[Index].frame.y);
        }
    }

    else if (type == "monster")
    {
        CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage(keyName), x, y, frame.x, frame.y);
    }

    else if (type == "player")
    {
        IMAGEMANAGER->findImage(keyName)->frameRender(hdc, (WINSIZEX >> 1) - 50, (WINSIZEY >> 1) - 50, frame.x, frame.y);
    }

    else if (type == "npc")
    {
        //CAMERAMANAGER->Rectangle(hdc, rc);
        CAMERAMANAGER->Render(hdc, IMAGEMANAGER->findImage(keyName),
            x - (IMAGEMANAGER->findImage(keyName)->getWidth() >> 1),
            rc.top - (IMAGEMANAGER->findImage(keyName)->getHeight() >> 1));
    }

    else if (type == "chest")
    {
        CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage(keyName),
            x - (IMAGEMANAGER->findImage(keyName)->getFrameWidth() >> 1),
            y - (IMAGEMANAGER->findImage(keyName)->getFrameHeight() >> 1),
            frame.x, 0);
    }
}