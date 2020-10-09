#include "stdafx.h"
#include "particleManager.h"

HRESULT particleManager::init()
{
	IMAGEMANAGER->addFrameImage("frameParticle", "Images/particle/frameParticle.bmp", 240, 160, 6, 4);
	IMAGEMANAGER->addFrameImage("explosionParticle", "Images/particle/explosionParticle.bmp", 960 / 2, 640 / 2, 6, 4);
	IMAGEMANAGER->addFrameImage("healBallParticle", "Images/particle/healBallParticle.bmp", 60, 5, 10, 1);
	IMAGEMANAGER->addFrameImage("bossJumpParticle", "Images/particle/stone.bmp", 60, 15, 4, 1);
	
	IMAGEMANAGER->addFrameImage("stoneX2", "Images/particle/stoneX2.bmp", 34, 122, 1, 4);
	return S_OK;
}

void particleManager::render(HDC hdc)
{
	image* img = nullptr;

	for (int i = 0; i < vParticle.size();)
	{
		img = IMAGEMANAGER->findImage(vParticle[i].keyName);

		vParticle[i].time++;

		//�̵�1
		if (vParticle[i].isBack == false)
		{
			vParticle[i].x += cosf(vParticle[i].angle) * vParticle[i].speed;
			vParticle[i].y -= sinf(vParticle[i].angle) * vParticle[i].speed;
		}

		//�̵�2
		if (vParticle[i].isBack == true)
		{
			if (vParticle[i].backTime > 0)
			{
				vParticle[i].backTime--;

				vParticle[i].x += cosf(vParticle[i].angle) * vParticle[i].speed;
				vParticle[i].y -= sinf(vParticle[i].angle) * vParticle[i].speed;
			}
			else
			{
				float angle = getAngle(vParticle[i].x, vParticle[i].y, PLAYERDATA->getX(), PLAYERDATA->getY());

				vParticle[i].x += cosf(angle) * vParticle[i].speed * 1.3f;
				vParticle[i].y -= sinf(angle) * vParticle[i].speed * 1.3f;
			}
		}

		CAMERAMANAGER->FrameRender(hdc, img,
			vParticle[i].x - img->getFrameWidth() / 2,
			vParticle[i].y - img->getFrameHeight() / 2,
			vParticle[i].frameX,
			vParticle[i].frameY);

		//�̹���
		if (vParticle[i].time % vParticle[i].delay == 0)
		{
			vParticle[i].frameX++;
		}

		//����

		//(���� ? �ð��� ������� �����ų�, �߾� ��ǥ��Ʈ�� �浹�� ��)
		if (vParticle[i].isCollect) // collect effect
		{
			RECT rc1 = RectMakeCenter(vParticle[i].x, vParticle[i].y, 10, 10);
			RECT rc2 = RectMakeCenter(vParticle[i].endX, vParticle[i].endY, 30, 30);

			//CAMERAMANAGER->Rectangle(hdc, rc1);
			//CAMERAMANAGER->Rectangle(hdc, rc2);

			if(colCheck(rc1,rc2) || vParticle[i].time > 50) vParticle.erase(vParticle.begin() + i);
			else i++;
		}

		else // default
		{
			if (vParticle[i].isFrameDel) //(���� ? maxFrame �Ǹ� ����)
			{

				if (vParticle[i].frameX == vParticle[i].maxFrameX) vParticle.erase(vParticle.begin() + i);
				else i++;

			}

			else //(���� ? �ð��� �Ǹ� ����)
			{
				
				if (vParticle[i].time == vParticle[i].lifeTime) vParticle.erase(vParticle.begin() + i);
				else i++;

			}
		}
	}
}

void particleManager::pointGenerate(string keyName, float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay)
{
	tagParticlePoint particlePoint;
	particlePoint.x = x;
	particlePoint.y = y;
	particlePoint.currentTime = 0;
	particlePoint.delay = CreateDelay;
	particlePoint.lifeTime = lifeTime;
	particlePoint.angleNum = 0;
	particlePoint.maxAngle = maxAngle;
	particlePoint.radius = radius;

	particlePoint.particleSpeed = particleSpeed;
	particlePoint.frameDelay = frameDelay;

	particlePoint.keyName = keyName;

	vParticlePoint.push_back(particlePoint);
}

void particleManager::explosionGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, bool isFrameDel, int particleEndTime, bool isBack)
{
	tagParticlePoint particlePoint;
	particlePoint.x = x;
	particlePoint.y = y;
	particlePoint.maxAngle = maxAngle;
	particlePoint.radius = radius;

	particlePoint.particleSpeed = particleSpeed;
	particlePoint.frameDelay = frameDelay;

	particlePoint.keyName = keyName;
	particlePoint.isBack = isBack;
	particlePoint.isCollect = false;

	particlePoint.currentTime = 0;
	particlePoint.lifeTime = 0;
	particlePoint.createDelay = 1;

	particlePoint.isFrameDel = isFrameDel;
	particlePoint.particleEndTime = particleEndTime;

	vExplosion.push_back(particlePoint);
}

void particleManager::collectingGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, int lifeTime , int createDelay)
{
	tagParticlePoint particlePoint;

	particlePoint.x = x;
	particlePoint.y = y;
	particlePoint.maxAngle = maxAngle;
	particlePoint.radius = radius;

	particlePoint.particleSpeed = particleSpeed;
	particlePoint.frameDelay = frameDelay;

	particlePoint.keyName = keyName;
	particlePoint.isBack = false;
	particlePoint.isCollect = true;

	particlePoint.currentTime = 0;
	particlePoint.lifeTime = lifeTime;
	particlePoint.createDelay = createDelay;

	particlePoint.isFrameDel = false;
	particlePoint.particleEndTime = 50;

	vExplosion.push_back(particlePoint);
}

void particleManager::pointActive()
{
	float* arr;
	for (int i = 0; i < vParticlePoint.size();)
	{
		arr = new float[vParticlePoint[i].maxAngle];

		for (int k = 0; k < vParticlePoint[i].maxAngle; k++)
		{
			arr[k] = (((2.f * PI) / vParticlePoint[i].maxAngle) * k);
		}

		vParticlePoint[i].currentTime++;

		//����
		if (vParticlePoint[i].currentTime % vParticlePoint[i].delay == 0)
		{
			//�ش� �������� ����.. �о��� (�߾ӿ��� ���� ��������)
			float x = vParticlePoint[i].x + cosf(arr[vParticlePoint[i].angleNum]) * vParticlePoint[i].radius; // 10���� �о���
			float y = vParticlePoint[i].y - sinf(arr[vParticlePoint[i].angleNum]) * vParticlePoint[i].radius; // 10���� �о���

			generate(vParticlePoint[i].keyName, x, y, arr[vParticlePoint[i].angleNum], vParticlePoint[i].frameDelay, vParticlePoint[i].particleSpeed, true);
			vParticlePoint[i].angleNum++;

			if (vParticlePoint[i].angleNum == vParticlePoint[i].maxAngle)vParticlePoint[i].angleNum = 0;
		}

		//����
		if (vParticlePoint[i].currentTime == vParticlePoint[i].lifeTime)
		{
			vParticlePoint.erase(vParticlePoint.begin() + i);
		}
		else i++;
	}
}

void particleManager::explosionActive()
{
	for (int i = 0; i < vExplosion.size(); i++)
	{
		if (vExplosion[i].lifeTime > 0)
		{
			vExplosion[i].currentTime++;
			if (vExplosion[i].currentTime % vExplosion[i].createDelay != 0)continue;
		}

		//����
		for (int k = 0; k < vExplosion[i].maxAngle; k++)
		{
			float angle = (((2.f * PI) / vExplosion[i].maxAngle) * k) + RANDOM->range(-0.4f, 0.4f);
			float speed = vExplosion[i].particleSpeed + RANDOM->range(-1.f, 1.f);
			float delay = vExplosion[i].frameDelay + RANDOM->range(0, 4);

			float x = vExplosion[i].x + cosf(angle) * vExplosion[i].radius;
			float y = vExplosion[i].y - sinf(angle) * vExplosion[i].radius;

			float culAngle = 0.f;
			if(vExplosion[i].isCollect) culAngle = getAngle(x, y, vExplosion[i].x, vExplosion[i].y);

			if (vExplosion[i].isCollect) generate2(vExplosion[i].keyName, x, y, culAngle, delay, speed, vExplosion[i].x, vExplosion[i].y);
			else generate(vExplosion[i].keyName, x, y, angle, delay, speed, vExplosion[i].isFrameDel, vExplosion[i].particleEndTime, vExplosion[i].isBack, 35);
		}

		//����
		if (vExplosion[i].lifeTime > 0) // �ð� ��?
		{

			if (vExplosion[i].currentTime == vExplosion[i].lifeTime) vExplosion.erase(vExplosion.begin() + i);

			else
			{
				i--;
			}

		}

		else // �ٷ� ����
		{
			vExplosion.erase(vExplosion.begin() + i);
			i--;
		}
	}
}

void particleManager::generate(string keyName, float x, float y, float angle, int delay, float speed, bool isFrameDel, int particleEndTime, bool isBack, int backTime)
{
	tagParticle particle;

	//default info
	particle.x = x;
	particle.y = y;
	particle.angle = angle;
	particle.delay = delay;
	particle.speed = speed;
	particle.time = 0;

	particle.isFrameDel = isFrameDel;
	particle.lifeTime = particleEndTime;

	image* img = IMAGEMANAGER->findImage(keyName);
	particle.keyName = keyName;
	//image info
	particle.frameX = 0;
	particle.maxFrameX = img->getMaxFrameX();
	particle.frameY = RANDOM->range(0, img->getMaxFrameY());

	particle.isBack = isBack;
	particle.backTime = backTime;
	particle.isCollect = false;
	vParticle.push_back(particle);
}

void particleManager::generate2(string keyName, float x, float y, float angle, int delay, float speed, float endX, float endY, bool isCollect)
{
	tagParticle particle;

	//default info
	particle.x = x;
	particle.y = y;
	particle.angle = angle;
	particle.delay = delay;
	particle.speed = speed;
	particle.time = 0;

	image* img = IMAGEMANAGER->findImage(keyName);
	particle.keyName = keyName;
	//image info
	particle.frameX = 0;
	particle.maxFrameX = img->getMaxFrameX();
	particle.frameY = RANDOM->range(0, img->getMaxFrameY());

	particle.isBack = false;
	particle.backTime = 0;

	particle.isCollect = isCollect;
	particle.endX = endX;
	particle.endY = endY;
	vParticle.push_back(particle);
}

void particleManager::potionParticlePlay(float x, float y)
{
	PARTICLE->explosionGenerate("healBallParticle", x, y, 15, 1.f, 4.f, 15, false, 0, true);
	PARTICLE->explosionGenerate("healBallParticle", x, y, 12, 1.f, 3.f, 13, false, 0, true);
	PARTICLE->explosionGenerate("healBallParticle", x, y, 9, 1.f, 2.f, 11, false, 0, true);
	PARTICLE->explosionGenerate("healBallParticle", x, y, 6, 1.f, 3.f, 9, false, 0, true);
	PARTICLE->explosionGenerate("healBallParticle", x, y, 3, 1.f, 4.f, 7, false, 0, true);
}

void particleManager::explosionParticlePlay(float x, float y)
{
	PARTICLE->explosionGenerate("explosionParticle", x, y, 18, 1.f, 4.f, 9, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 15, 1.f, 3.f, 7, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 12, 1.f, 2.f, 5, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 9, 1.f, 3.f, 4, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 6, 1.f, 4.f, 2, true);
}

void particleManager::explosionParticle2Play(float x, float y)
{
	PARTICLE->explosionGenerate("explosionParticle", x, y, 30, 1.f, 4.f, 3, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 24, 1.f, 3.f, 5, true);
	PARTICLE->explosionGenerate("explosionParticle", x, y, 6, 1.f, 2.f, 6, true);
}

void particleManager::bossJumpParticlePlay(float x, float y)
{
	PARTICLE->explosionGenerate("stoneX2", x, y, 18, 5.f, 4.f, 15, false, 40);
	PARTICLE->explosionGenerate("stoneX2", x, y, 15, 5.f, 3.f, 13, false, 40);
	PARTICLE->explosionGenerate("stoneX2", x, y, 12, 5.f, 2.f, 11, false, 20);
	PARTICLE->explosionGenerate("stoneX2", x, y, 9, 5.f, 3.f, 9, false, 20);
	PARTICLE->explosionGenerate("stoneX2", x, y, 6, 5.f, 4.f, 7, false, 10);
}