#include "stdafx.h"
#include "bossScene.h"

HRESULT bossScene::init()
{
    IMAGEMANAGER->addImage("bossRoom", "map/bossRoom.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("frontFrame", "map/frontFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("backFrame", "map/backFrame.bmp", MAXWIDTH, MAXHEIGHT, true, RGB(255, 0, 255));
    _player = new player;
    _player->init();
    _player->setX(752);
    _player->setY(856);
    _player->reposRect();
    CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXWIDTH * 2, MAXHEIGHT * 2,
        -MAXWIDTH, -MAXHEIGHT, WINSIZEX / 2, WINSIZEY / 2);

    EFFECT->init();
    UI->init();

    rc = RectMakeCenter(752, 288, 150, 150);
    area = RectMakeCenter(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, WINSIZEX, WINSIZEY);

    isBattle = false;
    cutTime = 0;

    return S_OK;
}

void bossScene::release()
{
    _player->release();
    SAFE_DELETE(_player);
}

void bossScene::update()
{
    //camera update
    CAMERAMANAGER->update();

    //player update
    if (!CAMERAMANAGER->getIsCutScene())
    {
        UI->update();
        _player->other_update();
    }

    else _player->setStateIDLE();

    //pixel collision
    _player->colorCheck(IMAGEMANAGER->findImage("bossRoom"));

    //cutScene check
    bossCutScene();
}

void bossScene::bossCutScene()
{
    if (!isBattle && colCheck(_player->getRect(), area))
    {
        isBattle = true; // 한번만 실행하기 위함
        CAMERAMANAGER->setCutScene
        (
            _player->getX(),                        // 시작 좌표 x
            _player->getY(),                        // 시작 좌표 y
            rc.left + (rc.right - rc.left) / 2,     // 목적 좌표 x
            rc.top + (rc.bottom - rc.top) / 2,      // 목적 좌표 y
            true,                                   // 시작 -> 목적 isLerp?
            true,                                   // 목적 -> 시작 isLerp?
            150,                                    // 컷씬 유지 시간
            50,                                     // 시작지점으로 되돌아오는 시간
            10.f                                    // lerp 속도
        );
    }
}

void bossScene::render()
{
    CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("bossRoom"), 0, 0);
    CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("backFrame"), 0, 0);

    //CAMERAMANAGER->Rectangle(getMemDC(), area);

    EFFECT->pRender(getMemDC());
    _player->render();

    CAMERAMANAGER->Rectangle(getMemDC(), rc);

    CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("frontFrame"), 0, 0);

    UI->render(getMemDC(), 50, 50);
}