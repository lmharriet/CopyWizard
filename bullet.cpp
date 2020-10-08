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
		if (_vBullet[i].bulletImage)
			CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage, _vBullet[i].x, _vBullet[i].y,
				_vBullet[i].FrameX, _vBullet[i].FrameY);

	}
}

void bullet::fire(float x, float y, float angle, float speed,int damage, bool image)
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
	if (image)
		bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	else bullet.bulletImage = NULL;


	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.radius = 36;
	bullet.damage = damage;
	bullet.count = 0;
	bullet.FrameX = 0;
	bullet.FrameY = 0;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y, 50, 50);

	//벡터에 담기
	_vBullet.push_back(bullet);
}

void bullet::move()
{

	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].FrameX = 1;
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;

		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 50, 50);

		//총알이 사거리 보다 커졌을때
		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
			_vBullet[i].x, _vBullet[i].y);
		if (_range < distance)
		{
			_vBullet.erase(_vBullet.begin() + i);
		}

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
	/*for (int i = 0; i < _vFlares.size(); i++)
	{
		CAMERAMANAGER->Ellipse(getMemDC(), _vFlares[i].rc);
		CAMERAMANAGER->Ellipse(getMemDC(), _vRange[i] .rc);
	}*/


}
//총알발사
void homingFlares::fire(float x, float y, float angle)
{


	/*tagArcana flares;
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
*/

}

//총알무브
void homingFlares::move()
{
	/*for (int i = 0; i < _vFlares.size(); i++)
	{
		_vFlares[i].x = _vFlares[i].x + cosf(_vFlares[i].angle) * 5.f;
		_vFlares[i].y = _vFlares[i].y - sinf(_vFlares[i].angle) * 5.f;
		_vFlares[i].rc = RectMakeCenter(_vFlares[i].x, _vFlares[i].y, 30, 30);

		if (colCheck(_vFlares[i].rc, _vRange[i].rc))
		{


		}
	}*/
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
	bullet.atkPower = 12;
	bullet.collision = false;
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
			PARTICLE->explosionGenerate("explosionParticle", _vBullet[i].x + 20, _vBullet[i].y + 20, 5, 30, 2.f, 3);
			//_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);
			_vBullet[i].collision = true;
			cout << i << " : " << _vBullet[i].collision << '\n';
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
	circleCount = CircleIndex = 0;
	isCol = false;
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

	circleCount++;
	if (circleCount % 7 == 0)
	{
		CircleIndex++;
		if (CircleIndex > 23)CircleIndex = 23;
	}
	move();

	meteorUlt();
}

void meteor::render()
{

	//test
	image* img = IMAGEMANAGER->addFrameImage("circle", "resource/player/castingCircle1.bmp",3072, 128, 24, 1,true,RGB(255,0,0));
	
	char temp[126];


	for (int i = 0; i < vMeteor.size(); i++)
	{
		//CAMERAMANAGER->Ellipse(getMemDC(), _vMeteor[i].rc);
		//CAMERAMANAGER->FrameRender(getMemDC(), vMeteor[i].img, vMeteor[i].rc.left, vMeteor[i].rc.top, index, vMeteor[i].frameY);
	}

	for (int i = 0; i < vCircle.size(); i++)
	{
		//CAMERAMANAGER->Ellipse(getMemDC(), vCircle[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), img,
			vCircle[i].x - img->getFrameWidth() / 2, vCircle[i].y - img->getFrameHeight() / 2, CircleIndex, 0);
	}
}

void meteor::makeCircle(float x, float y, float radius, MOVE direction)
{
	//플레이어 기준으로 왼쪽 오른쪽 위 아래 마법진 생성

	tagCircle circle;
	switch (direction)
	{
	case MOVE::LEFT:
		//왼쪽
		circle.angle = PI;
		circle.x = x + cosf(circle.angle) * radius;
		circle.y = y - sinf(circle.angle) * radius;
		circle.rc = RectMakeCenter(circle.x, circle.y, radius, radius);
		circle.lifeTime = 0;
		break;
	case MOVE::RIGHT:
		//오른쪽
		circle.angle = 0;
		circle.x = x + cosf(circle.angle) * radius;
		circle.y = y - sinf(circle.angle) * radius;
		circle.rc = RectMakeCenter(circle.x, circle.y, radius, radius);
		circle.lifeTime = 0;
		break;

	case MOVE::UP:
		//위
		circle.angle = PI_2;
		circle.x = x + cosf(circle.angle) * radius;
		circle.y = y - sinf(circle.angle) * radius;
		circle.rc = RectMakeCenter(circle.x, circle.y, radius, radius);
		circle.lifeTime = 0;
		break;
	case MOVE::DOWN:
		//아래
		circle.angle = PI_2 * 3;
		circle.x = x + cosf(circle.angle) * radius;
		circle.y = y - sinf(circle.angle) * radius;
		circle.rc = RectMakeCenter(circle.x, circle.y, radius, radius);
		circle.lifeTime = 0;
		break;
	default:

		circle.angle = 0;
		circle.x = x + cosf(circle.angle) * radius;
		circle.y = y - sinf(circle.angle) * radius;
		circle.rc = RectMakeCenter(circle.x, circle.y, radius, radius);
		circle.lifeTime = 0;
		break;
	}


	vCircle.push_back(circle);
}

void meteor::creatMeteor(float x, float y, float radius, MOVE direction)
{
	tagArcana meteor;

	switch (direction)
	{
	case MOVE::LEFT:
		//왼쪽
		meteor.angle = 70 * (PI / 180);
		meteor.x = x + cosf(meteor.angle) * radius;
		meteor.y = y - sinf(meteor.angle) * radius;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, radius, radius);
		break;
	
	case MOVE::RIGHT:
		//오른쪽
		meteor.angle = 120 * (PI / 180);
		meteor.x = x + cosf(meteor.angle) * radius;
		meteor.y = y - sinf(meteor.angle) * radius;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, radius, radius);
		break;

	case MOVE::UP:
		//위
		meteor.angle = PI_2;
		meteor.x = x + cosf(meteor.angle) * radius + radius / 2;
		meteor.y = y - sinf(meteor.angle) * radius + radius / 2;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, radius, radius);
		break;
	case MOVE::DOWN:
		//아래
		meteor.angle = PI_2;
		meteor.x = x + cosf(meteor.angle) * radius - radius / 2;
		meteor.y = y - sinf(meteor.angle) * radius - radius / 2;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, radius, radius);
		break;
	
	default:
		//오른쪽
		meteor.angle = 120 * (PI / 180);
		meteor.x = x + cosf(meteor.angle) * radius;
		meteor.y = y - sinf(meteor.angle) * radius;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, radius, radius);
		break;
	}
	
	

	
	

	vMeteor.push_back(meteor);

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
	meteor.atkPower = 0;
	//if (_vMeteor.size() >= _bulletMax) continue;
	vMeteor.push_back(meteor);
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

		vMeteor.push_back(tmpMeteor);

		if (meteorCount == 6)
		{
			isCol = true;
			tmpMeteor.atkPower = 1;
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
	ult.atkPower = 1;


	ranCount = 10; // 메테오간 딜레이를 랜덤으로 주기위함 (처음에만 10으로 둠)
}

void meteor::move()
{
	for (int i = 0; i < vMeteor.size(); i++)
	{
		vMeteor[i].lifeTime++;

		vMeteor[i].x = vMeteor[i].x + cosf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].y = vMeteor[i].y + (-sinf(vMeteor[i].angle)) * vMeteor[i].speed;
		vMeteor[i].rc = RectMakeCenter(vMeteor[i].x, vMeteor[i].y, 200 * 2, 160 * 2);


		if (vMeteor[i].lifeTime > vMeteor[i].range)
		{
			//이펙트 플레이
			//충돌 처리 , 카메라 shake
			CAMERAMANAGER->Shake(10, 10, 10);
			vMeteor.erase(vMeteor.begin() + i);
		}
	}

}


//DASH
HRESULT dashFire::init()
{
	return S_OK;
}

void dashFire::release()
{
}

void dashFire::update()
{

}

void dashFire::render()
{
	for (int i = 0; i < _vDash.size();)
	{
		_vDash[i].lifeTime++;

		//CAMERAMANAGER->Ellipse(getMemDC(), _vDash[i].rc);
		image* img = IMAGEMANAGER->findImage("flame");

		CAMERAMANAGER->FrameRender(getMemDC(), img,
			_vDash[i].x - img->getFrameWidth() / 2,
			_vDash[i].y - img->getFrameHeight() / 2, _vDash[i].frameX, 0);

		if (_vDash[i].lifeTime == 180)
		{
			_vDash.erase(_vDash.begin() + i);
		}
		else
		{
			if (_vDash[i].lifeTime % 5 == 0) _vDash[i].frameX++;
			if (_vDash[i].frameX == img->getMaxFrameX()) _vDash[i].frameX = 0;

			i++;
		}
	}
}

void dashFire::singleRender(int index)
{
	_vDash[index].lifeTime++;

	//CAMERAMANAGER->Ellipse(getMemDC(), _vDash[i].rc);
	image* img = IMAGEMANAGER->findImage("flame");

	CAMERAMANAGER->FrameRender(getMemDC(), img,
		_vDash[index].x - img->getFrameWidth() / 2,
		_vDash[index].y - img->getFrameHeight() / 2, _vDash[index].frameX, 0);

	if (_vDash[index].lifeTime == 180)
	{
		_vDash.erase(_vDash.begin() + index);
	}
	else
	{
		if (_vDash[index].lifeTime % 5 == 0) _vDash[index].frameX++;
		if (_vDash[index].frameX == img->getMaxFrameX()) _vDash[index].frameX = 0;
	}


}

void dashFire::fire(float x, float y)
{
	tagArcana dash;
	dash.rc = RectMakeCenter(x, y, 128, 128);
	dash.x = x;
	dash.y = y;
	dash.frameX = 0;
	dash.lifeTime = 0;
	dash.atkPower = 7;
	_vDash.push_back(dash);
}

//추가 스킬 RAGING INFERNO

HRESULT RagingInferno::init()
{
	time = 0;

	distance = 0;
	index = count = 0;

	isFire = gauging = isActive = false;
	inferno.img = IMAGEMANAGER->addFrameImage("inferno", "resource/player/inferno.bmp", 240, 80, 3, 1);
	return S_OK;
}

void RagingInferno::release()
{
}

void RagingInferno::update(int* gaugeTime)
{
	count++;

	if (count % 3 == 0)
	{
		index++;
		if (index > 2) index = 0;
	}

	distance = getDistance(inferno.x, inferno.y, inferno.fireX, inferno.fireY);

	move(*gaugeTime);
	if (isActive && *gaugeTime < 70)
	{
		*gaugeTime = 70;
		inferno.lifeTime--;
	}
	
}

void RagingInferno::render()
{
	if (isFire)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), inferno.img,
			inferno.x - (inferno.img->getFrameWidth() / 2),
			inferno.y - (inferno.img->getFrameHeight() / 2), index, 0);

		if (PLAYERDATA->getGaugeTime() >= 70)
		{
			for (int i = 0; i < vTail.size();)
			{
				//이동
				vTail[i].x += cosf(vTail[i].angle) * vTail[i].speed;
				vTail[i].y -= sinf(vTail[i].angle) * vTail[i].speed;

				if (10.f > vTail[i].minAngle)
				{
					vTail[i].minAngle += 0.002f;
				}
				vTail[i].angle -= sinf(vTail[i].minAngle);

				//렌더
				//CAMERAMANAGER->Ellipse(getMemDC(), RectMakeCenter(vTail[i].x, vTail[i].y, 20, 20));
				PARTICLE->generate("frameParticle", vTail[i].x, vTail[i].y, vTail[i].angle, 4, 1.f);

				//삭제
				if (vTail[i].currentTime == vTail[i].lifeTime) vTail.erase(vTail.begin() + i);
				else
				{
					vTail[i].currentTime++;

					i++;
				}
			}
		}
		//CAMERAMANAGER->Ellipse(getMemDC(), inferno.rc);
	}
	else
	{
		if (vTail.empty() == false)vTail.clear();
	}

}

void RagingInferno::fire(float x, float y, float angle, int* gaugeTime)
{
	inferno.angle = angle;
	inferno.speed = -30.f;
	inferno.x = inferno.fireX = x + cosf(angle) * inferno.speed;
	inferno.y = inferno.fireY = y - sinf(angle) * inferno.speed;
	inferno.rc = RectMakeCenter(inferno.x, inferno.y, 20, 20);
	inferno.lifeTime = 100;
	inferno.atkPower = 30;
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 3, 3.f, 0.8f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 5, 5.f, 0.7f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 7, 7.f, 0.6f, 10);
	*gaugeTime = 0;
	isFire = true;
	gauging = true;
}

void RagingInferno::move(int gaugeTime)
{
	if (isFire)
	{
		if (gaugeTime > 50 && gaugeTime < 70)
		{

			inferno.x = inferno.x + cosf(inferno.angle) * 20.0f;
			inferno.y = inferno.y - sinf(inferno.angle) * 20.0f;
			inferno.rc = RectMakeCenter(inferno.x, inferno.y, 50, 50);
			gauging = false;
			if (gaugeTime % 3 == 0) PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 1, 6, 6, 20.f, 0.4f, 10);
		}
		else isActive = false;
		 if (gaugeTime == 70)
		{
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 3, 3.f, 0.8f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 5, 5.f, 0.7f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 7, 7.f, 0.6f, 10);
		}

		 if (gaugeTime >= 70)
		{
			 isActive = true;
			inferno.rc = RectMakeCenter(inferno.x, inferno.y, 150, 150);

			inferno.lifeTime--;

			if (time % 35 == 0)
			{
				tagTail tail;
				tail.currentTime = 0;
				tail.minAngle = 0.1f;
				tail.lifeTime = 100;
				tail.speed = 8.f;
				float angle = 0;

				tail.x = inferno.x - 17;
				tail.y = inferno.y - 75;
				tail.angle = angle;

				vTail.push_back(tail);

				angle = 120 * (PI / 180);
				tail.x = inferno.x - 57;
				tail.y = inferno.y + 50;
				tail.angle = angle;

				vTail.push_back(tail);

				angle = 240 * (PI / 180);
				tail.x = inferno.x + 73;
				tail.y = inferno.y + 20;
				tail.angle = angle;

				vTail.push_back(tail);
			}
			time++;
		}

		if (inferno.lifeTime == 0)
		{
			PARTICLE->explosionParticlePlay(inferno.x, inferno.y);

			inferno.x = inferno.fireX;
			inferno.y = inferno.fireY;
			isFire = false;
			isActive = false;
		}
	}
}

bool RagingInferno::CheckCollision(RECT enemy)
{
	RECT temp;
	if (IntersectRect(&temp, &inferno.rc, &enemy))
	{
		return true;
	}
	else return false;

}