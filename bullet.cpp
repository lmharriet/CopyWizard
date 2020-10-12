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
			CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage,
				_vBullet[i].x - _vBullet[i].bulletImage->getFrameWidth() / 2,
				_vBullet[i].y - _vBullet[i].bulletImage->getFrameHeight() / 2,
				_vBullet[i].FrameX, _vBullet[i].FrameY);

		//CAMERAMANAGER->Rectangle(getMemDC(), _vBullet[i].rc);
	}
}

void bullet::fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind)
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
	if (kind == MONSTERKIND::SUMMONER)
	{
		bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	}
	else
	{
		bullet.bulletImage = NULL;
	}
	bullet.kind = kind;
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.radius = 36;
	bullet.atkPower = damage;
	bullet.count = 0;
	bullet.FrameX = 0;
	bullet.FrameY = 0;


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
		switch (_vBullet[i].kind)
		{
		case MONSTERKIND::GOLEM:
			_vBullet[i].rc = RectMake(_vBullet[i].x, _vBullet[i].y, 90, 120);
			_vBullet[i].count++;
			if (_vBullet[i].count >= 10)
				_vBullet.erase(_vBullet.begin() + i);
			break;
		case MONSTERKIND::KNIGHT:
			_vBullet[i].rc = RectMake(_vBullet[i].x, _vBullet[i].y, 90, 120);
			_vBullet[i].count++;
			if (_vBullet[i].count >= 5)
				_vBullet.erase(_vBullet.begin() + i);
			break;
		case MONSTERKIND::SUMMONER:
			_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);
			float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
				_vBullet[i].x, _vBullet[i].y);
			if (_range < distance)//총알이 사거리 보다 커졌을때
			{
				_vBullet.erase(_vBullet.begin() + i);
			}
			break;
		}
	}
}

//총알삭제
void bullet::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
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

	isCoolTime = false;
	coolTime = 30;
	currentCoolTime = 0;
	return S_OK;
}

void bomb::release()
{
}

void bomb::update()
{
	count++;

	//
	//if (ranAtk > 10 && count % 3 == 0)
	//{
	//	ranAtk = criticalHit;
	//}

	if (count % 2 == 0)
	{
		index++;
		if (index > 3)index = 0;
	}
	this->move();

	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}
}

void bomb::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage,
			_vBullet[i].x - (_vBullet[i].bulletImage->getFrameWidth() / 2),
			_vBullet[i].y - (_vBullet[i].bulletImage->getFrameHeight() / 2), index, 0);

		if (INPUT->GetToggleKey('L'))CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 20, 20));
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
	bullet.collision = false;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y, radius * 2, radius * 2);
	bullet.atkPower = 12;

	_vBullet.push_back(bullet);

	isCoolTime = true;

	//sound
	if(SOUNDMANAGER->isPlaySound("RagingInfernoFire") == false) SOUNDMANAGER->play("RagingInfernoFire", false);

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
			PARTICLE->explosionGenerate("explosionParticle", _vBullet[i].x + 20, _vBullet[i].y + 20, 5, 30, 2.f, 3, true);
			SOUNDMANAGER->play("blazeExp", false);
			//_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);
			_vBullet[i].collision = true;
		}

		//충돌
		for (int j = 0; j < PLAYERDATA->getWall().size(); j++)
		{
			int num = PLAYERDATA->getWall()[j];

			if (colCheck(PLAYERDATA->_getTile()[num].rc, _vBullet[0].rc))
			{
				PARTICLE->explosionGenerate("explosionParticle", _vBullet[i].x + 20, _vBullet[i].y + 20, 5, 30, 2.f, 3, true);
				SOUNDMANAGER->play("blazeExp", false);
				_vBullet.erase(_vBullet.begin());
				break;
			}
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
HRESULT meteor::init()
{
	IMAGEMANAGER->addFrameImage("circle", "resource/player/castingCircle1.bmp", 3072, 128, 24, 1, true, RGB(255, 0, 0));

	currentCoolTime = 0;
	coolTime = 300;
	isCoolTime = false;

	count = index = 0;


	//upgrade용
	upgrade = false;
	ranCount = 5;
	save = { 0,0 };
	meteorCount = 0;

	return S_OK;
}

void meteor::release()
{
}

void meteor::update()
{
	count++;

	move();

	if (isCoolTime)
	{
		currentCoolTime++;

		if (currentCoolTime == coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}

	//데미지 넣을 용도
	for (int i = 0; i < vDamage.size();)
	{
		vDamage[i].lifeTime--;

		if (vDamage[i].lifeTime == 0)
		{
			vDamage.erase(vDamage.begin());
		}
		else
		{
			i++;
		}
	}

	//upgrade meteor
	if (meteorCount > 0)
	{
		//count = 7 % 5 == 2
		if (count % ranCount == 0)
		{
			ranCount = RANDOM->range(10, 20);

			float ranX = RANDOM->range(-150.f, 150.f);
			float ranY = RANDOM->range(-150.f, 150.f);
			tagMeteor meteor;
			meteor.endY = save.y + ranY;
			meteor.img = IMAGEMANAGER->findImage("meteor");

			float cul = 110 * (PI / 180);
			meteor.x = (save.x + ranX) + cosf(cul) * 600.f;
			meteor.y = (save.y + ranY) - sinf(cul) * 600.f;
			meteor.rc = RectMakeCenter(meteor.x, meteor.y, 100, 100);
			meteor.angle = 290 * (PI / 180);
			meteor.speed = 15.f;

			vMeteor.push_back(meteor);
			makeCircle(save.x + ranX, save.y + ranY);

			isCoolTime = true;
			UI->addCoolTime("meteorIcon");
			meteorCount--;
		}

	}


}

void meteor::render()
{
	image* img = IMAGEMANAGER->findImage("circle");

	char temp[126];

	for (int i = 0; i < vCircle.size(); i++)
	{
		vCircle[i].count++;
		if (vCircle[i].count % 3 == 0)
		{
			if (vCircle[i].index < 23)
				vCircle[i].index++;
		}

		//CAMERAMANAGER->Ellipse(getMemDC(), vCircle[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), img,
			vCircle[i].x - img->getFrameWidth() / 2, vCircle[i].y - img->getFrameHeight() / 2, vCircle[i].index, 0);
	}
	for (int i = 0; i < vMeteor.size(); i++)
	{
		if (count % 5 == 0)
		{
			index++;
			if (index > 5) index = 0;
		}

		CAMERAMANAGER->FrameRender(getMemDC(), vMeteor[i].img,
			vMeteor[i].x - vMeteor[i].img->getFrameWidth() / 2 - 10,
			vMeteor[i].y - vMeteor[i].img->getFrameHeight() / 2 - 50,
			index, 0);
	}
	if (INPUT->GetToggleKey('L'))
	{
		for (int i = 0; i < vDamage.size(); i++)
		{
			CAMERAMANAGER->Ellipse(getMemDC(), vDamage[i].rc);
		}
	}
}

void meteor::makeCircle(float x, float y)
{
	tagCircle circle;

	circle.angle = 0;
	circle.x = x;
	circle.y = y;
	circle.count = 0;
	circle.index = 0;
	circle.rc = RectMakeCenter(circle.x, circle.y, 100, 100);
	circle.lifeTime = 0;

	vCircle.push_back(circle);
}

void meteor::creatMeteor(float x, float y, float angle)
{
	if (upgrade == true)
	{
		tagMeteor meteor;
		meteor.endY = y;
		meteor.img = IMAGEMANAGER->findImage("meteor");

		float cul = 110 * (PI / 180);
		meteor.x = x + cosf(cul) * 600.f;
		meteor.y = y - sinf(cul) * 600.f;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, 100, 100);
		meteor.angle = 290 * (PI / 180);
		meteor.speed = 15.f;

		vMeteor.push_back(meteor);
		makeCircle(x, y);
		isCoolTime = true;
		UI->addCoolTime("meteorIcon");
	}

	else
	{
		save = { (long)x,(long)y };
		meteorCount = 5;
		ranCount = 5;
	}

}



void meteor::move()
{
	for (int i = 0; i < vMeteor.size();)
	{
		if (count % 2 == 0)PARTICLE->smokeParticlePlay(vMeteor[i].x, vMeteor[i].y);

		//이동
		vMeteor[i].x += cosf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].y -= sinf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].rc = RectMakeCenter(vMeteor[i].x, vMeteor[i].y, 100, 100);

		//삭제
		if (vMeteor[i].y > vMeteor[i].endY)
		{
			PARTICLE->explosionParticle2Play(vMeteor[i].x, vMeteor[i].y);

			//데미지 넣을 용도
			tagMeteor damage;

			damage.x = vCircle[i].x;
			damage.y = vCircle[i].y;
			damage.angle = vMeteor[i].angle; //메테오 방향으로 넉백시킬 때 사용
			damage.lifeTime = 30;
			damage.rc = RectMakeCenter(damage.x, damage.y, 100, 100);
			damage.atkPower = 20;

			vDamage.push_back(damage);

			CAMERAMANAGER->Shake(20, 20, 4);

			index = 0;
			vMeteor.erase(vMeteor.begin() + i);
			vCircle.erase(vCircle.begin() + i);
		}
		else i++;
	}
}


//DASH
HRESULT dashFire::init()
{
	isCoolTime = false;
	coolTime = 240;
	currentCoolTime = 0;

	return S_OK;
}

void dashFire::release()
{
}

void dashFire::update()
{
	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}
}

//사용X
void dashFire::render()
{
	//for (int i = 0; i < _vDash.size();)
	//{
	//	_vDash[i].lifeTime++;

	//	//CAMERAMANAGER->Ellipse(getMemDC(), _vDash[i].rc);
	//	image* img = IMAGEMANAGER->findImage("flame");

	//	CAMERAMANAGER->FrameRender(getMemDC(), img,
	//		_vDash[i].x - img->getFrameWidth() / 2,
	//		_vDash[i].y - img->getFrameHeight() / 2, _vDash[i].frameX, 0);

	//	if (_vDash[i].lifeTime == 180)
	//	{
	//		_vDash.erase(_vDash.begin() + i);
	//	}
	//	else
	//	{
	//		if (_vDash[i].lifeTime % 5 == 0) _vDash[i].frameX++;
	//		if (_vDash[i].frameX == img->getMaxFrameX()) _vDash[i].frameX = 0;

	//		i++;
	//	}
	//}
}

void dashFire::singleRender(int index)
{
	vDash[index].lifeTime++;

	image* img = IMAGEMANAGER->findImage("flame");

	CAMERAMANAGER->FrameRender(getMemDC(), img,
		vDash[index].x - img->getFrameWidth() / 2,
		vDash[index].y - img->getFrameHeight() / 2 - 20, vDash[index].frameX, 0);

	if (INPUT->GetToggleKey('L'))	CAMERAMANAGER->Ellipse(getMemDC(), vDash[index].rc);
	if (vDash[index].lifeTime == 180)
	{
		vDash.erase(vDash.begin() + index);
	}
	else
	{
		if (vDash[index].lifeTime % 5 == 0) vDash[index].frameX++;
		if (vDash[index].frameX == img->getMaxFrameX()) vDash[index].frameX = 0;
	}
}

void dashFire::fire(float x, float y)
{
	tagArcana dash;
	dash.rc = RectMakeCenter(x, y, 50, 50);
	dash.x = x;
	dash.y = y;
	dash.frameX = 0;
	dash.lifeTime = 0;
	dash.atkPower = 7;
	vDash.push_back(dash);
}

//추가 스킬 RAGING INFERNO

HRESULT RagingInferno::init()
{

	inferno.img = IMAGEMANAGER->addFrameImage("inferno", "resource/player/inferno.bmp", 240, 80, 3, 1);

	time = 0;
	distance = 0;
	currentCoolTime = 0;
	coolTime = 300;

	isFire = gauging = isActive = false;
	isCoolTime = false;

	index = count = 0;
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
	if (isActive && *gaugeTime < 50)
	{
		*gaugeTime = 50;
		inferno.lifeTime--;
	}

	if (isCoolTime)
	{
		currentCoolTime++;

		if (currentCoolTime == coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}

}

void RagingInferno::render()
{
	if (isFire)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), inferno.img,
			inferno.x - (inferno.img->getFrameWidth() / 2),
			inferno.y - (inferno.img->getFrameHeight() / 2), index, 0);

		if (PLAYERDATA->getGaugeTime() >= 50)
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
				PARTICLE->generate("frameParticle", vTail[i].x, vTail[i].y, vTail[i].angle, 4, 1.f, true);

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
	inferno.atkPower = 15;
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 3, 3.f, 0.8f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 5, 5.f, 0.7f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 7, 7.f, 0.6f, 10);
	if (SOUNDMANAGER->isPlaySound("RagingInfernoFire") == false) SOUNDMANAGER->play("RagingInfernoFire", false);
	*gaugeTime = 0;
	isFire = true;
	gauging = true;

	isCoolTime = true;
	UI->addCoolTime("infernoIcon");
}

void RagingInferno::move(int gaugeTime)
{
	if (isFire)
	{
		if (gaugeTime > 30 && gaugeTime < 50)
		{

			inferno.x = inferno.x + cosf(inferno.angle) * 20.0f;
			inferno.y = inferno.y - sinf(inferno.angle) * 20.0f;
			inferno.rc = RectMakeCenter(inferno.x, inferno.y, 50, 50);
			gauging = false;
			if (gaugeTime % 3 == 0) PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 1, 6, 6, 20.f, 0.4f, 10);
		}
		else isActive = false;
		if (gaugeTime == 50)
		{
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 3, 3.f, 0.8f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 5, 5.f, 0.7f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 7, 7.f, 0.6f, 10);
			SOUNDMANAGER->play("RagingInfernoExp", false);
		}

		if (gaugeTime >= 50)
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


//=============================================================
//	## dragonArc ## 
//=============================================================

HRESULT dragonArc::init()
{
	IMAGEMANAGER->addFrameImage("dragon", "resource/player/dragonArc.bmp", 1800 * 3, 100 * 3, 36, 2);
	count = index = 0;
	return S_OK;
}

void dragonArc::release()
{

}

void dragonArc::update()
{
	count++;

	move();

}

void dragonArc::render()
{

	for (int i = 0; i < vDragon.size(); i++)
	{
		CAMERAMANAGER->Ellipse(getMemDC(), vDragon[i].rc);

		if (count % 2 == 0)
		{
			PARTICLE->explosionGenerate("explosionParticle2", vDragon[i].x, vDragon[i].y, 3, 0.75f, 0.f, 3, true);
			PARTICLE->explosionGenerate("explosionParticle2", vDragon[i].x, vDragon[i].y, 5, 1.25f, 0.f, 1, true);
		}

		CAMERAMANAGER->FrameRender(getMemDC(), vDragon[i].img,
			vDragon[i].x - vDragon[i].img->getFrameWidth() / 2,
			vDragon[i].y - vDragon[i].img->getFrameHeight() / 2, vDragon[i].index, vDragon[i].frameY);
	}
}

void dragonArc::fire(float x, float y, float angle)
{
	if (vDragon.size() < 4)
	{
		tagDragon dragon;

		dragon.img = IMAGEMANAGER->findImage("dragon");
		dragon.x = x;
		dragon.y = y;
		dragon.currentTime = 0;
		dragon.lifeTime = 88;
		dragon.speed = 15.f;
		dragon.rc = RectMakeCenter(dragon.x, dragon.y, 20, 20);

		dragon.angle = .7f + angle;
		dragon.saveAngle = -.7f + angle;
		dragon.frameY = 0;
		dragon.index = angle * 18 / PI;//0-35

		vDragon.push_back(dragon);
	
		dragon.index = 0;
		dragon.angle = -.7f + angle;
		dragon.saveAngle = .7f + angle;

		dragon.frameY = 1;
		dragon.index = 36 - (angle * 18 / PI);//0-35

		vDragon.push_back(dragon);
	}
}

void dragonArc::move()
{

	for (int i = 0; i < vDragon.size();)
	{

		if (vDragon[i].currentTime < 35 || vDragon[i].currentTime > 69)
		{
			vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
			vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
			vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 20, 20);
			if (i % 2 == 0)
			{
				vDragon[i].angle -= sinf(0.05f);
			}
			else
			{
				vDragon[i].angle += sinf(0.05f);
			}

			if (vDragon[i].currentTime == 34)
			{
				vDragon[i].angle = vDragon[i].saveAngle;
				vDragon[i].speed -= 2.6f;

				if (i % 2 == 0)
				{
					vDragon[i].frameY = 1;
					vDragon[i].index = abs(36 - vDragon[i].index);
				}

				else
				{
					vDragon[i].frameY = 0;
					vDragon[i].index = abs(36 - vDragon[i].index);
				}
			}
		}
		else
		{
			vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
			vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
			vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 20, 20);
			if (i % 2 == 0)
			{
				vDragon[i].angle += sinf(0.05f);
			}

			else
			{
				vDragon[i].angle -= sinf(0.05f);
			}

			if (vDragon[i].currentTime == 69)
			{
				vDragon[i].angle = vDragon[i].saveAngle;

				if (i % 2 == 0)
					vDragon[i].angle = vDragon[i].saveAngle + PI / 2;

				else vDragon[i].angle = vDragon[i].saveAngle - PI / 2;

				vDragon[i].speed += 1.6f;
			}
		}

		if (vDragon[i].currentTime == vDragon[i].lifeTime) vDragon.erase(vDragon.begin() + i);
		else
		{
			vDragon[i].currentTime++;
			i++;
		}
	}
}