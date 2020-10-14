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

    if (type == "wall")
    {
        tile = PLAYERDATA->_getTile();

        rc = tile[_index].rc;
    }

    else if (type == "player")
    {
        image* img = IMAGEMANAGER->findImage(keyName);
        rc = RectMakeCenter(WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, img->getFrameWidth(), img->getFrameHeight());
    }

    else if (type == "monster")
    {
        //image* img = IMAGEMANAGER->findImage(keyName);
        //rc = RectMakeCenter(WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, img->getFrameWidth(), img->getFrameHeight());
    }

    return S_OK;
}

void zObject::release()
{
    SAFE_DELETE(tile);
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

    }
}