#include "stdafx.h"
#include "bullet.h"

//=============================================================
//	## bullet ## (공용총알)
//=============================================================

HRESULT bullet::init(const char* imageName, int bulletMax, float range, bool isFrameImg)
{
	//총알 이미지 초기화
	_imageName = imageName;
	//총알갯수 및 사거리 초기화
	_bulletMax = bulletMax;
	_range = range;
	//총알 이미지가 프레임 이미지냐?
	_isFrameImg = isFrameImg;

	return S_OK;
}

void bullet::release()
{
}

void bullet::update()
{
	this->move();
}

void bullet::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		CAMERAMANAGER->Rectangle(getMemDC(), _vBullet[i].rc);
	}
}

void bullet::fire(float x, float y, float angle, float speed)
{
	//총알 벡터에 담는것을 제한한다
	//if (_bulletMax < _vBullet.size() + 1) return;

	//총알 구조체 선언
	tagBullet bullet;
	//총알 구조체 초기화
	//제로메모리, 멤셋
	//구조체 변수들의 값을 한번에 0으로 초기화 시켜준다
	//ZeroMemory(&bullet, sizeof(tagBullet));
	//bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.radius = 25;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y, 50, 50);

	//벡터에 담기
	_vBullet.push_back(bullet);
}

void bullet::move()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;

		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 50, 50);

		//총알이 사거리 보다 커졌을때
		//float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
		//	_vBullet[i].x, _vBullet[i].y);
		//if (_range < distance)
		//{
		//	_vBullet.erase(_vBullet.begin() + i);
		//}
	}
}
//총알삭제
void bullet::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

//=============================================================
//	## homingFlares ## (homingFlares[0] -> 배열처럼 미리 장전해두고 총알발사)
//=============================================================

HRESULT homingFlares::init(float range)
{
	_range = range;

	////총알의 갯수만큼 구조체를 초기화 한 후 벡터에 담기
	//for (int i = 0; i < bulletMax; i++)
	//{
	//	//총알 구조체 선언
	//	tagBullet bullet;
	//	//총알 구조체 초기화
	//	//제로메모리, 멤셋
	//	//구조체 변수들의 값을 한번에 0으로 초기화 시켜준다
	//	ZeroMemory(&bullet, sizeof(tagBullet));
	//	//bullet.bulletImage = IMAGEMANAGER->findImage();
	//	bullet.bulletImage = new image;
	//	bullet.bulletImage->init("Images/missile.bmp", 26, 124, true, RGB(255, 0, 255));
	//	bullet.speed = 5.0f;
	//	bullet.fire = false;

	//	//벡터에 담기
	//	_vBullet.push_back(bullet);
	//}

	return S_OK;
}

void homingFlares::release()
{
	/*for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].bulletImage->release();
		SAFE_DELETE(_vBullet[i].bulletImage);
	}*/
}

void homingFlares::update()
{
	this->move();
}

void homingFlares::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		CAMERAMANAGER->Rectangle(getMemDC(), _vBullet[i].rc);
	}

	char vec[126];
	wsprintf(vec, "vecsize : %d", _vBullet.size());
	textOut(getMemDC(), 10, 300, vec, RGB(255, 255, 255));
}
//총알발사
void homingFlares::fire(float x, float y)
{

	for (int i = 0; i < 7; i++)
	{
		tagBullet flares;

		memset(&flares, 0, sizeof(flares));
		//flares.bulletImage
		flares.x = flares.fireX = x;
		flares.y = flares.fireY = y;
		flares.speed = 5.f;
		flares.angle = 0;
		flares.rc = RectMakeCenter(flares.x, flares.y, 30, 30);

		_vBullet.push_back(flares);
	}
}

//총알무브
void homingFlares::move()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].angle = (52 * i) + PI;
		_vBullet[i].speed = 7.f;
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);

		//if (_vBullet[i].x - _vBullet[i].fireX < 10.f)
		//	_vBullet[i].speed = 0.f;
	}

}

//수정 중(player flame strike)
//=============================================================
//	## bomb ## (폭탄처럼 한발씩 발사하고 생성하고 자동삭제)
//=============================================================
HRESULT bomb::init(int bulletMax, float range)
{
	//총알 사거리 및 총알 갯수 초기화
	_range = range;
	_bulletMax = bulletMax;

	count = index = 0;
	return S_OK;
}

void bomb::release()
{
}

void bomb::update()
{
	count++;
	if (count % 2 == 0)
	{
		index++;
		if (index > 3)index = 0;
	}
	this->move();
}

void bomb::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage,
			_vBullet[i].x - (_vBullet[i].bulletImage->getFrameWidth() / 2),
			_vBullet[i].y - (_vBullet[i].bulletImage->getFrameHeight() / 2), index, 0);

		CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 20, 20));
	}
}

void bomb::fire(float x, float y, float speed, float angle, float radius)
{
	//총알 벡터에 담는것을 제한한다
	if (_bulletMax < _vBullet.size() + 1) return;

	//총알 구조체 선언
	tagBullet bullet;
	//총알 구조체 초기화
	//제로메모리, 멤셋
	//구조체 변수들의 값을 한번에 0으로 초기화 시켜준다


	bullet.bulletImage = IMAGEMANAGER->addFrameImage("blaze", "resource/player/blaze.bmp", 288, 96, 3, 1, true, RGB(255, 0, 255));
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.radius = radius;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y, radius * 2, radius * 2);

	_vBullet.push_back(bullet);
}

void bomb::move()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, _vBullet[i].radius * 2, _vBullet[i].radius * 2);

		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY, _vBullet[i].x, _vBullet[i].y);
		if (_range < distance)
		{
			_vBullet.erase(_vBullet.begin() + i);
		}
	}
}
//폭탄삭제
void bomb::removeBomb(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

//수정 중(player meteor)
//=============================================================
//	## meteor ## (signature)
//=============================================================
HRESULT meteor::init(int bulletMax, float range)
{
	_bulletMax = bulletMax;
	_range = range;
	angleRange = 0;

	count = index = 0;
	return S_OK;
}

void meteor::release()
{
}

void meteor::update()
{
	count++;
	if (count % 5 == 0)
	{
		index++;
		if (index > 5) index = 0;
	}
	move();
}

void meteor::render()
{


	for (int i = 0; i < _vMeteor.size(); i++)
	{
		//CAMERAMANAGER->Rectangle(getMemDC(), _vMeteor[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), _vMeteor[i].bulletImage, _vMeteor[i].rc.left, _vMeteor[i].rc.top, index, 0);
	}

	char temp[126];
	wsprintf(temp, "size : %d", _vMeteor.size());
	textOut(getMemDC(), 100, 300, temp, RGB(255, 255, 255));
}

void meteor::meteorFire(float x, float y, float speed, float angle)
{
	//angleRange = RANDOM->range(PI / 9, angle);


	tagBullet meteor;
	memset(&meteor, 0, sizeof(meteor));

	meteor.bulletImage = IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp",1200*2, 160*2, 6, 1);
	meteor.angle = angle;// angleRange;
	meteor.speed = speed;
	meteor.x = meteor.fireX = x + cosf(meteor.angle) * meteor.speed;
	meteor.y = meteor.fireY = y - sinf(meteor.angle) * meteor.speed;
	meteor.rc = RectMakeCenter(meteor.x, meteor.y, 200*2, 160*2);

	//if (_vMeteor.size() >= _bulletMax) continue;
	_vMeteor.push_back(meteor);

}

void meteor::move()
{
	for (int i = 0; i < _vMeteor.size(); i++)
	{
		_vMeteor[i].angle = PI + PI / 3;
		_vMeteor[i].speed = 10.f;
		_vMeteor[i].x = _vMeteor[i].x + cosf(_vMeteor[i].angle) * _vMeteor[i].speed;
		_vMeteor[i].y = _vMeteor[i].y + (-sinf(_vMeteor[i].angle)) * _vMeteor[i].speed;
		_vMeteor[i].rc = RectMakeCenter(_vMeteor[i].x, _vMeteor[i].y, 200*2, 160*2);


		float distance = getDistance(_vMeteor[i].fireX, _vMeteor[i].fireY, _vMeteor[i].x, _vMeteor[i].y);
		if (distance > _range)
		{
			for (int i = 0; i < _vMeteor.size(); i++)
			{
				_vMeteor.erase(_vMeteor.begin() + i);
			}
		}
	}

}
