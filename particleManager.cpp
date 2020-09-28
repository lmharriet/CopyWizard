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

HRESULT particleManager::pointInit(int x, int y, int number, float minAngle, float maxAngle, int CreateDelay)
{
	tagParticlePoint partiPoint = { x, y, number, minAngle, maxAngle, CreateDelay, 0 };

	particlePoint.push_back(partiPoint);
	return S_OK;
}

void particleManager::active(HDC hdc, RECT cam)
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

void particleManager::pointActive()
{
	vector<tagParticlePoint>::iterator iter;
	for (iter = particlePoint.begin(); iter != particlePoint.end(); ++iter)
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
			// cout << "파티클 생성" << '\n';
		}
	}
}