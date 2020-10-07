#include "stdafx.h"
#include "particleManager.h"

HRESULT particleManager::init()
{
	IMAGEMANAGER->addFrameImage("frameParticle", "Images/particle/frameParticle.bmp", 240, 160, 6, 4);
	IMAGEMANAGER->addFrameImage("explosionParticle", "Images/particle/explosionParticle.bmp", 960/2, 640/2, 6, 4);
	IMAGEMANAGER->addFrameImage("healBallParticle", "Images/particle/healBallParticle.bmp", 60, 5, 10, 1);
	return S_OK;
}

void particleManager::render(HDC hdc)
{
	image* img = nullptr;

	for (int i = 0; i < vParticle.size();)
	{
		img = IMAGEMANAGER->findImage(vParticle[i].keyName);

		vParticle[i].time++;
		//이동
		vParticle[i].x += cosf(vParticle[i].angle) * vParticle[i].speed;
		vParticle[i].y -= sinf(vParticle[i].angle) * vParticle[i].speed;

		CAMERAMANAGER->FrameRender(hdc, img, 
			vParticle[i].x - img->getFrameWidth()/2,
			vParticle[i].y - img->getFrameHeight()/2,
			vParticle[i].frameX,
			vParticle[i].frameY);

		//이미지
		if (vParticle[i].time % vParticle[i].delay == 0)
		{
			vParticle[i].frameX++;
		}

		//제거 (조건? maxFrame 되면 제거)
		if (vParticle[i].frameX == vParticle[i].maxFrameX) vParticle.erase(vParticle.begin() + i);
		else i++;
	}
}

void particleManager::pointGenerate(string keyName,float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay)
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

void particleManager::explosionGenerate(float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay)
{
	tagParticlePoint particlePoint;
	particlePoint.x = x;
	particlePoint.y = y;
	particlePoint.maxAngle = maxAngle;
	particlePoint.radius = radius;

	particlePoint.particleSpeed = particleSpeed;
	particlePoint.frameDelay = frameDelay;

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

		//생성
		if (vParticlePoint[i].currentTime % vParticlePoint[i].delay == 0)
		{
			//해당 방향으로 조금.. 밀어줌 (중앙에서 조금 떨어지게)
			float x = vParticlePoint[i].x + cosf(arr[vParticlePoint[i].angleNum]) * vParticlePoint[i].radius; // 10정도 밀어줌
			float y = vParticlePoint[i].y - sinf(arr[vParticlePoint[i].angleNum]) * vParticlePoint[i].radius; // 10정도 밀어줌

			generate(vParticlePoint[i].keyName, x, y, arr[vParticlePoint[i].angleNum], vParticlePoint[i].frameDelay, vParticlePoint[i].particleSpeed);
			vParticlePoint[i].angleNum++;

			if (vParticlePoint[i].angleNum == vParticlePoint[i].maxAngle)vParticlePoint[i].angleNum = 0;
		}

		//제거
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
		//생성
		for (int k = 0; k < vExplosion[i].maxAngle; k++)
		{
			float angle = (((2.f * PI) / vExplosion[i].maxAngle) * k) + RANDOM->range(-0.4f, 0.4f);
			float speed = vExplosion[i].particleSpeed + RANDOM->range(-1.f, 1.f);
			float delay = vExplosion[i].frameDelay + RANDOM->range(0, 4);

			generate("explosionParticle", vExplosion[i].x, vExplosion[i].y, angle, delay, speed);
		}

		//제거
		vExplosion.erase(vExplosion.begin() + i);
		i--;
	}
}

void particleManager::generate(string keyName ,float x, float y, float angle, int delay, float speed)
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
	
	vParticle.push_back(particle);
}