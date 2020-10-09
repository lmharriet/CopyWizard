#include "stdafx.h"
#include "bullet.h"

//=============================================================
//	## bullet ## (�����Ѿ�)
//=============================================================

HRESULT bullet::init(const char* imageName, int bulletMax, float range, bool isFrameImg)
{
	//�Ѿ� �̹��� �ʱ�ȭ
	_imageName = imageName;
	//�Ѿ˰��� �� ��Ÿ� �ʱ�ȭ
	_bulletMax = bulletMax;
	_range = range;
	//�Ѿ� �̹����� ������ �̹�����?
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

void bullet::fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind, bool image)
{
	//�Ѿ� ���Ϳ� ��°��� �����Ѵ�
	//if (_bulletMax < _vBullet.size() + 1) return;

	//�Ѿ� ����ü ����
	tagBullet bullet;
	//�Ѿ� ����ü �ʱ�ȭ
	//���θ޸�, ���
	//����ü �������� ���� �ѹ��� 0���� �ʱ�ȭ �����ش�
	//ZeroMemory(&bullet, sizeof(tagBullet));
	//bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	if (image)
		bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	else bullet.bulletImage = NULL;
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


	//���Ϳ� ���
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
			_vBullet[i].rc = RectMake(_vBullet[i].x, _vBullet[i].y, 60, 60);
			float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
				_vBullet[i].x, _vBullet[i].y);
			if (_range < distance)//�Ѿ��� ��Ÿ� ���� Ŀ������
			{
				_vBullet.erase(_vBullet.begin() + i);
			}
			break;
		}
	}
}

//�Ѿ˻���
void bullet::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

//=============================================================
//	## homingFlares ## (homingFlares[0] -> �迭ó�� �̸� �����صΰ� �Ѿ˹߻�)
//=============================================================

HRESULT cleaver::init(float range)
{
	_range = range;

	////�Ѿ��� ������ŭ ����ü�� �ʱ�ȭ �� �� ���Ϳ� ���
	//for (int i = 0; i < bulletMax; i++)
	//{
	//	//�Ѿ� ����ü ����
	//	tagBullet bullet;
	//	//�Ѿ� ����ü �ʱ�ȭ
	//	//���θ޸�, ���
	//	//����ü �������� ���� �ѹ��� 0���� �ʱ�ȭ �����ش�
	//	ZeroMemory(&bullet, sizeof(tagBullet));
	//	//bullet.bulletImage = IMAGEMANAGER->findImage();
	//	bullet.bulletImage = new image;
	//	bullet.bulletImage->init("Images/missile.bmp", 26, 124, true, RGB(255, 0, 255));
	//	bullet.speed = 5.0f;
	//	bullet.fire = false;

	//	//���Ϳ� ���
	//	_vBullet.push_back(bullet);
	//}

	return S_OK;
}

void cleaver::release()
{

}

void cleaver::update()
{
	this->move();
}

void cleaver::render()
{
	/*for (int i = 0; i < _vFlares.size(); i++)
	{
		CAMERAMANAGER->Ellipse(getMemDC(), _vFlares[i].rc);
		CAMERAMANAGER->Ellipse(getMemDC(), _vRange[i] .rc);
	}*/


}
//�Ѿ˹߻�
void cleaver::fire(float x, float y, float angle)
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

//�Ѿ˹���
void cleaver::move()
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

//���� ��(player flame strike)
//=============================================================
//	## bomb ## (��źó�� �ѹ߾� �߻��ϰ� �����ϰ� �ڵ�����)
//=============================================================
HRESULT bomb::init(int bulletMax, float range)
{
	//�Ѿ� ��Ÿ� �� �Ѿ� ���� �ʱ�ȭ
	_range = range;
	_bulletMax = bulletMax;

	ranAtk = 5;
	criticalHit = 20;
	count = index = 0;
	return S_OK;
}

void bomb::release()
{
}

void bomb::update()
{
	count++;

	ranAtk = RANDOM->range(5, 12);
	if (ranAtk > 10 && count % 3 == 0)
	{
		ranAtk = criticalHit;
	}

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
	//�Ѿ� ���Ϳ� ��°��� �����Ѵ�
	if (_bulletMax < _vBullet.size() + 1) return;

	//�Ѿ� ����ü ����
	tagBullet bullet;
	//�Ѿ� ����ü �ʱ�ȭ
	//���θ޸�, ���
	//����ü �������� ���� �ѹ��� 0���� �ʱ�ȭ �����ش�


	bullet.bulletImage = IMAGEMANAGER->addFrameImage("blaze", "resource/player/blaze.bmp", 288, 96, 3, 1, true, RGB(255, 0, 255));
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.radius = radius;
	bullet.atkPower = ranAtk;
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
			PARTICLE->explosionGenerate("explosionParticle", _vBullet[i].x + 20, _vBullet[i].y + 20, 5, 30, 2.f, 3, true);
			//_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);
			_vBullet[i].collision = true;
		}
	}
}
//��ź����
void bomb::removeBomb(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

//���� ��(player meteor)
//=============================================================
//	## meteor ## (signature)
//=============================================================
HRESULT meteor::init()
{
	IMAGEMANAGER->addFrameImage("circle", "resource/player/castingCircle1.bmp", 3072, 128, 24, 1, true, RGB(255, 0, 0));

	currentCoolTime = 0;
	coolTime = 300;
	isCoolTime = false;
	isAttack = false;

	count = index = collisionCount = 0;
	circleCount = CircleIndex = 0;

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
	if (isAttack)
	{
		if (collisionCount < 60)
			collisionCount++;
		if (collisionCount == 60)
			isAttack = false;
	}

}

void meteor::render()
{

	image* img = IMAGEMANAGER->findImage("circle");

	char temp[126];

	for (int i = 0; i < vCircle.size(); i++)
	{
		circleCount++;
		if (circleCount % 3 == 0)
		{
			if (CircleIndex < 23)
				CircleIndex++;
		}

		//CAMERAMANAGER->Ellipse(getMemDC(), vCircle[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), img,
			vCircle[i].x - img->getFrameWidth() / 2, vCircle[i].y - img->getFrameHeight() / 2, CircleIndex, 0);
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
		//CAMERAMANAGER->Ellipse(getMemDC(), vMeteor[i].rc);
	}
	if (isAttack)
		CAMERAMANAGER->Ellipse(getMemDC(), rc);
}

void meteor::makeCircle(float x, float y)
{
	//�÷��̾� �������� ���� ������ �� �Ʒ� ������ ����

	tagCircle circle;

	circle.angle = 0;
	circle.x = x;
	circle.y = y;
	circle.rc = RectMakeCenter(circle.x, circle.y, 100, 100);
	circle.lifeTime = 0;

	vCircle.push_back(circle);
}

void meteor::creatMeteor(float x, float y, float angle)
{

	tagMeteor meteor;
	meteor.endY = y;
	meteor.atkPower = 22;
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

void meteor::move()
{
	for (int i = 0; i < vMeteor.size();)
	{
		if (count % 2 == 0)PARTICLE->smokeParticlePlay(vMeteor[i].x, vMeteor[i].y);

		//�̵�
		vMeteor[i].x += cosf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].y -= sinf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].rc = RectMakeCenter(vMeteor[i].x, vMeteor[i].y, 100, 100);

		//����
		if (vMeteor[i].y > vMeteor[i].endY)
		{
			PARTICLE->explosionParticle2Play(vMeteor[i].x, vMeteor[i].y);

			//�浹 ���� ���� damage �ֱ� ���� �뵵
			isAttack = true;
			if (isAttack) rc = RectMakeCenter(vCircle[i].x, vCircle[i].y, 100, 100);


			CAMERAMANAGER->Shake(20, 20, 4);

			vMeteor.erase(vMeteor.begin() + i);
			index = 0;

			vCircle.erase(vCircle.begin() + i);
			CircleIndex = 0;

		}
		else i++;
	}

}


//DASH
HRESULT dashFire::init()
{

	//gameScene ���� �ٲٱ� , damage�� isCoolTime false�϶� ���� �ֱ�
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

//���X
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

	_vDash[index].lifeTime++;

	image* img = IMAGEMANAGER->findImage("flame");

	CAMERAMANAGER->FrameRender(getMemDC(), img,
		_vDash[index].x - img->getFrameWidth() / 2,
		_vDash[index].y - img->getFrameHeight() / 2-20, _vDash[index].frameX, 0);

	CAMERAMANAGER->Ellipse(getMemDC(), _vDash[index].rc);
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
	dash.rc = RectMakeCenter(x, y, 50, 50);
	dash.x = x;
	dash.y = y;
	dash.frameX = 0;
	dash.lifeTime = 0;
	dash.atkPower = 7;
	_vDash.push_back(dash);
}

//�߰� ��ų RAGING INFERNO

HRESULT RagingInferno::init()
{
	inferno.img = IMAGEMANAGER->addFrameImage("inferno", "resource/player/inferno.bmp", 240, 80, 3, 1);

	time = 0;
	distance = 0;
	currentCoolTime = 0;
	coolTime = 300;
	ranAtkPower = 0;

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
	ranAtkPower = RANDOM->range(7, 12);

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
				//�̵�
				vTail[i].x += cosf(vTail[i].angle) * vTail[i].speed;
				vTail[i].y -= sinf(vTail[i].angle) * vTail[i].speed;

				if (10.f > vTail[i].minAngle)
				{
					vTail[i].minAngle += 0.002f;
				}
				vTail[i].angle -= sinf(vTail[i].minAngle);

				//����
				//CAMERAMANAGER->Ellipse(getMemDC(), RectMakeCenter(vTail[i].x, vTail[i].y, 20, 20));
				PARTICLE->generate("frameParticle", vTail[i].x, vTail[i].y, vTail[i].angle, 4, 1.f, true);

				//����
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
	inferno.atkPower = 7;
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 3, 3.f, 0.8f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 5, 5.f, 0.7f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 7, 7.f, 0.6f, 10);
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
			inferno.atkPower = ranAtkPower;
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