#include "stdafx.h"
#include "pixelCollisionScene.h"


HRESULT pixelCollisionScene::init()
{
	//백그라운드, 공이미지 초기화
	_mountain = IMAGEMANAGER->addImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY,true,RGB(255,0,255));
	_ball = IMAGEMANAGER->addImage("ball", "Images/ball.bmp", 60, 60,true, RGB(255, 0, 255));

	/*_mountain = IMAGEMANAGER->findImage("mountain");
	_ball = IMAGEMANAGER->findImage("ball");*/


	//공 위치 초기화
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//Y축으로 탐지(공의 하단에서 검사함)
	_probeY = _y + _ball->getHeight() / 2;

	return S_OK;
}

void pixelCollisionScene::release()
{
}

void pixelCollisionScene::update()
{
	if (INPUT->GetKey(VK_LEFT))
	{
		_x -= 3.0f;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		_x += 3.0f;
	}
	//공의 렉트 움직이기
	_rc = RectMakeCenter(_x, _y, 60, 60);
	_probeY = _y + _ball->getHeight() / 2;

	/*이부분이 픽셀충돌의 핵심*/
	for (int i = _probeY - 50; i < _probeY + 50; i++)
	{
		COLORREF color = GetPixel(_mountain->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		//if (r == 255 && g == 0 && b == 255)
		//{
		//
		//}

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}
	}


}

void pixelCollisionScene::render()
{
	//백그라운드 렌더
	_mountain->render(getMemDC());
	//공 이미지 렌더
	_ball->render(getMemDC(), _rc.left, _rc.top);

	//디버깅용
	if (INPUT->GetToggleKey('A'))
	{
		//충돌용 작은 사각형
		RECT rc = RectMakeCenter(_x, _probeY, 10, 10);
		Rectangle(getMemDC(), rc);
	}

	textOut(getMemDC(), 10, 10, "픽셀충돌");
}
