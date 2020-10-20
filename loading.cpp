#include "stdafx.h"
#include "loading.h"

HRESULT loadItem::init(string strKey, int width, int height)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_0;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_1;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_2;

	//이미지 구조체 초기화
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

HRESULT loadItem::init(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_0;

	//이미지 구조체 초기화
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

HRESULT loadItem::init(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_1;

	//이미지 구조체 초기화
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

HRESULT loadItem::init(string strKey, const char* fileName, bool isBGM )
{
	//로딩종류 초기화
	_kind = LOAD_KIND_SOUND;

	//sound 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = NULL;
	_imageResource.height = NULL;
	_imageResource.isTrans = isBGM;
	_imageResource.transColor = NULL;

	return S_OK;
}

//=============================================================
//	## loading ## (로딩클래스 - 로딩화면 구현하기, 로딩바, 백그라운드)
//=============================================================
HRESULT loading::init()
{
	//로딩화면 백그라운드 이미지 초기화
	_background = IMAGEMANAGER->addImage("LoadingBackground", "resource/UI/loadingBackground.bmp", WINSIZEX, WINSIZEY);
	//로딩바 이미지 초기화
	IMAGEMANAGER->addImage("loadingBarFront", "Images/loadingBarFront.bmp", 600, 20);
	IMAGEMANAGER->addImage("loadingBarBack", "Images/loadingBarBack.bmp", 600, 20);

	//플레이어 이미지 초기화
	_char = IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	_enemy = IMAGEMANAGER->addFrameImage("enemyFrame", "resource/enemy/loading.bmp",  1233, 202,5, 1 );
	//로딩바 클래스 초기화
	_loadingBar = new progressBar;
	_loadingBar->init("loadingBarFront", "loadingBarBack");
	//로딩바 위치 초기화
	_loadingBar->setPos(WINSIZEX/2-300, 500);
	
	//현재 게이지
	_currentGauge = 0;
	_currentFrameX = 0;
	_currentEnemyFrameX = 0;

	_playerMaxX = 700.f;
	_playerCurrentX = 0.f;

	return S_OK;
}

void loading::release()
{
	//로딩바 해제
	SAFE_DELETE(_loadingBar);
}

void loading::update()
{
	//로딩바 클래스 업데이트
	_loadingBar->update();
}

void loading::render()
{
	//백그라운드 이미지 렌더
	_background->render(getMemDC());
	//로딩바 클래스 렌더
	//_loadingBar->render();
	
	
	_enemy->frameRender(getMemDC(),_loadingBar->getPosX()+_loadingBar->getWidth()-120,_loadingBar->getPosY()-_enemy->getFrameHeight()+140, _currentEnemyFrameX,0);
	_char->frameRender(getMemDC(),_loadingBar->getPosX()+ _playerCurrentX,_loadingBar->getPosY()-_char->getFrameHeight()/2+100, _currentFrameX,3);
	//_enemy->frameRender(getMemDC(),_loadingBar->getPosX()+_loadingBar->getWidth(),_loadingBar->getPosY()-_enemy->getFrameHeight()/2);
	
	if(_currentGauge < _vLoadItem.size())
	TextOut(getMemDC(), _loadingBar->getPosX() + _loadingBar->getWidth()-100, WINSIZEY/2+200, _vLoadItem[_currentGauge]->getImageResource().keyName.c_str(),strlen(_vLoadItem[_currentGauge]->getImageResource().keyName.c_str()));
}

void loading::loadImage(string strKey, int width, int height)
{
	loadItem* item = new loadItem;
	item->init(strKey, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, x, y, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadSound(string strKey, const char* fileName, bool isBGM)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, isBGM);
	_vLoadItem.push_back(item);
}

bool loading::loadingDone()
{
	//로딩완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return true;
	}

	loadItem* item = _vLoadItem[_currentGauge];
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

		//여러분들이 한번 만들어 보기
		case LOAD_KIND_SOUND:
		{
			tagImageResource img = item->getImageResource();
			SOUNDMANAGER->addSound(img.keyName, img.fileName,img.isTrans,img.isTrans); //loop랑 bgm이랑 같은걸로 되어있음. 
		}
		break;
	}
	
	//현재 게이지 증가
	_currentGauge++;
	

		


	if (_currentGauge % 2 == 0)// 플레이어 이미지
	{
		_currentFrameX++;
		if(_currentGauge%4 ==0)
			_currentEnemyFrameX++;
		if (_currentFrameX > 9)
			_currentFrameX = 0;
			
		if(_currentEnemyFrameX >4)
			_currentEnemyFrameX = 0;

		if (_playerMaxX >= _playerCurrentX)// 플레이어 위치
			_playerCurrentX += 50.f;
		/*else
			_currentFrameY = */
	}
	

	//로딩바 이미지 변경
	_loadingBar->setGauge(_vLoadItem.size(), _currentGauge);

	return false;
}
