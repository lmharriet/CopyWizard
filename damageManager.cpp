#include "stdafx.h"
#include "damageManager.h"

HRESULT damageManager::init()
{

	time = 0;
	return S_OK;
}

void damageManager::render(HDC hdc)
{
	time++;
	image* img = IMAGEMANAGER->findImage("numbers");
	for (int i = 0; i < vDamage.size();)
	{
		int first = vDamage[i].damage % 10; // 일의 자리
		int second = vDamage[i].damage / 10; // 십의 자리

		//중력 계산
		float angle = 0.f;

		if (vDamage[i].isLeft) angle = 120 * (PI / 180);
		else angle = 60 * (PI / 180);

		//올라간다
		if (vDamage[i].isUp)
		{

			vDamage[i].x += cosf(angle) * 5;
			vDamage[i].y -= sinf(angle) * 5;

			vDamage[i].y -= vDamage[i].upForce;
			vDamage[i].upForce--;

			if (vDamage[i].upForce == 0)vDamage[i].isUp = false;
		}
		//내려간다
		else
		{
			if (time % 2 == 0)vDamage[i].gravity++;

			vDamage[i].x += cosf(angle) * 5;
			vDamage[i].y -= sinf(angle) * 5;

			vDamage[i].y += vDamage[i].gravity;
		}

		//좌표계산

		//출력
		if (second > 0)
		{
			CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x - 5 - img->getFrameWidth()/2,
				vDamage[i].y - img->getFrameHeight()/2, second, 0, vDamage[i].size);

			CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x + 5 - img->getFrameWidth() / 2,
				vDamage[i].y - img->getFrameHeight() / 2, first, 0, vDamage[i].size);
		}
		else
		{
			CAMERAMANAGER->StretchFrameRender(hdc, img, 
				vDamage[i].x - img->getFrameWidth()/2,
				vDamage[i].y - img->getFrameHeight()/2,
				first, 0, vDamage[i].size);
		}
		
		//삭제
		if (vDamage[i].lifeTime == 0) vDamage.erase(vDamage.begin() + i);
		else
		{
			if(time % 3 == 0) vDamage[i].size -= 0.07f;

			vDamage[i].lifeTime--;
			i++;
		}
	}
}

void damageManager::generator(POINT pt, int damage, bool isLeft)
{
	tagDamage Damage;

	Damage.x = pt.x;
	Damage.y = pt.y;
	Damage.damage = damage;

	Damage.upForce = 7;
	Damage.gravity = 0;
	Damage.size = 1.5f;

	Damage.isUp = true;
	Damage.isLeft = isLeft;
	Damage.lifeTime = 30;
	vDamage.push_back(Damage);
}