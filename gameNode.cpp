#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);				//HDC ���
	INPUT->init();						//�Է¸Ŵ��� �ʱ�ȭ
	RANDOM->init();						//�����Ŵ��� �ʱ�ȭ
	IMAGEMANAGER->init();				//�̹����Ŵ��� �ʱ�ȭ
	INIDATA->init();					//ini������ �ʱ�ȭ
	SCENEMANAGER->init();				//���Ŵ��� �ʱ�ȭ
	SOUNDMANAGER->init();				//����Ŵ��� �ʱ�ȭ
	TIME->init();						//Ÿ�ӸŴ��� �ʱ�ȭ
	
	EFFECT->init();
	ITEM->init();
	DAMAGE->init();
	PORTAL->init();
	//DROP->init();
	myFont = CreateFont(25, 0, 0, 0, 1000, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "font/MunroSmall.ttf");
	oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void gameNode::release()
{
	//Ÿ�̸� ����
	KillTimer(_hWnd, 1);

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);

	//�Է¸Ŵ��� ����
	INPUT->release();
	INPUT->releaseSingleton();
	//�����Ŵ��� ����
	RANDOM->release();
	RANDOM->releaseSingleton();
	//�̹����Ŵ��� ����
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//ini�Ŵ��� ����
	INIDATA->release();
	INIDATA->releaseSingleton();
	//�÷��̾���� ����
	//PLAYERDATA->release();
	//PLAYERDATA->releaseSingleton();
	//���Ŵ��� ����
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//����Ŵ��� ����
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//ī�޶�Ŵ��� ����
	CAMERAMANAGER->releaseSingleton();
	//Ÿ�ӸŴ��� ����
	TIME->release();
	TIME->releaseSingleton();

	EFFECT->releaseSingleton();
	ITEM->releaseSingleton();
	DROP->releaseSingleton();
	DAMAGE->releaseSingleton();

	//HDC ����
	ReleaseDC(_hWnd, _hdc);
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void gameNode::update()
{
	//���ΰ�ħ (���߿� ���� Ÿ�̸Ӹ� �����Ŀ��� ������� �ʴ´�)
	//������۸� ���� ������� �ʴ´� (true => false)
	InvalidateRect(_hWnd, NULL, FALSE);
}

//=============================================================
//	## ���� ## render()
//=============================================================
void gameNode::render()
{
}

void gameNode::zRender(int index)
{
}

//=============================================================
//	## �������ν��� ## MainProc()
//=============================================================
LRESULT gameNode::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		this->update();
		break;
	case WM_PAINT:
		this->render();
		break;
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		if ((SHORT)HIWORD(wParam) > 0) {
			_mouseWheel = 1;
		}
		else if ((SHORT)HIWORD(wParam) < 0) {
			_mouseWheel = -1;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}