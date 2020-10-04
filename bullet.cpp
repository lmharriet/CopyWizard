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

}

void homingFlares::update()
{
	this->move();
}

void homingFlares::render()
{
	for (int i = 0; i < _vFlares.size(); i++)
	{
		CAMERAMANAGER->Ellipse(getMemDC(), _vFlares[i].rc);
		CAMERAMANAGER->Ellipse(getMemDC(), _vRange[i] .rc);
	}


}
//총알발사
void homingFlares::fire(float x, float y, float angle)
{
	tagArcana flares;
	flares.angle = angle;
	flares.speed = 20.f;
	flares.x = flares.fireX = x + cosf(flares.angle) * (flares.speed);
	flares.y = flares.fireY = y - sinf(flares.angle) * (flares.speed);
	flares.rc = RectMakeCenter(flares.x, flares.y, 30, 30);

	_vFlares.push_back(flares);

	tagArcana range;
	range.angle = angle;
	range.speed = 100;
	range.x = range.fireX = x + cosf(range.angle) * range.speed;
	range.y = range.fireY = y - sinf(range.angle) * range.speed;
	range.rc = RectMakeCenter(range.x, range.y, 40, 40);

	_vRange.push_back(range);


}

//총알무브
void homingFlares::move()
{
	for (int i = 0; i < _vFlares.size(); i++)
	{
		_vFlares[i].x = _vFlares[i].x + cosf(_vFlares[i].angle) * 5.f;
		_vFlares[i].y = _vFlares[i].y - sinf(_vFlares[i].angle) * 5.f;
		_vFlares[i].rc = RectMakeCenter(_vFlares[i].x, _vFlares[i].y, 30, 30);

		if (colCheck(_vFlares[i].rc, _vRange[i].rc))
		{
			
			
		}
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
HRESULT meteor::init(float range)
{
	_range = range;
	angleRange = meteorCount = isUlt = 0;
	ranCount = 10;

	count = index = timer = 0;
	return S_OK;
}

void meteor::release()
{
}

void meteor::update()
{
	timer++;

	count++;
	if (count % 5 == 0)
	{
		index++;
		if (index > 5) index = 0;
	}
	move();

	meteorUlt();
}

void meteor::render()
{
	char temp[126];

	for (int i = 0; i < _vMeteor.size(); i++)
	{
		//CAMERAMANAGER->Ellipse(getMemDC(), _vMeteor[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), _vMeteor[i].img, _vMeteor[i].rc.left, _vMeteor[i].rc.top, index, _vMeteor[i].frameY);
	}

	/*wsprintf(temp, "size : %d", _vMeteor.size());
	textOut(getMemDC(), 100, 300, temp, RGB(255, 255, 255));*/
}

void meteor::meteorFire(float x, float y, float speed, MOVE dir, float range)
{
	//angleRange = RANDOM->range(PI / 9, angle);
	float angle = .0f;
	float spd = .0f;

	tagArcana meteor;

	meteor.range = range;
	switch (dir)
	{
	case MOVE::LEFT:
		angle = 70.f * (PI / 180);
		meteor.angle = angle + PI - .4f;
		meteor.frameY = 0;

		meteor.x = meteor.fireX = x + cosf(angle) * speed;
		meteor.y = meteor.fireY = y - sinf(angle) * speed;
		break;
	case MOVE::RIGHT:
		angle = 110.f * (PI / 180);
		meteor.angle = angle + PI + .4f;
		meteor.frameY = 1;

		meteor.x = meteor.fireX = x + cosf(angle) * speed;
		meteor.y = meteor.fireY = y - sinf(angle) * speed;
		break;
	case MOVE::UP:
		angle = 110.f * (PI / 180);
		meteor.angle = angle + PI + .4f;
		meteor.frameY = 1;

		spd = speed + 300;

		meteor.x = meteor.fireX = x + cosf(angle) * spd;
		meteor.y = meteor.fireY = y - sinf(angle) * spd;
		break;
	case MOVE::DOWN:
		angle = 70.f * (PI / 180);
		meteor.angle = angle + PI - .4f;
		meteor.frameY = 0;

		spd = speed - 300;

		meteor.x = meteor.fireX = x + cosf(angle) * spd;
		meteor.y = meteor.fireY = y - sinf(angle) * spd;
		break;
	default:
		return;
	}

	//memset(&meteor, 0, sizeof(meteor));

	meteor.speed = 12.5f;

	meteor.rc = RectMakeCenter(meteor.x, meteor.y, 200 * 2, 160 * 2);
	meteor.img = IMAGEMANAGER->findImage("meteor");
	meteor.lifeTime = 0;
	//if (_vMeteor.size() >= _bulletMax) continue;
	_vMeteor.push_back(meteor);
}

void meteor::meteorUlt()
{
	if (isUlt && timer % ranCount == 0)
	{
		ranCount = RANDOM->range(15, 27);

		meteorCount++;

		tagArcana tmpMeteor = ult;

		int ranX = 0, ranY = 0;

		switch (tmpMeteor.dir)
		{
		case MOVE::LEFT:
			ranX = RANDOM->range(-100, 0);

			ranY = RANDOM->range(-150, 150);
			break;
		case MOVE::RIGHT:
			ranX = RANDOM->range(0, 100);

			ranY = RANDOM->range(-150, 150);
			break;
		case MOVE::UP:
			ranX = RANDOM->range(-400, -200);

			ranY = RANDOM->range(-100, 0);
			break;
		case MOVE::DOWN:
			ranX = RANDOM->range(300, 500);

			ranY = RANDOM->range(0, 100);
			break;
		default:
			ranX = RANDOM->range(-100, 0);

			ranY = RANDOM->range(-150, 150);
			break;
		}

		tmpMeteor.x += ranX;
		tmpMeteor.fireX += ranX;

		tmpMeteor.y += ranY;
		tmpMeteor.fireY += ranY;

		tmpMeteor.rc = RectMakeCenter(tmpMeteor.x, tmpMeteor.y, 200 * 2, 160 * 2);

		_vMeteor.push_back(tmpMeteor);

		if (meteorCount == 6)
		{
			isUlt = false;
			ult = { 0, };
			meteorCount = 0;
		}
	}
}

void meteor::meteorUltFire(float x, float y, float speed, MOVE dir, float range)
{
	isUlt = true;

	float angle = .0f;
	float spd = .0f;

	switch (dir)
	{
	case MOVE::LEFT:
		angle = 70.f * (PI / 180);
		ult.angle = angle + PI - .4f;
		ult.frameY = 0;

		ult.x = ult.fireX = x + cosf(angle) * speed;
		ult.y = ult.fireY = y - sinf(angle) * speed;
		break;
	case MOVE::RIGHT:
		angle = 110.f * (PI / 180);
		ult.angle = angle + PI + .4f;
		ult.frameY = 1;

		ult.x = ult.fireX = x + cosf(angle) * speed;
		ult.y = ult.fireY = y - sinf(angle) * speed;
		break;
	case MOVE::UP:
		angle = 110.f * (PI / 180);
		ult.angle = angle + PI + .4f;
		ult.frameY = 1;

		spd = speed + 200;

		ult.x = ult.fireX = x + cosf(angle) * spd;
		ult.y = ult.fireY = y - sinf(angle) * spd;
		break;
	case MOVE::DOWN:
		angle = 70.f * (PI / 180);
		ult.angle = angle + PI - .4f;
		ult.frameY = 0;

		spd = speed - 200;

		ult.x = ult.fireX = x + cosf(angle) * spd;
		ult.y = ult.fireY = y - sinf(angle) * spd;
		break;
	default:
		angle = 70.f * (PI / 180);
		ult.angle = angle + PI - .4f;
		ult.frameY = 0;

		ult.x = ult.fireX = x + cosf(angle) * speed;
		ult.y = ult.fireY = y - sinf(angle) * speed;
		break;
	}

	ult.speed = 12.5f;

	ult.range = range;
	ult.rc = RectMakeCenter(ult.x, ult.y, 200 * 2, 160 * 2);
	ult.img = IMAGEMANAGER->findImage("meteor");
	ult.lifeTime = 0;
	ult.dir = dir;
	ranCount = 10; // 메테오간 딜레이를 랜덤으로 주기위함 (처음에만 10으로 둠)
}

void meteor::move()
{
	for (int i = 0; i < _vMeteor.size(); i++)
	{
		_vMeteor[i].lifeTime++;

		_vMeteor[i].x = _vMeteor[i].x + cosf(_vMeteor[i].angle) * _vMeteor[i].speed;
		_vMeteor[i].y = _vMeteor[i].y + (-sinf(_vMeteor[i].angle)) * _vMeteor[i].speed;
		_vMeteor[i].rc = RectMakeCenter(_vMeteor[i].x, _vMeteor[i].y, 200 * 2, 160 * 2);


		if (_vMeteor[i].lifeTime > _vMeteor[i].range)
		{
			//cout << _vMeteor.size() << '\n';
			//이펙트 플레이
			//충돌 처리 , 카메라 shake
			CAMERAMANAGER->Shake(10, 10, 10);
			_vMeteor.erase(_vMeteor.begin() + i);
		}
	}

}
