#include "stdafx.h"
#include "uiManager.h"

HRESULT uiManager::init()
{
	IMAGEMANAGER->addImage("hpInfo", "Images/ui/leftTopInfo.bmp", 324, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBar", "Images/ui/hpBar.bmp", 242, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("uiCoin", "Images/ui/coin.bmp", 21, 17, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("numbers", "Images/ui/numbers.bmp", 264, 29, 13, 1);

	coin = 123;
	hp = 470;
	return S_OK;
}

void uiManager::infoRender(HDC hdc, int destX, int destY)
{
	image* img = IMAGEMANAGER->findImage("hpInfo");
	img->render(hdc, destX, destY);

	img = IMAGEMANAGER->findImage("hpBar");

	int hpBar = (float)img->getWidth() * ((float)hp / 500);
	img->render(hdc, destX + 75, destY + 23, 0, 0, hpBar, img->getHeight());

	img = IMAGEMANAGER->findImage("numbers");

	int tmp[3] = { hp / 100, hp / 10 % 10, hp % 10 };

	for (int i = 0; i < 3; i++)
	{
		img->frameRender(hdc, destX + 180 + (i * 17), destY + 2, tmp[i], 0);
	}

	int arr[4] = { 10,5,0,0 };

	for (int i = 0; i < 4; i++)
	{
		img->frameRender(hdc, destX + 231 + (i * 17), destY + 2, arr[i], 0);
	}
}

void uiManager::coinRender(HDC hdc)
{
	image* img = IMAGEMANAGER->findImage("uiCoin");
	img->render(hdc, WINSIZEX / 2 - 50, WINSIZEY - 50);

	int tmp[3] = { coin / 100, coin / 10 % 10, coin % 10 };

	img = IMAGEMANAGER->findImage("numbers");

	bool n100 = coin >= 100; // 세 자리 수 ?
	bool n10  = coin >= 10;  // 두 자리 수 ?

	int cycle = n100 + n10 + 1;

	for (int i = 0; i < cycle; i++)
	{
		if (n100)
		{
			img->frameRender(hdc, WINSIZEX / 2 - 20 + (i * 17), WINSIZEY - 55, tmp[i], 0);
		}
		else if (n10)
		{
			img->frameRender(hdc, WINSIZEX / 2 - 20 + (i * 17), WINSIZEY - 55, tmp[i + 1], 0);
		}
		else
		{
			img->frameRender(hdc, WINSIZEX / 2 - 20 + (i * 17), WINSIZEY - 55, tmp[i + 2], 0);
		}
	}
}