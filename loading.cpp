#include "stdafx.h"
#include "loading.h"

HRESULT loadItem::init(string strKey, int width, int height)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_0;

	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_1;

	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_2;

	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_FRAMEIMAGE_0;

	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_FRAMEIMAGE_1;

	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char* fileName, bool isBGM)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_SOUND;

	//sound ����ü �ʱ�ȭ
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = NULL;
	_imageResource.height = NULL;
	_imageResource.isTrans = isBGM;
	_imageResource.transColor = NULL;

	return S_OK;
}

//=============================================================
//	## loading ## (�ε�Ŭ���� - �ε�ȭ�� �����ϱ�, �ε���, ��׶���)
//=============================================================
HRESULT loading::init(string keyName, const char* fileName, int imgWidth, int imgHeight)
{
	//�ε�ȭ�� ��׶��� �̹��� �ʱ�ȭ
	_background = IMAGEMANAGER->addImage(keyName, fileName, imgWidth, imgHeight);
	//�ε��� �̹��� �ʱ�ȭ
	IMAGEMANAGER->addImage("loadingBarFront", "Images/loadingBarFront.bmp", 1190, 20);
	IMAGEMANAGER->addImage("loadingBarBack", "Images/loadingBarBack.bmp", 1190, 20);

	//�÷��̾� �̹��� �ʱ�ȭ
	_char = IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	_enemy = IMAGEMANAGER->addFrameImage("enemyFrame", "resource/UI/loading.bmp", 1233, 202, 5, 1);
	//�ε��� Ŭ���� �ʱ�ȭ
	_loadingBar = make_unique<progressBar>();
	_loadingBar->init("loadingBarFront", "loadingBarBack");
	//�ε��� ��ġ �ʱ�ȭ
	_loadingBar->setPos(0, 500);

	//���� ������
	_currentGauge = 0;
	_currentFrameX = 0;
	_currentEnemyFrameX = 0;

	_playerMaxX = 1300.f;
	_playerCurrentX = 0.f;


	return S_OK;
}

void loading::release()
{
	//�ε��� ����
	//SAFE_DELETE(_loadingBar);
}

void loading::update()
{
	//�ε��� Ŭ���� ������Ʈ
	_loadingBar->update();
}

void loading::render()
{
	//��׶��� �̹��� ����
	_background->render(getMemDC());
	//�ε��� Ŭ���� ����
	//_loadingBar->render();


	_enemy->frameRender(getMemDC(), _loadingBar->getPosX() + _loadingBar->getWidth() - 120, _loadingBar->getPosY() - _enemy->getFrameHeight() + 140, _currentEnemyFrameX, 0);
	_char->frameRender(getMemDC(), _loadingBar->getPosX() + _playerCurrentX, _loadingBar->getPosY() - _char->getFrameHeight() / 2 + 100, _currentFrameX, 3);

	if (_currentGauge < _vLoadItem.size())
		TextOut(getMemDC(), _loadingBar->getPosX() + _loadingBar->getWidth() - 100, WINSIZEY / 2 + 280, _vLoadItem[_currentGauge]->getImageResource().keyName.c_str(), strlen(_vLoadItem[_currentGauge]->getImageResource().keyName.c_str()));
}

void loading::loadImage(string strKey, int width, int height)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, width, height);
	_vLoadItem.push_back(move(item));
}

void loading::loadImage(string strKey, const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(move(item));
}

void loading::loadImage(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(move(item));
}

void loading::loadFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, fileName, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(move(item));
}

void loading::loadFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, fileName, width, x, y, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(move(item));
}

void loading::loadSound(string strKey, const char* fileName, bool isBGM)
{
	shared_ptr<loadItem> item = make_shared<loadItem>();
	item->init(strKey, fileName, isBGM);
	_vLoadItem.push_back(move(item));
}

bool loading::loadingDone()
{
	//�ε��Ϸ��
	if (_currentGauge >= _vLoadItem.size())
	{
		return true;
	}

	shared_ptr<loadItem> item = move(_vLoadItem[_currentGauge]);
	switch (item->getLoadKind())
	{
	case LOAD_KIND_IMAGE_0:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.width, img.height);
	}
	break;

	case LOAD_KIND_IMAGE_1:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.fileName, img.width, img.height, img.isTrans, img.transColor);
	}
	break;

	case LOAD_KIND_IMAGE_2:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.isTrans, img.transColor);
	}
	break;

	case LOAD_KIND_FRAMEIMAGE_0:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.width, img.height, img.frameX, img.frameY, img.isTrans, img.transColor);
	}
	break;

	case LOAD_KIND_FRAMEIMAGE_1:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.frameX, img.frameY, img.isTrans, img.transColor);
	}
	break;

	//�����е��� �ѹ� ����� ����
	case LOAD_KIND_SOUND:
	{
		tagImageResource img = item->getImageResource();
		SOUNDMANAGER->addSound(img.keyName, img.fileName, img.isTrans, img.isTrans); //loop�� bgm�̶� �����ɷ� �Ǿ�����. 
	}
	break;
	}

	//���� ������ ����
	_currentGauge++;





	if (_currentGauge % 2 == 0)// �÷��̾� �̹���
	{
		_currentFrameX++;
		if (_currentGauge % 4 == 0)
			_currentEnemyFrameX++;
		if (_currentFrameX > 9)
			_currentFrameX = 0;

		if (_currentEnemyFrameX > 4)
			_currentEnemyFrameX = 0;

		if (_playerMaxX >= _playerCurrentX)// �÷��̾� ��ġ
			_playerCurrentX += 50.f;
		/*else
			_currentFrameY = */
	}


	//�ε��� �̹��� ����
	_loadingBar->setGauge(_vLoadItem.size(), _currentGauge);

	return false;
}
