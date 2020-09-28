#include "stdafx.h"
#include "golem.h"

HRESULT golem::init()
{
    img = IMAGEMANAGER->addFrameImage("golem", "wizard/Golem.bmp", 720, 700, 6, 5);
    return S_OK;
}

void golem::release()
{
}

void golem::update()
{
}

void golem::render()
{
    img->frameRender(getMemDC(), WINSIZEX / 2, WINSIZEY / 2);
}
