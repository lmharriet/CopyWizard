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
		CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage, _vBullet[i].x, _vBullet[i].y,
			_vBullet[i].FrameX, _vBullet[i].FrameY);
	}

}

void bullet::fire(float x, float y, float angle, float speed)
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
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.radius = 36;
	bullet.count = 0;
	bullet.FrameX = 0;
	bullet.FrameY = 0;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y, 50, 50);

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

		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 50, 50);

		//�Ѿ��� ��Ÿ� ���� Ŀ������
		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
			_vBullet[i].x, _vBullet[i].y);
		if (_range < distance)
		{
			_vBullet.erase(_vBullet.begin() + i);
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

HRESULT homingFlares::init(float range)
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
//�Ѿ˹߻�
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

//�Ѿ˹���
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

//���� ��(player flame strike)
//=============================================================
//	## bomb ## (��źó�� �ѹ߾� �߻��ϰ� �����ϰ� �ڵ�����)
//=============================================================
HRESULT bomb::init(int bulletMax, float range)
{
	//�Ѿ� ��Ÿ� �� �Ѿ� ���� �ʱ�ȭ
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
			PARTICLE->explosionGenerate("explosionParticle",_vBullet[i].x + 20, _vBullet[i].y + 20, 5, 30, 2.f, 3);
			//_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 30, 30);
			_vBullet[i].collision = true;
			cout << i << " : " << _vBullet[i].collision << '\n';
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
HRESULT meteor::init(float range)
{
	_range = range;
	angleRange = meteorCount = isUlt = 0;
	ranCount = 10;

	count = index = timer = 0;

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
	move();

	meteorUlt();
}

void meteor::render()
{
	char temp[126];

	for (int i = 0; i < vMeteor.size(); i++)
	{
		//CAMERAMANAGER->Ellipse(getMemDC(), _vMeteor[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), vMeteor[i].img, vMeteor[i].rc.left, vMeteor[i].rc.top, index, vMeteor[i].frameY);
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


	ranCount = 10; // ���׿��� �����̸� �������� �ֱ����� (ó������ 10���� ��)
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
			//����Ʈ �÷���
			//�浹 ó�� , ī�޶� shake
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

//�߰� ��ų RAGING INFERNO

HRESULT RagingInferno::init()
{
	time = 0;

	distance = 0;
	index = count = 0;

	isFire = gauging=false;
	inferno.img = IMAGEMANAGER->addFrameImage("inferno", "resource/player/inferno.bmp", 240, 80, 3, 1);
	return S_OK;
}

void RagingInferno::release()
{
}

void RagingInferno::update(int *gaugeTime)
{
	count++;

	if (count % 3 == 0)
	{
		index++;
		if (index > 2) index = 0;
	}

	distance = getDistance(inferno.x, inferno.y, inferno.fireX, inferno.fireY);

	move(*gaugeTime);

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
				PARTICLE->generate("frameParticle", vTail[i].x, vTail[i].y, vTail[i].angle, 4, 1.f);

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

void RagingInferno::fire(float x, float y, float angle,int *gaugeTime)
{
	inferno.angle = angle;
	inferno.speed = -30.f;
	inferno.x = inferno.fireX = x + cosf(angle) * inferno.speed;
	inferno.y = inferno.fireY = y - sinf(angle) * inferno.speed;
	inferno.rc = RectMakeCenter(inferno.x, inferno.y, 20, 20);
	inferno.lifeTime = 100;
	inferno.atkPower = 30;
	PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 60, 3, 3.f, 0.8f, 10);
	PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 60, 5, 5.f, 0.7f, 10);
	PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 60, 7, 7.f, 0.6f, 10);
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
			if (gaugeTime % 3 == 0) PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 1, 6, 6, 20.f, 0.4f, 10);
		}

		if (gaugeTime == 70)
		{
			PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 100, 3, 3.f, 0.8f, 10);
			PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 100, 5, 5.f, 0.7f, 10);
			PARTICLE->pointGenerate("frameParticle",inferno.x, inferno.y, 2, 100, 7, 7.f, 0.6f, 10);
		}

		if (gaugeTime >= 70)
		{
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
		}
	}
}
void RagingInferno::createTail()
{
}