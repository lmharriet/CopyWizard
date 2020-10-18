#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);				//HDC 얻기
	INPUT->init();						//입력매니져 초기화
	RANDOM->init();						//랜덤매니져 초기화
	IMAGEMANAGER->init();				//이미지매니져 초기화
	PLAYERDATA->init();					//플레이어데이터 초기화
	SCENEMANAGER->init();				//씬매니져 초기화
	SOUNDMANAGER->init();				//사운드매니져 초기화
	TIME->init();						//타임매니져 초기화
	
	EFFECT->init();
	ITEM->init();
	DAMAGE->init();
	//PORTAL->init();
	//DROP->init();
	myFont = CreateFont(25, 0, 0, 0, 1000, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "font/MunroSmall.ttf");
	oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void gameNode::release()
{
	//타이머 해제
	KillTimer(_hWnd, 1);

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);

	//입력매니져 해제
	INPUT->release();
	INPUT->releaseSingleton();
	//랜덤매니져 해제
	RANDOM->release();
	RANDOM->releaseSingleton();
	//이미지매니져 해제
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//플레이어데이터 해제
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
	//씬매니져 해제
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//사운드매니져 해제
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//카메라매니져 해제
	CAMERAMANAGER->releaseSingleton();
	//타임매니져 해제
	TIME->release();
	TIME->releaseSingleton();

	EFFECT->releaseSingleton();
	ITEM->releaseSingleton();
	DROP->releaseSingleton();
	DAMAGE->releaseSingleton();

	//HDC 해제
	ReleaseDC(_hWnd, _hdc);
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void gameNode::update()
{
	//새로고침 (나중에 고성능 타이머를 만든후에는 사용하지 않는다)
	//더블버퍼링 이후 사용하지 않는다 (true => false)
	InvalidateRect(_hWnd, NULL, FALSE);
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void gameNode::render()
{
}

void gameNode::zRender(int index)
{
}

//=============================================================
//	## 메인프로시져 ## MainProc()
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