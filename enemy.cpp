#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init(const char * imageName, POINT position)
{
	//������ ���� �ʱ�ȭ
	_count = 0;
	_currentFrameX = _currentFrameY = 0;

	//�̹���, �̹�����Ʈ �ʱ�ȭ
	_image = IMAGEMANAGER->findImage(imageName);

	posX = position.x;
	posY = position.y;

	_rc = RectMakeCenter(posX, posY, _image->getFrameWidth(), _image->getFrameHeight());
	area = RectMakeCenter(posX, posY, _image->getFrameWidth() * 5, _image->getFrameHeight() * 5);

	//�������� �Ѿ� ��Ÿ�� �ִ� ���� �ʱ�ȭ
	_fireCount = 0;
	_rndFireCount = RANDOM->range(1, 1000);

	isDetect = isAttack = false;
	time = 0;

	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	this->move();
	this->animation();
}

void enemy::render()
{
	this->draw();
}

//�˾Ƽ� ����� �ȴ�
void enemy::move()
{
}

void enemy::draw()
{
	//_image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
}

void enemy::animation()
{
	_count++;
	if (_count % 3 == 0)
	{
		_image->setFrameX(_image->getFrameX() + 1);
		_currentFrameX++;
		if (_currentFrameX >= _image->getMaxFrameX())
		{
			_currentFrameX = 0;
			_count = 0;
		}
	}
}

bool enemy::bulletCountFire()
{
	_fireCount++;
	if (_fireCount % _rndFireCount == 0)
	{
		_rndFireCount = RANDOM->range(1, 1000);
		_fireCount = 0;

		return true;
	}
	return false;
}