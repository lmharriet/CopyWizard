#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	if (_loading != nullptr)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("인게임");
	}

}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();

	textOut(getMemDC(), 10, 10, "로딩화면");
}

void loadingScene::loadingImage()
{
	//몬스터 이미지
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
	IMAGEMANAGER->addFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
	IMAGEMANAGER->addFrameImage("ghoul", "resource/enemy/knight.bmp", 1200, 1400, 6, 7);

	//스킬 이펙트 이미지
	IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
	IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);



}

void loadingScene::loadingSound()
{
}
