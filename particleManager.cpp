#include "stdafx.h"
#include "particleManager.h"

HRESULT particleManager::init()
{
	///object pooling///
	for (int i = 0; i < 30; i++)
	{
		vTemp.push_back(new tagParticlePoint);
	}

	for (int i = 0; i < 500; i++)
	{
		vTempParticle.push_back(new tagParticle);
	}
	//

	//add image
	IMAGEMANAGER->addFrameImage("frameParticle", "Images/particle/frameParticle.bmp", 240, 160, 6, 4);
	IMAGEMANAGER->addFrameImage("explosionParticle", "Images/particle/explosionParticle.bmp", 480, 320, 6, 4);
	IMAGEMANAGER->addFrameImage("explosionParticle2", "Images/particle/explosionParticle(320,213).bmp", 320, 213, 6, 4);
	IMAGEMANAGER->addFrameImage("healBallParticle", "Images/particle/healBallParticle.bmp", 60, 5, 10, 1);
	IMAGEMANAGER->addFrameImage("bossJumpParticle", "Images/particle/stone.bmp", 60, 15, 4, 1);
	
	IMAGEMANAGER->addFrameImage("stoneX2", "Images/particle/stoneX2.bmp", 34, 122, 1, 4);
	IMAGEMANAGER->addFrameImage("smokeX2", "Images/particle/smokeParticle.bmp", 480, 320, 6, 4);
	IMAGEMANAGER->addFrameImage("smokeX4", "Images/particle/smokeParticle.bmp", 480 >> 1, 320 >> 1, 6, 4);

	return S_OK;
}

tagParticle* particleManager::getTempParticle()
{
	tagParticle* particle;

	if (vTempParticle.empty())
	{
		particle = new tagParticle;
	}

	else
	{
		particle = vTempParticle[0];
		vTempParticle.erase(vTempParticle.begin());
	}

	return particle;
}

tagParticlePoint* particleManager::getTempPoint()
{
	tagParticlePoint* point;

	if (vTemp.empty())
	{
		point = new tagParticlePoint;
	}
	else
	{
		point = vTemp[0];
		vTemp.erase(vTemp.begin());
	}

	return point;
}

void particleManager::returnPoint(tagParticlePoint* particle)
{
	vTemp.push_back(particle);
}

void particleManager::returnParticle(tagParticle* particle)
{
	vTempParticle.push_back(particle);
}

void particleManager::render(HDC hdc)
{
	if (vParticle.empty())return;

	image* img = nullptr;

	int size = vParticle.size();
	for (int i = 0; i < size;)
	{
		img = IMAGEMANAGER->findImage(vParticle[i]->keyName);

		vParticle[i]->time++;

		//이동1
		if (vParticle[i]->isBack == false)
		{
			vParticle[i]->x += cosf(vParticle[i]->angle) * vParticle[i]->speed;
			vParticle[i]->y -= sinf(vParticle[i]->angle) * vParticle[i]->speed;
		}

		//이동2
		if (vParticle[i]->isBack == true)
		{
			if (vParticle[i]->backTime > 0)
			{
				vParticle[i]->backTime--;

				vParticle[i]->x += cosf(vParticle[i]->angle) * vParticle[i]->speed;
				vParticle[i]->y -= sinf(vParticle[i]->angle) * vParticle[i]->speed;
			}
			else
			{
				float angle = getAngle(vParticle[i]->x, vParticle[i]->y, PLAYERDATA->getX(), PLAYERDATA->getY());

				vParticle[i]->speed *= 1.05f;

				vParticle[i]->x += cosf(angle) * vParticle[i]->speed * 1.3f;
				vParticle[i]->y -= sinf(angle) * vParticle[i]->speed * 1.3f;
			}
		}

		CAMERAMANAGER->FrameRender(hdc, img,
			vParticle[i]->x - img->getFrameWidth() / 2,
			vParticle[i]->y - img->getFrameHeight() / 2,
			vParticle[i]->frameX,
			vParticle[i]->frameY);

		//이미지
		if (vParticle[i]->time % vParticle[i]->delay == 0)
		{
			vParticle[i]->frameX++;
		}

		//제거

		//(조건 ? 시간이 어느정도 지나거나, 중앙 좌표렉트와 충돌할 때)
		if (vParticle[i]->isCollect) // collect effect
		{
			RECT rc1 = RectMakeCenter(vParticle[i]->x, vParticle[i]->y, 10, 10);
			RECT rc2 = RectMakeCenter(vParticle[i]->endX, vParticle[i]->endY, 30, 30);

			if (colCheck(rc1, rc2) || vParticle[i]->time > 50)
			{
				returnParticle(vParticle[i]);

				vParticle.erase(vParticle.begin() + i);
				size = vParticle.size();
			}
			else i++;
		}

		else if (vParticle[i]->isBack)
		{
			RECT rc1 = RectMakeCenter(vParticle[i]->x, vParticle[i]->y, 10, 10);
			RECT rc2 = RectMakeCenter(PLAYERDATA->getX(), PLAYERDATA->getY(), 20, 20);

			if (vParticle[i]->backTime != 0)
			{
				i++;
				continue;
			}

			if (colCheck(rc1, rc2) || vParticle[i]->time > 300)
			{
				returnParticle(vParticle[i]);

				vParticle.erase(vParticle.begin() + i);
				size = vParticle.size();
			}
			else i++;
		}

		else // default
		{
			if (vParticle[i]->isFrameDel) //(조건 ? maxFrame 되면 제거)
			{

				if (vParticle[i]->frameX == vParticle[i]->maxFrameX)
				{
					returnParticle(vParticle[i]);

					vParticle.erase(vParticle.begin() + i);
					size = vParticle.size();
				}
				else i++;

			}

			else //(조건 ? 시간이 되면 제거)
			{
				
				if (vParticle[i]->time == vParticle[i]->lifeTime)
				{
					returnParticle(vParticle[i]);

					vParticle.erase(vParticle.begin() + i);
					size = vParticle.size();
				}
				else i++;

			}
		}
	}
}

void particleManager::pointGenerate(string keyName, float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay)
{
	tagParticlePoint* particlePoint = getTempPoint();
	particlePoint->x = x;
	particlePoint->y = y;
	particlePoint->currentTime = 0;
	particlePoint->delay = CreateDelay;
	particlePoint->lifeTime = lifeTime;
	particlePoint->angleNum = 0;
	particlePoint->maxAngle = maxAngle;
	particlePoint->radius = radius;

	particlePoint->particleSpeed = particleSpeed;
	particlePoint->frameDelay = frameDelay;

	particlePoint->keyName = keyName;

	vParticlePoint.push_back(particlePoint);
}

void particleManager::explosionGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, bool isFrameDel, int particleEndTime, bool isBack)
{
	tagParticlePoint* particlePoint = getTempPoint();

	particlePoint->x = x;
	particlePoint->y = y;
	particlePoint->maxAngle = maxAngle;
	particlePoint->radius = radius;

	particlePoint->particleSpeed = particleSpeed;
	particlePoint->frameDelay = frameDelay;

	particlePoint->keyName = keyName;
	particlePoint->isBack = isBack;
	particlePoint->isCollect = false;

	particlePoint->currentTime = 0;
	particlePoint->lifeTime = 0;
	particlePoint->createDelay = 1;

	particlePoint->isFrameDel = isFrameDel;
	particlePoint->particleEndTime = particleEndTime;

	vExplosion.push_back(particlePoint);
}

void particleManager::collectingGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, int lifeTime , int createDelay)
{
	tagParticlePoint* particlePoint = getTempPoint();

	particlePoint->x = x;
	particlePoint->y = y;
	particlePoint->maxAngle = maxAngle;
	particlePoint->radius = radius;

	particlePoint->particleSpeed = particleSpeed;
	particlePoint->frameDelay = frameDelay;

	particlePoint->keyName = keyName;
	particlePoint->isBack = false;
	particlePoint->isCollect = true;

	particlePoint->currentTime = 0;
	particlePoint->lifeTime = lifeTime;
	particlePoint->createDelay = createDelay;

	particlePoint->isFrameDel = false;
	particlePoint->particleEndTime = 50;

	vExplosion.push_back(particlePoint);
}

void particleManager::pointActive()
{
	if (vParticlePoint.empty())return;

	float* arr;
	int size = vParticlePoint.size();
	for (int i = 0; i < size;)
	{
		arr = new float[vParticlePoint[i]->maxAngle];

		for (int k = 0; k < vParticlePoint[i]->maxAngle; k++)
		{
			arr[k] = (((2.f * PI) / vParticlePoint[i]->maxAngle) * k);
		}

		vParticlePoint[i]->currentTime++;

		//생성
		if (vParticlePoint[i]->currentTime % vParticlePoint[i]->delay == 0)
		{
			//해당 방향으로 조금.. 밀어줌 (중앙에서 조금 떨어지게)
			float x = vParticlePoint[i]->x + cosf(arr[vParticlePoint[i]->angleNum]) * vParticlePoint[i]->radius; // 10정도 밀어줌
			float y = vParticlePoint[i]->y - sinf(arr[vParticlePoint[i]->angleNum]) * vParticlePoint[i]->radius; // 10정도 밀어줌

			generate(vParticlePoint[i]->keyName, x, y, arr[vParticlePoint[i]->angleNum], vParticlePoint[i]->frameDelay, vParticlePoint[i]->particleSpeed, true);
			vParticlePoint[i]->angleNum++;

			if (vParticlePoint[i]->angleNum == vParticlePoint[i]->maxAngle)vParticlePoint[i]->angleNum = 0;
		}

		//제거
		if (vParticlePoint[i]->currentTime == vParticlePoint[i]->lifeTime)
		{
			returnPoint(vParticlePoint[i]);

			vParticlePoint.erase(vParticlePoint.begin() + i);
			size = vParticlePoint.size();
		}
		else i++;
	}
}

void particleManager::explosionActive()
{
	if (vExplosion.empty())return;

	int size = vExplosion.size();
	for (int i = 0; i < size; i++)
	{
		if (vExplosion[i]->lifeTime > 0)
		{
			vExplosion[i]->currentTime++;
			if (vExplosion[i]->currentTime % vExplosion[i]->createDelay != 0)continue;
		}

		//생성
		for (int k = 0; k < vExplosion[i]->maxAngle; k++)
		{
			float angle = (((2.f * PI) / vExplosion[i]->maxAngle) * k) + RANDOM->range(-0.4f, 0.4f);
			float speed = vExplosion[i]->particleSpeed + RANDOM->range(-1.f, 1.f);
			if (speed <= 0.f)speed = 0.3f;
			float delay = vExplosion[i]->frameDelay + RANDOM->range(0, 4);

			float x = vExplosion[i]->x + cosf(angle) * vExplosion[i]->radius;
			float y = vExplosion[i]->y - sinf(angle) * vExplosion[i]->radius;

			float culAngle = 0.f;
			if(vExplosion[i]->isCollect) culAngle = getAngle(x, y, vExplosion[i]->x, vExplosion[i]->y);

			if (vExplosion[i]->isCollect) generate2(vExplosion[i]->keyName, x, y, culAngle, delay, speed, vExplosion[i]->x, vExplosion[i]->y);
			else generate(vExplosion[i]->keyName, x, y, angle, delay, speed, vExplosion[i]->isFrameDel, vExplosion[i]->particleEndTime, vExplosion[i]->isBack, 37);
		}

		//제거
		if (vExplosion[i]->lifeTime > 0) // 시간 순?
		{

			if (vExplosion[i]->currentTime == vExplosion[i]->lifeTime)
			{
				returnPoint(vExplosion[i]);

				vExplosion.erase(vExplosion.begin() + i);
				size = vExplosion.size();
			}

			else
			{
				i--;
			}

		}

		else // 바로 삭제
		{
			returnPoint(vExplosion[i]);

			vExplosion.erase(vExplosion.begin() + i);
			size = vExplosion.size();
			i--;
		}
	}
}

void particleManager::generate(string keyName, float x, float y, float angle, int delay, float speed, bool isFrameDel, int particleEndTime, bool isBack, int backTime)
{
	tagParticle* particle = getTempParticle();

	//default info
	particle->x = x;
	particle->y = y;
	particle->angle = angle;
	particle->delay = delay;
	particle->speed = speed;
	particle->time = 0;

	particle->isFrameDel = isFrameDel;
	particle->lifeTime = particleEndTime;

	image* img = IMAGEMANAGER->findImage(keyName);
	particle->keyName = keyName;
	//image info
	particle->frameX = 0;
	particle->maxFrameX = img->getMaxFrameX();
	particle->frameY = RANDOM->range(0, img->getMaxFrameY());

	particle->isBack = isBack;
	particle->backTime = backTime;
	particle->isCollect = false;
	vParticle.push_back(particle);
}

void particleManager::generate2(string keyName, float x, float y, float angle, int delay, float speed, float endX, float endY, bool isCollect)
{
	tagParticle* particle = getTempParticle();

	//default info
	particle->x = x;
	particle->y = y;
	particle->angle = angle;
	particle->delay = delay;
	particle->speed = speed;
	particle->time = 0;

	image* img = IMAGEMANAGER->findImage(keyName);
	particle->keyName = keyName;
	//image info
	particle->frameX = 0;
	particle->maxFrameX = img->getMaxFrameX();
	particle->frameY = RANDOM->range(0, img->getMaxFrameY());

	particle->isBack = false;
	particle->backTime = 0;

	particle->isCollect = isCollect;
	particle->endX = endX;
	particle->endY = endY;
	vParticle.push_back(particle);
}

//

void particleManager::potionParticlePlay(float x, float y)
{
	explosionGenerate("healBallParticle", x, y, 18, 1.f, 2.f, 15, false, 210, true);
	explosionGenerate("healBallParticle", x, y, 15, 1.f, 1.f, 13, false, 200, true);
	explosionGenerate("healBallParticle", x, y, 12, 1.f, 0.7f, 11, false, 190, true);
	explosionGenerate("healBallParticle", x, y, 8, 1.f, 0.4f, 9, false, 180, true);
	explosionGenerate("healBallParticle", x, y, 5, 1.f, 0.3f, 7, false, 170, true);
}

void particleManager::explosionParticlePlay(float x, float y)
{
	explosionGenerate("explosionParticle", x, y, 18, 1.f, 4.f, 9, true);
	explosionGenerate("explosionParticle", x, y, 15, 1.f, 3.f, 7, true);
	explosionGenerate("explosionParticle", x, y, 12, 1.f, 2.f, 5, true);
	explosionGenerate("explosionParticle", x, y, 9, 1.f, 3.f, 4, true);
	explosionGenerate("explosionParticle", x, y, 6, 1.f, 4.f, 2, true);
}

void particleManager::explosionParticle2Play(float x, float y)
{
	explosionGenerate("explosionParticle", x, y, 30, 1.f, 4.f, 3, true);
	explosionGenerate("explosionParticle", x, y, 24, 1.f, 3.f, 5, true);
	explosionGenerate("explosionParticle", x, y, 6, 1.f, 2.f, 6, true);
}

void particleManager::burningParticlePlay(float x, float y)
{
	explosionGenerate("frameParticle", x, y, 5, 1.f, 2.f, 3, true);
}

void particleManager::burningParticle2Play(float x, float y)
{
	//explosionGenerate("frameParticle", x, y, 30, 1.f, 4.f, 3, true);
	//explosionGenerate("explosionParticle2", x, y, 6, 5.f, 0.f, 2, true);
	//explosionGenerate("frameParticle", x, y, 6, 10.f, 0.f, 3, true);
}

void particleManager::bossJumpParticlePlay(float x, float y)
{
	explosionGenerate("stoneX2", x, y, 18, 5.f, 4.f, 15, false, 40);
	explosionGenerate("stoneX2", x, y, 15, 5.f, 3.f, 13, false, 40);
	explosionGenerate("stoneX2", x, y, 12, 5.f, 2.f, 11, false, 20);
	explosionGenerate("stoneX2", x, y, 9, 5.f, 3.f, 9, false, 20);
	explosionGenerate("stoneX2", x, y, 6, 5.f, 4.f, 7, false, 10);
}

void particleManager::crashRockParticlePlay(float x, float y)
{
	
	explosionGenerate("stoneX2", x, y, 9, 5.f, 3.f, 3, false, 20);
	explosionGenerate("stoneX2", x, y, 6, 5.f, 4.f, 2, false, 10);
}

void particleManager::smokeParticlePlay(float x, float y)
{
	generate("smokeX2", x - 25, y - 35, 0.f, 6, 0.f, true);
	for (int i = 0; i < 6; i++)
	{
		int ranX = RANDOM->range(-10, 20) * 3;
		int ranY = RANDOM->range(-10, 20) * 2;
		generate("smokeX2", x - 45 + ranX, y - 55 + ranY, 0.f, 6, 0.f, true);
	}
}