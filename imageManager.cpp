#include "stdafx.h"
#include "imageManager.h"

HRESULT imageManager::init()
{
	return S_OK;
}

void imageManager::release()
{
	this->deleteAll();
}

image * imageManager::addImage(string strKey, int width, int height)
{
	//추가하려는 이미지가 존재하는지 키값으로 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면
	//새로 만들지 않고 해당 이미지만 리턴시킨다
	if (img) return img;

	//해당 이미지가 없으니 새로 생성후 초기화 하자
	img = new image;
	if (FAILED(img->init(width, height)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}

	//맵이미지 리스트에 추가
	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addImage(string strKey, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//추가하려는 이미지가 존재하는지 키값으로 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면
	//새로 만들지 않고 해당 이미지만 리턴시킨다
	if (img) return img;

	//해당 이미지가 없으니 새로 생성후 초기화 하자
	img = new image;
	if (FAILED(img->init(fileName, width, height, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}

	//맵이미지 리스트에 추가
	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addImage(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//추가하려는 이미지가 존재하는지 키값으로 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면
	//새로 만들지 않고 해당 이미지만 리턴시킨다
	if (img) return img;

	//해당 이미지가 없으니 새로 생성후 초기화 하자
	img = new image;
	if (FAILED(img->init(fileName, x, y, width, height, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}

	//맵이미지 리스트에 추가
	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//추가하려는 이미지가 존재하는지 키값으로 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면
	//새로 만들지 않고 해당 이미지만 리턴시킨다
	if (img) return img;

	//해당 이미지가 없으니 새로 생성후 초기화 하자
	img = new image;
	if (FAILED(img->init(fileName, width, height, frameX, frameY, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}

	//맵이미지 리스트에 추가
	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//추가하려는 이미지가 존재하는지 키값으로 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면
	//새로 만들지 않고 해당 이미지만 리턴시킨다
	if (img) return img;

	//해당 이미지가 없으니 새로 생성후 초기화 하자
	img = new image;
	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}

	//맵이미지 리스트에 추가
	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	//검색한 키를 찾았따면 이미지클래스 리턴
	if (key != _mImageList.end())
	{
		//key->first => 키
		return key->second; //해당 데이터 = 값(이미지 클래스)
	}

	return nullptr;
}

BOOL imageManager::deleteImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	//검색한 키를 찾았다면 이미지를 삭제하자
	if (key != _mImageList.end())
	{
		//이미지 해제
		key->second->release();
		//메모리 해제
		SAFE_DELETE(key->second);
		//맵의 반복자 해제
		_mImageList.erase(key);

		return true;
	}

	return false;
}

BOOL imageManager::deleteAll()
{
	//맵 전체를 돌면서 하나씩 전부 지운다
	mapImageIter iter = _mImageList.begin();

	//for (;;) => while(true)
	for (;iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	//맵 전체를 삭제
	_mImageList.clear();

	return true;
}

//=============================================================
//	## 일반렌더 ##
//=============================================================
void imageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

//=============================================================
//	## 알파렌더 ##
//=============================================================
void imageManager::alphaRender(string strKey, HDC hdc, BYTE alpha)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha);
}

void imageManager::alphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, destX, destY, alpha);
}

void imageManager::alphaRender(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight, alpha);
}

//=============================================================
//	## 프레임렌더 ##
//=============================================================
void imageManager::frameRender(string strKey, HDC hdc, int destX, int destY)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->frameRender(hdc, destX, destY);
}

void imageManager::frameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

//=============================================================
//	## 루프렌더 ##
//=============================================================
void imageManager::loopRender(string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->loopRender(hdc, drawArea, offsetX, offsetY);
}

void imageManager::loopAlphaRender(string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	//이미지 찾아서 그냥 이미지클래스의 함수를 실행한다
	image* img = findImage(strKey);
	if (img) img->loopAlphaRender(hdc, drawArea, offsetX, offsetY, alpha);
}
