#include "stdafx.h"
#include "bossScene.h"

HRESULT bossScene::init()
{
    _background = IMAGEMANAGER->addImage("bossRoom", "map/bossRoom.bmp", 1500, 989, true, RGB(255, 0, 255));
    _player = new player;
    _player->init();
    CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

    EFFECT->init();
    UI->init();

    return S_OK;
}

void bossScene::release()
{
    _player->release();
    SAFE_DELETE(_player);

    _background->release();
    SAFE_DELETE(_background);
}

void bossScene::update()
{
    UI->update();

    _player->other_update();
    _player->colorCheck(_background);
}

void bossScene::render()
{
    CAMERAMANAGER->Render(getMemDC(), _background, 0, 0);
    EFFECT->render(getMemDC());
    _player->render();
    UI->render(getMemDC(), 50, 50);
}