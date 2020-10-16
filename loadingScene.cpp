#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//�ε�Ŭ���� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	//�ε� �̹��� �� ���� �ʱ�ȭ
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//�ε�Ŭ���� ����
	if (_loading != nullptr)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void loadingScene::update()
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��Ϸ��� ȭ�� ����
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("�ΰ���");
	}

}

void loadingScene::render()
{
	//�ε�Ŭ���� ����
	_loading->render();

	textOut(getMemDC(), 10, 10, "�ε�ȭ��");
}

void loadingScene::loadingImage()
{
	//���� �̹���
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
	IMAGEMANAGER->addFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
	IMAGEMANAGER->addFrameImage("ghoul", "resource/enemy/knight.bmp", 1200, 1400, 6, 7);

	//��ų ����Ʈ �̹���
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
