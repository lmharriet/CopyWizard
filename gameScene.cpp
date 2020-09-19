#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{
	//백그라운드 이미지 초기화
	IMAGEMANAGER->addImage("백그라운드", "Images/background.bmp", WINSIZEX, WINSIZEY);
	//UFO 프레임 이미지 초기화
	IMAGEMANAGER->addFrameImage("ufo", "Images/ufo.bmp", 530, 32, 10, 1);
	//총알 이미지
	IMAGEMANAGER->addImage("bullet", "Images/bullet.bmp", 21, 21, true, RGB(255, 0, 255));



	//플레이어 클래스 초기화
	_player = new player;
	_player->init();

	//에너미매니져 클래스 초기화
	_enemyManager = new enemyManager;
	_enemyManager->init();

	//상호참조하기 위해 각각의 클래스 포인터 세팅해주기
	_player->setEMLink(_enemyManager);
	_enemyManager->setPlayerLink(_player);

	return S_OK;
}

void gameScene::release()
{
	//플레이어 해제
	_player->release();
	SAFE_DELETE(_player);

	//에너미매니져 해제
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
}

void gameScene::update()
{
	//플레이어 업데이트
	_player->update();

	//에너미매니져 업데이트
	_enemyManager->update();
}

void gameScene::render()
{
	//백그라운드 렌더
	IMAGEMANAGER->render("백그라운드", getMemDC());

	//플레이어 렌더
	_player->render();

	//에너미매니져 렌더
	_enemyManager->render();
}
