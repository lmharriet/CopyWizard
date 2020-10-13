#include "stdafx.h"
#include "uiManager.h"

HRESULT uiManager::init()
{
	IMAGEMANAGER->addImage("hpInfo", "Images/ui/leftTopInfo.bmp", 324, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBar", "Images/ui/hpBar.bmp", 242, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skillGaugeBar", "Images/ui/gaugeBar.bmp", 184, 20, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("uiCoin", "Images/ui/coin.bmp", 21, 17, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("pictureFrame", "Images/ui/pictureFrame.bmp", 96, 48, 2, 1);

	IMAGEMANAGER->addFrameImage("buttons", "Images/ui/button.bmp", 204, 32, 6, 1);

	IMAGEMANAGER->addFrameImage("numbers", "Images/ui/numbers.bmp", 264, 29, 13, 1);
	
	//skill//
	IMAGEMANAGER->addImage("nonSkill", "Images/ui/skill/nonSkill.bmp", 34, 34);


	//fire
	IMAGEMANAGER->addImage("default1", "Images/ui/skill/defaultAttack.bmp", 34, 34);
	IMAGEMANAGER->addImage("searingDash", "Images/ui/skill/fireDash.bmp", 34, 34);
	IMAGEMANAGER->addImage("fireDragon", "Images/ui/skill/fireDragon.bmp", 34, 34);

	IMAGEMANAGER->addImage("blazeIcon", "Images/ui/skill/blaze.bmp", 34, 34);
	IMAGEMANAGER->addImage("infernoIcon", "Images/ui/skill/inferno.bmp", 34, 34);
	IMAGEMANAGER->addImage("meteorIcon", "Images/ui/skill/meteor.bmp", 34, 34);


	IMAGEMANAGER->addImage("coolTime", "Images/ui/coolTime.bmp", 34, 34);

	for (int i = 0; i < 4; i++)
	{
		skillSlot[i].keyName = "nonSkill";
		skillSlot[i].coolTime = skillSlot[i].maxCoolTime = 0;
		skillSlot[i].available = true;
	}

	skillSlot[0].keyName = "blazeIcon";
	skillSlot[0].maxCoolTime = 10;

	skillSlot[1].keyName = "searingDash";
	skillSlot[1].maxCoolTime = 240;	// 4초

	skillSlot[2].keyName = "infernoIcon";
	skillSlot[2].maxCoolTime = 240; // 4초

	skillSlot[3].keyName = "meteorIcon";
	skillSlot[3].maxCoolTime = 300; // 5초

	coin = 0;
	hp = 0;
	return S_OK;
}

void uiManager::update()
{
	for (int i = 0; i < 4; i++)
	{
		//스킬이 없거나, 스킬이 사용 가능 상태 일 때는 쿨타임이 생기지않는다. 
		if (skillSlot[i].keyName == "nonSkill" || skillSlot[i].available)continue;

		skillSlot[i].coolTime++;

		if (skillSlot[i].coolTime == skillSlot[i].maxCoolTime)
		{
			skillSlot[i].coolTime = 0;
			skillSlot[i].available = true;
		}
	}

	/*if (INPUT->GetKeyDown('Q'))
	{
		if(skillSlot[3].available) skillSlot[3].available = false;
	}*/
}

void uiManager::render(HDC hdc, int destX, int destY)
{
	infoRender(hdc, destX, destY);
	coinRender(hdc);
	skillRender(hdc);
}

void uiManager::infoRender(HDC hdc, int destX, int destY)
{
	image* img = IMAGEMANAGER->findImage("hpInfo");
	img->render(hdc, destX, destY);

	img = IMAGEMANAGER->findImage("hpBar");

	int hpBar = (float)img->getWidth() * ((float)hp / 500);
	img->render(hdc, destX + 75, destY + 23, 0, 0, hpBar, img->getHeight());

	img = IMAGEMANAGER->findImage("skillGaugeBar");

	int skillGaugeBar = (float)img->getWidth() * (skillGauge / 100.f);
	img->render(hdc, destX + 78, destY + 60, 0, 0, skillGaugeBar, img->getHeight());

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

void uiManager::skillRender(HDC hdc)
{
	image* img = IMAGEMANAGER->findImage("buttons");

	for (int i = 0; i < 4; i++) img->frameRender(hdc, 27 + (i * 52), WINSIZEY - 110, i, 0);

	img = IMAGEMANAGER->findImage("pictureFrame");

	for (int i = 0; i < 4; i++)
	{
		img->frameRender(hdc, 20 + (i * 52), WINSIZEY - 70, i / 3, 0);
	}

	//skill
	for (int i = 0; i < 4; i++)
	{
		img = IMAGEMANAGER->findImage(skillSlot[i].keyName);
		img->render(hdc, 27 + (i * 52), WINSIZEY - 63);
	}

	//coolTime
	img = IMAGEMANAGER->findImage("coolTime");

	for (int i = 0; i < 4; i++)
	{
		if (skillSlot[i].keyName == "nonSkill" || skillSlot[i].coolTime == 0)continue;

		int cul = img->getHeight() * (float)skillSlot[i].coolTime / skillSlot[i].maxCoolTime;

		img->alphaRender(hdc, 27 + (i * 52), WINSIZEY - 63 + cul, 0, 0, img->getWidth(), img->getHeight() - cul, 150);
	}
}

//1sec = 60 ('초 단위'를 넣어주면 됨)
void uiManager::setSkillSlot(string keyName, int sec)
{
	int time = sec * 60;

	for (int i = 0; i < 4; i++)
	{
		if (skillSlot[i].keyName != "nonSkill") continue;

		skillSlot[i].keyName = keyName;
		skillSlot[i].maxCoolTime = time;
	}
}

void uiManager::addCoolTime(string keyName)
{
	for (int i = 0; i < 4; i++)
	{
		if (keyName == skillSlot[i].keyName)
		{
			skillSlot[i].available = false;
			return;
		}
	}

	cout << keyName << "이라는 스킬은 없습니다." << '\n';
}