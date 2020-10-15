#include "stdafx.h"
#include "damageManager.h"

HRESULT damageManager::init()
{
	IMAGEMANAGER->addImage("critical", "Images/effect/critical.bmp", 96, 65, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("rNumbers", "Images/ui/rNumbers.bmp", 264, 29, 13, 1);
	time = 0;
	return S_OK;
}

void damageManager::render(HDC hdc)
{
	time++;

	image* cri = IMAGEMANAGER->findImage("critical");

	for (int i = 0; i < vDamage.size();)
	{
		image* img = IMAGEMANAGER->findImage(vDamage[i].keyName);

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
			if (vDamage[i].isCritical == false)
			{
				CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x - 5 - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2, second, 0, vDamage[i].size);

				CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x + 5 - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2, first, 0, vDamage[i].size);
			}

			else // critical == true
			{
				//cri render
				CAMERAMANAGER->StretchRender(hdc, cri, vDamage[i].x - cri->getWidth() / 2,
					vDamage[i].y - cri->getHeight() / 2, vDamage[i].size);
				
				//number render
				CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x - 5 - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2, second, 0, vDamage[i].size);

				CAMERAMANAGER->StretchFrameRender(hdc, img, vDamage[i].x + 5 - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2, first, 0, vDamage[i].size);
			}
		}
		else
		{
			if (vDamage[i].isCritical == false)
			{
				CAMERAMANAGER->StretchFrameRender(hdc, img,
					vDamage[i].x - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2,
					first, 0, vDamage[i].size);
			}

			else // critical == true
			{
				//cri render
				CAMERAMANAGER->StretchRender(hdc, cri, vDamage[i].x - cri->getWidth() / 2,
					vDamage[i].y - cri->getHeight() / 2, vDamage[i].size);

				//number render
				CAMERAMANAGER->StretchFrameRender(hdc, img,
					vDamage[i].x - img->getFrameWidth() / 2,
					vDamage[i].y - img->getFrameHeight() / 2,
					first, 0, vDamage[i].size);
			}
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

void damageManager::generator(POINT pt, string keyName, int damage, bool isLeft, bool isCritical)
{
	tagHit Damage;
	Damage.keyName = keyName;

	Damage.x = pt.x;
	Damage.y = pt.y;
	Damage.damage = damage;

	Damage.upForce = 7;
	Damage.gravity = 0;

	Damage.isCritical = isCritical;

	Damage.size = 1.5f;

	Damage.isUp = true;
	Damage.isLeft = isLeft;
	Damage.lifeTime = 30;
	vDamage.push_back(Damage);
}