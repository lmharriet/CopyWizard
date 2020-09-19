#pragma once
#include "image.h"

//백버퍼 이미지를 이미지매니져에 담아두기
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

class gameNode
{
private:
	HDC _hdc;				//HDC

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//백버퍼 이미지 얻기
	image* getBackBuffer() { return _backBuffer; }
	//화면 HDC 얻기
	HDC getHDC() { return _hdc; }
	//메모리 DC 얻기
	HDC getMemDC() { return _backBuffer->getMemDC(); }

	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	gameNode() {}
	virtual ~gameNode() {}
};