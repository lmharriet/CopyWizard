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
		CAMERAMANAGER->Rectangle(getMemDC(), _vBullet[i].rc);
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
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.radius = 25;
	
	bullet.rc = RectMakeCenter(bullet.x, bullet.y, 50, 50);

	//���Ϳ� ���
	_vBullet.push_back(bullet);
}

void bullet::move()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;

		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 50, 50);

		//�Ѿ��� ��Ÿ� ���� Ŀ������
		//float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
		//	_vBullet[i].x, _vBullet[i].y);
		//if (_range < distance)
		//{
		//	_vBullet.erase(_vBullet.begin() + i);
		//}
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
//�Ѿ˹߻�
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

//�Ѿ˹���
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
		CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage, _vBullet[i].rc.left, _vBullet[i].rc.top, index, 0);
		//Rectangle(getMemDC(), _vBullet[i].rc);
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
//��ź����
void bomb::removeBomb(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}
