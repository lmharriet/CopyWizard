#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init(const char * imageName, POINT position)
{
	//프레임 변수 초기화
	_count = 0;
	_currentFrameX = _currentFrameY = 0;

	//이미지, 이미지렉트 초기화
	_image = IMAGEMANAGER->findImage(imageName);
	_rc = RectMakeCenter(position.x, position.y, _image->getFrameWidth(), _image->getFrameHeight());

	//랜덤으로 총알 쿨타임 주는 변수 초기화
	_fireCount = 0;
	_rndFireCount = RANDOM->range(1, 1000);

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

//알아서 만들면 된다
void enemy::move()
{
}

void enemy::draw()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
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
