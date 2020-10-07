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

	_boss = new boss;
	_boss->init(752, 288);
    rc = RectMakeCenter(752, 288, 150, 150);
    area = RectMakeCenter(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, WINSIZEX, WINSIZEY);

    isBattle = false;
    cutTime = 0;

	_boss->getPlayerInfo(_player);

    PARTICLE->init();
    UI->setCoin(PLAYERDATA->getCoin());
    UI->setHp(PLAYERDATA->getHp());


    //sound
    soundInit();

    return S_OK;
}

void bossScene::release()
{
    //_player->release();
    //SAFE_DELETE(_player);
}

void bossScene::update()
{
    //sound 
    if (isBossBGM)
    {
        SOUNDMANAGER->fadeIn("bossBGM", fadeIn);
        fadeIn += 0.002f;
        if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
            isBossBGM = false;
    }
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

	if (isBattle) {
		_boss->update();
	}

    PARTICLE->pointActive();
    PARTICLE->explosionActive();


}

void bossScene::bossCutScene()
{
    if (!isBattle && colCheck(_player->getRect(), area))
    {
        SOUNDMANAGER->play("CutSceneSFX", false);
        SOUNDMANAGER->play("CutSceneVO", false);
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

    //CAMERAMANAGER->Rectangle(getMemDC(), rc);
	//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("boss"), rc.left, rc.top, 0, 0);
	
	_boss->render();

    CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("frontFrame"), 0, 0);
    PARTICLE->render(getMemDC());
    UI->render(getMemDC(), 50, 50);
}

void bossScene::soundInit()
{
    isBossBGM = true;
    fadeIn = 0.f;

    SOUNDMANAGER->addSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
    SOUNDMANAGER->addSound("bossDrillVO", "Sound/bossDrillVO.mp3");
    SOUNDMANAGER->addSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
    SOUNDMANAGER->addSound("bossJumpVO", "Sound/bossJumpVO.mp3");
    SOUNDMANAGER->addSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
    SOUNDMANAGER->addSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
    SOUNDMANAGER->addSound("bossPunchVO", "Sound/bossPunchVO.mp3");
    SOUNDMANAGER->addSound("bossWallSFX", "Sound/bossWallSFX.mp3");
    SOUNDMANAGER->addSound("bossWallVO", "Sound/bossWallVO.mp3");
    SOUNDMANAGER->addSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
    SOUNDMANAGER->addSound("CutSceneVO", "Sound/CutSceneVO.mp3");

    SOUNDMANAGER->play("bossBGM",true);
}
