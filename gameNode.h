#pragma once
#include "image.h"

//����� �̹����� �̹����Ŵ����� ��Ƶα�
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

	virtual void zRender(int index);

	//����� �̹��� ���
	image* getBackBuffer() { return _backBuffer; }
	//ȭ�� HDC ���
	HDC getHDC() { return _hdc; }
	//�޸� DC ���
	HDC getMemDC() { return _backBuffer->getMemDC(); }

	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	gameNode() {}
	virtual ~gameNode() {}
};