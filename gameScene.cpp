#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{
	//��׶��� �̹��� �ʱ�ȭ
	IMAGEMANAGER->addImage("��׶���", "Images/background.bmp", WINSIZEX, WINSIZEY);
	//UFO ������ �̹��� �ʱ�ȭ
	IMAGEMANAGER->addFrameImage("ufo", "Images/ufo.bmp", 530, 32, 10, 1);
	//�Ѿ� �̹���
	IMAGEMANAGER->addImage("bullet", "Images/bullet.bmp", 21, 21, true, RGB(255, 0, 255));



	//�÷��̾� Ŭ���� �ʱ�ȭ
	_player = new player;
	_player->init();

	//���ʹ̸Ŵ��� Ŭ���� �ʱ�ȭ
	_enemyManager = new enemyManager;
	_enemyManager->init();

	//��ȣ�����ϱ� ���� ������ Ŭ���� ������ �������ֱ�
	_player->setEMLink(_enemyManager);
	_enemyManager->setPlayerLink(_player);

	return S_OK;
}

void gameScene::release()
{
	//�÷��̾� ����
	_player->release();
	SAFE_DELETE(_player);

	//���ʹ̸Ŵ��� ����
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
}

void gameScene::update()
{
	//�÷��̾� ������Ʈ
	_player->update();

	//���ʹ̸Ŵ��� ������Ʈ
	_enemyManager->update();
}

void gameScene::render()
{
	//��׶��� ����
	IMAGEMANAGER->render("��׶���", getMemDC());

	//�÷��̾� ����
	_player->render();

	//���ʹ̸Ŵ��� ����
	_enemyManager->render();
}
