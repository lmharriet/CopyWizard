#include "stdafx.h"
#include "camera.h"

HRESULT camera::init()
{
	//��׶��� �̹��� �ʱ�ȭ
	_img = IMAGEMANAGER->addImage("��׶���", "����ũ.bmp", WINSIZEX, WINSIZEY);
	//ī�޶� ��Ʈ ��ġ �ʱ�ȭ
	_rcCam = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, CAMSIZEX, CAMSIZEY);

	return S_OK;
}

void camera::release()
{
}

void camera::update()
{
	//���Ű�� ��׶���� ī�޶� ����Ī �ϸ鼭 �������ش�
	if (INPUT->GetToggleKey(VK_SPACE))
	{
		//�̹����� ���� �̵�
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
		//ī�޶� ���� �̵�
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
	//��׶�� ����
	_img->render(getMemDC(), _rcCam.left, _rcCam.top, _img->getX() + _rcCam.left, _img->getY() + _rcCam.top, CAMSIZEX, CAMSIZEY);

	//ī�޶� ����
	FrameRect(getMemDC(), _rcCam, RGB(255, 255, 0));
}
