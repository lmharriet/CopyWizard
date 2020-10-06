#include "stdafx.h"
#include "particleManager.h"

HRESULT particleManager::init()
{
	for (int i = 0; i < 1000; i++)
	{
		vParticlePuller.push_back(new tagParticle(0,0,0,0,0,0,0,0));
	}

	return S_OK;
}

tagParticle* particleManager::getParticle()
{
	//cout << vParticlePuller.size() << " " << vParticle.size() << endl;

	tagParticle* rtnParticle;
	if (vParticlePuller.size() > 0)
	{
		rtnParticle = vParticlePuller[0];
		vParticlePuller.erase(vParticlePuller.begin());
	}
	
	else
	{
		rtnParticle = new tagParticle(0, 0, 0, 0, 0, 0, 0, 0);
	}

	return rtnParticle;
}

void particleManager::rtnParticle(tagParticle* ptc)
{
	vParticlePuller.push_back(ptc);
}

HRESULT particleManager::generate(int x, int y, int size, int gravity, float angle, int delay, int speed)
{
	tagParticle* particle = getParticle();
	
	particle->x = x;
	particle->y = y;
	particle->size = size;
	particle->gravity = gravity;
	particle->angle = angle;
	particle->delay = delay;
	particle->fixedDelay = delay;
	particle->speed = speed;

	vParticle.push_back(particle);
	return S_OK;
}

HRESULT particleManager::pointGenerate(int x, int y, int number, float minAngle, float maxAngle, int CreateDelay, int time)
{
	tagParticlePoint partiPoint = { x, y, number, minAngle, maxAngle, CreateDelay, 0, time };

	particlePoint.push_back(partiPoint);
	return S_OK;
}

void particleManager::render(HDC hdc, RECT cam)
{
	for (int i = 0; i < vParticle.size(); i++)
	{
		int culX = CAMERAMANAGER->GetRelativeX(vParticle[i]->x);
		int culY = CAMERAMANAGER->GetRelativeY(vParticle[i]->y);
		culX += cosf(vParticle[i]->angle) * vParticle[i]->speed;
		culY += -sinf(vParticle[i]->angle) * vParticle[i]->speed + vParticle[i]->gravity;

		RectangleMakeCenter(hdc, culX, culY, vParticle[i]->size, vParticle[i]->size);
		
		if (vParticle[i]->size > 0)
		{
			vParticle[i]->delay--;
			vParticle[i]->speed++;
			vParticle[i]->gravity++;

			if (vParticle[i]->delay <= 0)
			{
				vParticle[i]->delay = vParticle[i]->fixedDelay;
				vParticle[i]->size--;
			}
		}
		else
		{
			rtnParticle(vParticle[i]);
			vParticle.erase(vParticle.begin() + i);
			i--;
		}
	}
}

void particleManager::tRender(HDC hdc)
{
	for (int i = 0; i < vParticle.size(); i++)
	{
		int culX = CAMERAMANAGER->GetRelativeX(vParticle[i]->x);
		int culY = CAMERAMANAGER->GetRelativeY(vParticle[i]->y);
		culX += cosf(vParticle[i]->angle) * vParticle[i]->speed;
		culY += -sinf(vParticle[i]->angle) * vParticle[i]->speed;

		RectangleMakeCenter(hdc, culX, culY, vParticle[i]->size, vParticle[i]->size);

		if (vParticle[i]->size > 0)
		{
			vParticle[i]->delay--;
			vParticle[i]->speed++;
			vParticle[i]->gravity++;

			if (vParticle[i]->delay <= 0)
			{
				vParticle[i]->delay = vParticle[i]->fixedDelay;
				vParticle[i]->size--;
			}
		}
		else
		{
			rtnParticle(vParticle[i]);
			vParticle.erase(vParticle.begin() + i);
			i--;
		}
	}
}

void particleManager::pointActive()
{
	int i = 0;
	vector<tagParticlePoint>::iterator iter;
	for (iter = particlePoint.begin(); iter != particlePoint.end(); ++iter, ++i)
	{
		if (iter->delay > 0)
		{
			iter->delay--;
		}
		else
		{
			iter->delay = iter->CreateDelay;

			int ranX = RANDOM->range(-3, 3);
			int ranY = RANDOM->range(-3, 3);
			int ranSize = RANDOM->range(10, 20);
			float ranAngle = RANDOM->range(1.f, 2.1f);

			generate(iter->x + ranX, iter->y + ranY, 10, 0, ranAngle, 5, 1);
		}
		iter->time--;

		if (iter->time == 0)
		{
			particlePoint.erase(particlePoint.begin() + i);
			break;
		}
	}

}

HRESULT particleManager0::init()
{
	IMAGEMANAGER->addFrameImage("frameParticle", "Images/particle/frameParticle.bmp", 240, 160, 6, 4);
	IMAGEMANAGER->addFrameImage("explosionParticle", "Images/particle/explosionParticle.bmp", 960*5, 640*5, 6, 4);
	return S_OK;
}

void particleManager0::render(HDC hdc)
{
	image* img = IMAGEMANAGER->findImage("frameParticle");

	for (int i = 0; i < vParticle.size();)
	{
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

void particleManager0::pointGenerate(float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay)
{
	tagParticlePoint2 particlePoint;
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

	vParticlePoint.push_back(particlePoint);
}

void particleManager0::explosionGenerate(float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay)
{
	tagParticlePoint2 particlePoint;
	particlePoint.x = x;
	particlePoint.y = y;
	particlePoint.maxAngle = maxAngle;
	particlePoint.radius = radius;

	particlePoint.particleSpeed = particleSpeed;
	particlePoint.frameDelay = frameDelay;

	vExplosion.push_back(particlePoint);
}


void particleManager0::pointActive()
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

			generate("frameParticle", x, y, arr[vParticlePoint[i].angleNum], vParticlePoint[i].frameDelay, vParticlePoint[i].particleSpeed);
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

void particleManager0::explosionActive()
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

void particleManager0::generate(string keyName ,float x, float y, float angle, int delay, float speed)
{
	tagParticle2 particle;

	//default info
	particle.x = x;
	particle.y = y;
	particle.angle = angle;
	particle.delay = delay;
	particle.speed = speed;
	particle.time = 0;

	image* img = IMAGEMANAGER->findImage(keyName);
	//image info
	particle.frameX = 0;
	particle.maxFrameX = img->getMaxFrameX();
	particle.frameY = RANDOM->range(0, img->getMaxFrameY());
	
	vParticle.push_back(particle);
}