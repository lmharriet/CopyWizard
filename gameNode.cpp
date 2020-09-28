#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);				//HDC ���
	SetTimer(_hWnd, 1, 10, NULL);		//Ÿ�̸� �ʱ�ȭ
	INPUT->init();						//�Է¸Ŵ��� �ʱ�ȭ
	RANDOM->init();						//�����Ŵ��� �ʱ�ȭ
	IMAGEMANAGER->init();				//�̹����Ŵ��� �ʱ�ȭ
	PLAYERDATA->init();					//�÷��̾���� �ʱ�ȭ
	SCENEMANAGER->init();				//���Ŵ��� �ʱ�ȭ
	SOUNDMANAGER->init();				//����Ŵ��� �ʱ�ȭ
	TIME->init();						//Ÿ�ӸŴ��� �ʱ�ȭ

	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void gameNode::release()
{
	//Ÿ�̸� ����
	KillTimer(_hWnd, 1);
	//�Է¸Ŵ��� ����
	INPUT->release();
	INPUT->releaseSingleton();
	//�����Ŵ��� ����
	RANDOM->release();
	RANDOM->releaseSingleton();
	//�̹����Ŵ��� ����
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//�÷��̾���� ����
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
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