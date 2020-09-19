#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//�ε�Ŭ���� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	item = new loadItem;

	//�ε� �̹��� �� ���� �ʱ�ȭ
	this->loadingImage();
	this->loadingSound();

	IMAGEMANAGER->addFrameImage("number", "Images/number.bmp", 180, 30, 10, 1);
	IMAGEMANAGER->addImage("%", "Images/persent.bmp", 18, 30, true, RGB(255, 0, 255));

	return S_OK;
}

void loadingScene::release()
{
	//�ε�Ŭ���� ����
	//_loading->release();
	//SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��Ϸ��� ȭ�� ����
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("�ȼ��浹");
	}

}

void loadingScene::render()
{
	//�ε�Ŭ���� ����
	_loading->render();

	textOut(getMemDC(), 10, 10, "�ε�ȭ��");

	int temp = _loading->getCurrentGauge();

	if (_loading->getLoadItem().size() > temp)
	{
		switch (_loading->getLoadItem()[temp]->getLoadKind())
		{
		case LOAD_KIND_SOUND:
			textOut(getMemDC(), WINSIZEX / 2 - 55, WINSIZEY / 2 - 9, _loading->getLoadItem()[temp]->getSoundResource().fileName);
			break;
		default:
			textOut(getMemDC(), WINSIZEX / 2 - 55, WINSIZEY / 2 - 9, _loading->getLoadItem()[temp]->getImageResource().fileName);
			break;
		}
	}

	int number = (float)_loading->getCurrentGauge() / _loading->getLoadItem().size() * 100;


	if (number < 10)
	{
		IMAGEMANAGER->frameRender("number", getMemDC(), WINSIZEX / 2, WINSIZEY / 2 - 17, number, 0);
		IMAGEMANAGER->render("%", getMemDC(), WINSIZEX / 2 + 19, WINSIZEY / 2 - 10);
	}
	else if (number < 100)
	{
		IMAGEMANAGER->frameRender("number", getMemDC(), WINSIZEX / 2-9, WINSIZEY / 2 - 17, number / 10, 0);
		IMAGEMANAGER->frameRender("number", getMemDC(), WINSIZEX / 2+9, WINSIZEY / 2 - 17, number % 10, 0);
		IMAGEMANAGER->render("%", getMemDC(), WINSIZEX / 2 + 28, WINSIZEY / 2 - 10);
	}
}
void loadingScene::loadingImage()
{
	//IMAGEMANAGER->addImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY);
	//IMAGEMANAGER->addImage("ball", "Images/ball.bmp", 60, 60);
	//�ε��� �ʹ� ������ ����Ǽ� õõ�� ���ư����� �׽�Ʈ������ �����
	for (int i = 0; i < 100; i++)
	{
		_loading->loadImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
		_loading->loadImage("ball", "Images/ball.bmp", 60, 60, true, RGB(255, 0, 255));
	}
}

void loadingScene::loadingSound()
{
	for (int i = 0; i < 20; i++)
	{
		_loading->loadSound("bgm", "Sound/bgm.mp3");
	}
}