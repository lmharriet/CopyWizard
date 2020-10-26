#include "stdafx.h"
#include "uiManager.h"

HRESULT uiManager::init()
{
	IMAGEMANAGER->addImage("hpInfo", "Images/ui/leftTopInfo.bmp", 324, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBar", "Images/ui/hpBar.bmp", 242, 32);
	IMAGEMANAGER->addImage("skillGaugeBar", "Images/ui/gaugeBar.bmp", 184, 20);
	IMAGEMANAGER->addImage("skillGaugeBar_ult", "Images/ui/gaugeBar_ult.bmp", 184, 20);
	IMAGEMANAGER->addImage("uiCoin", "Images/ui/coin.bmp", 21, 17, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("pictureFrame", "Images/ui/pictureFrame.bmp", 96, 48, 2, 1);

	IMAGEMANAGER->addFrameImage("buttons", "Images/ui/button.bmp", 204, 32, 6, 1);

	IMAGEMANAGER->addFrameImage("numbers", "Images/ui/numbers.bmp", 264, 29, 13, 1);

	//skill//
	IMAGEMANAGER->addImage("nonSkill", "Images/ui/skill/nonSkill.bmp", 34, 34);


	//fire
	IMAGEMANAGER->addImage("default1", "Images/ui/skill/defaultAttack.bmp", 34, 34);
	IMAGEMANAGER->addImage("skill_searingDash", "Images/ui/skill/fireDash.bmp", 34, 34);
	IMAGEMANAGER->addImage("skill_dragonArc", "Images/ui/skill/fireDragon.bmp", 34, 34);

	IMAGEMANAGER->addImage("skill_blaze", "Images/ui/skill/blaze.bmp", 34, 34);
	IMAGEMANAGER->addImage("skill_inferno", "Images/ui/skill/inferno.bmp", 34, 34);
	IMAGEMANAGER->addImage("skill_meteor", "Images/ui/skill/meteor.bmp", 34, 34);

	// ice
	IMAGEMANAGER->addImage("skill_iceSpear", "Images/ui/skill/iceSpear.bmp", 34, 34);

	IMAGEMANAGER->addImage("coolTime", "Images/ui/coolTime.bmp", 34, 34);

	//inferno gauging bar
	IMAGEMANAGER->addImage("infernoGauging", "Images/ui/gaugingBar.bmp", 50, 10);

	IMAGEMANAGER->addImage("blackWindow", "Images/ui/blackWindow.bmp", WINSIZEX, WINSIZEY);

	IMAGEMANAGER->addFrameImage("Fbutton", "Images/ui/Fbutton.bmp", 170, 36, 5, 1);

	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		skillSlot[i].keyName = "nonSkill";
		skillSlot[i].coolTime = skillSlot[i].maxCoolTime = 0;
		skillSlot[i].available = true;
	}

	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		skillSlot[i].keyName = "nonSkill";
		skillSlot[i].maxCoolTime = 0;
	}

	gaugeBlink.isActive = false;
	gaugeBlink.isUp = true;
	gaugeBlink.opacity = 0;

	coin = 0;
	hp = 0;

	blackOpacity = 0;
	blackOpacity2 = 0;
	//

	Fbutton.keyName = "Fbutton";
	Fbutton.delay = 4;
	Fbutton.frameX = 0;
	Fbutton.isActive = false;

	itemFbutton.keyName = "Fbutton";
	itemFbutton.delay = 4;
	itemFbutton.frameX = 0;
	itemFbutton.isActive = false;
	return S_OK;
}

void uiManager::update()
{
	for (int i = 0; i < SKILLSLOT_MAX; i++)
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
	if (!gaugeBlink.isActive && PLAYERDATA->getUpgradeReady())
	{
		gaugeBlink.isActive = true;
		gaugeBlink.opacity = 0;
		gaugeBlink.isUp = true;
	}

	else if (gaugeBlink.isActive && !PLAYERDATA->getUpgradeReady())
	{
		gaugeBlink.isActive = false;
	}

	if (gaugeBlink.isActive)
	{
		if (gaugeBlink.isUp)
		{
			gaugeBlink.opacity += 5;

			if (gaugeBlink.opacity == 100)gaugeBlink.isUp = false;
		}

		else
		{
			gaugeBlink.opacity -= 5;

			if (gaugeBlink.opacity == 0)gaugeBlink.isUp = true;
		}
	}
	
	time++;

	if (Fbutton.isActive)
	{
		if (time % Fbutton.delay == 0)
		{
			if (Fbutton.frameX < 3) Fbutton.frameX++;
		}
	}
	else Fbutton.frameX = 0;

	if (itemFbutton.isActive)
	{
		if (time % itemFbutton.delay == 0)
		{
			if (itemFbutton.frameX < 3) itemFbutton.frameX++;
		}
	}
	else itemFbutton.frameX = 0;
}

void uiManager::FbuttonRender(HDC hdc)
{
	if (Fbutton.isActive == false)return;

	CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage(Fbutton.keyName),
		Fbutton.pos.x - (IMAGEMANAGER->findImage(Fbutton.keyName)->getFrameWidth() >> 1),
		Fbutton.pos.y - (IMAGEMANAGER->findImage(Fbutton.keyName)->getFrameHeight() >> 1) - 100,
		Fbutton.frameX, 0);
}

void uiManager::FbuttonRender2(HDC hdc, POINT pt)
{
	if (itemFbutton.isActive == false)return;

	CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage("Fbutton"),
		pt.x - (IMAGEMANAGER->findImage("Fbutton")->getFrameWidth() >> 1),
		pt.y - (IMAGEMANAGER->findImage("Fbutton")->getFrameHeight() >> 1) - 100,
		itemFbutton.frameX, 0);
}

void uiManager::setPoint(POINT pt)
{
	Fbutton.pos = pt;
}

void uiManager::render(HDC hdc, int destX, int destY)
{
	infoRender(hdc, destX, destY);
	coinRender(hdc);
	skillRender(hdc);

	if (blackOpacity > 0)
	{
		IMAGEMANAGER->alphaRender("blackWindow", hdc, blackOpacity);

		blackOpacity -= 5;
	}

	if (blackOpacity2 > 0)
	{
		IMAGEMANAGER->alphaRender("blackWindow", hdc, blackOpacity2);

		if(blackOpacity2 <= 250) blackOpacity2 += 5;
	}
}

void uiManager::infoRender(HDC hdc, int destX, int destY)
{
	image* img = IMAGEMANAGER->findImage("hpInfo");
	img->render(hdc, destX, destY);

	img = IMAGEMANAGER->findImage("hpBar");

	int hpBar = (float)img->getWidth() * ((float)hp / PLAYERDATA->getMaxHp());
	img->render(hdc, destX + 75, destY + 23, 0, 0, hpBar, img->getHeight());

	img = IMAGEMANAGER->findImage("skillGaugeBar");

	int skillGaugeBar = (float)img->getWidth() * (skillGauge / 100.f);
	img->render(hdc, destX + 78, destY + 60, 0, 0, skillGaugeBar, img->getHeight());

	if (gaugeBlink.isActive)
	{
		img = IMAGEMANAGER->findImage("skillGaugeBar_ult");
		img->alphaRender(hdc, destX + 78, destY + 60, 0, 0, skillGaugeBar, img->getHeight(), gaugeBlink.opacity);
	}

	img = IMAGEMANAGER->findImage("numbers");



	int tmp[3] = { hp / 100, hp / 10 % 10, hp % 10 };

	for (int i = 0; i < 3; i++)
	{
		img->frameRender(hdc, destX + 180 + (i * 17), destY + 2, tmp[i], 0);
	}

	int arr[4] = { 10,
		PLAYERDATA->getMaxHp() / 100,
		PLAYERDATA->getMaxHp() / 10 % 10,
		PLAYERDATA->getMaxHp() % 10 };

	for (int i = 0; i < 4; i++)
	{
		img->frameRender(hdc, destX + 231 + (i * 17), destY + 2, arr[i], 0);
	}




	image* gauge;
	gauge = IMAGEMANAGER->findImage("infernoGauging");

	if (PLAYERDATA->getGauging())
	{
		int infernoBar = (float)gauge->getWidth() * ((float)PLAYERDATA->getGaugeTime() / 30);
		gauge->render(hdc, WINSIZEX / 2 - 25, WINSIZEY / 2 - 70, 0, 0, infernoBar, gauge->getHeight());

	}
}

void uiManager::coinRender(HDC hdc)
{
	image* img = IMAGEMANAGER->findImage("uiCoin");
	img->render(hdc, WINSIZEX / 2 - 50, WINSIZEY - 50);

	int tmp[3] = { coin / 100, coin / 10 % 10, coin % 10 };

	img = IMAGEMANAGER->findImage("numbers");

	bool n100 = coin >= 100; // 세 자리 수 ?
	bool n10 = coin >= 10;  // 두 자리 수 ?

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

	for (int i = 0; i < SKILLSLOT_MAX; i++) img->frameRender(hdc, 27 + (i * 52), WINSIZEY - 110, i, 0);

	img = IMAGEMANAGER->findImage("pictureFrame");

	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		img->frameRender(hdc, 20 + (i * 52), WINSIZEY - 70, i / 3, 0);
	}

	//skill
	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		img = IMAGEMANAGER->findImage(skillSlot[i].keyName);
		img->render(hdc, 27 + (i * 52), WINSIZEY - 63);
	}

	//coolTime
	img = IMAGEMANAGER->findImage("coolTime");

	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		if (skillSlot[i].keyName == "nonSkill" || skillSlot[i].coolTime == 0)continue;

		int cul = img->getHeight() * (float)skillSlot[i].coolTime / skillSlot[i].maxCoolTime;

		img->alphaRender(hdc, 27 + (i * 52), WINSIZEY - 63 + cul, 0, 0, img->getWidth(), img->getHeight() - cul, 150);
	}
}


void uiManager::setSkillSlot(string keyName, int _time)
{
	int time = _time;

	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		if (skillSlot[i].keyName != "nonSkill") continue;

		skillSlot[i].keyName = keyName;
		skillSlot[i].maxCoolTime = time;

		break;
	}
}

void uiManager::setSkillSlotIndex(int index, string keyName, int sec)
{
	if (index < 0 || index > SKILLSLOT_MAX) return;

	skillSlot[index].keyName = keyName;
	skillSlot[index].maxCoolTime = sec;
}

void uiManager::fixCoolTime(string keyName, int time)
{
	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		if (keyName != skillSlot[i].keyName) continue;

		skillSlot[i].maxCoolTime = time;
	}
}

void uiManager::addCoolTime(string keyName)
{
	for (int i = 0; i < SKILLSLOT_MAX; i++)
	{
		if (keyName == skillSlot[i].keyName)
		{
			skillSlot[i].available = false;
			return;
		}
	}

	cout << keyName << "이라는 스킬은 없습니다." << '\n';
}

void uiManager::fadeIn()
{
	blackOpacity = 255;
}

void uiManager::fadeOut()
{
	blackOpacity2 = 1;
}
