#include "stdafx.h"
#include "camera.h"

HRESULT camera::init()
{
	//백그라운드 이미지 초기화
	_img = IMAGEMANAGER->addImage("백그라운드", "블랙핑크.bmp", WINSIZEX, WINSIZEY);
	//카메라 렉트 위치 초기화
	_rcCam = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, CAMSIZEX, CAMSIZEY);

	return S_OK;
}

void camera::release()
{
}

void camera::update()
{
	//토글키로 백그라운드와 카메라를 스위칭 하면서 조정해준다
	if (INPUT->GetToggleKey(VK_SPACE))
	{
		//이미지를 직접 이동
		if (INPUT->GetKey('A'))
		{
			_img->setX(_img->getX() + 5);
		}
		if (INPUT->GetKey('D'))
		{
			_img->setX(_img->getX() - 5);
		}
		if (INPUT->GetKey('W'))
		{
			_img->setY(_img->getY() + 5);
		}
		if (INPUT->GetKey('S'))
		{
			_img->setY(_img->getY() - 5);
		}
	}
	else
	{
		//카메라를 직업 이동
		if (INPUT->GetKey('A') && _rcCam.left > 0)
		{
			_rcCam.left -= 5;
			_rcCam.right -= 5;
		}
		if (INPUT->GetKey('D') && _rcCam.right < WINSIZEX)
		{
			_rcCam.left += 5;
			_rcCam.right += 5;
		}
		if (INPUT->GetKey('W') && _rcCam.top > 0)
		{
			_rcCam.top -= 5;
			_rcCam.bottom -= 5;
		}
		if (INPUT->GetKey('S') && _rcCam.bottom < WINSIZEY)
		{
			_rcCam.top += 5;
			_rcCam.bottom += 5;
		}
	}

}

void camera::render()
{
	//백그라운 렌더
	_img->render(getMemDC(), _rcCam.left, _rcCam.top, _img->getX() + _rcCam.left, _img->getY() + _rcCam.top, CAMSIZEX, CAMSIZEY);

	//카메라 렌더
	FrameRect(getMemDC(), _rcCam, RGB(255, 255, 0));
}
