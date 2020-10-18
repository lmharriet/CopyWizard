#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	maxHp = 500;
	hp = 500;
	coin = 0;
	skillGauge = 0;
	upgradeReady = false;

	_gaugeTime = 0;
	isGauging = false;

	stat.damage = 1.f;				//100%
	stat.criDamage = 0.5f;			//100% + 50% = 150%
	stat.criChance = 0.15f;			// 15%

	//normal
	stat.potionDropChance = 0.f;	//  0%
	stat.defenceChance = 0.1f;		// 10%
	stat.burnChance = 0.f;			//  0%

	stat.goldPig = false;
	stat.CoolTimeReduction = 0;
	stat.glassMirror = false;

	//curse
	stat.vampireBlood = false;

	stat.doubleDamage = false;
	stat.ManaRejection = false;
	stat.criRejection = false;
	stat.reducedVisibility = false;
	stat.halfFace = false;
	stat.bloodGold = false;

	gShroud.currentTime = 0;
	gShroud.curSize = 0;
	gShroud.endSize = 1.f;
	gShroud.endTime = 200;
	gShroud.isActives = false;
	//add image
	IMAGEMANAGER->addImage("glassShroud", "Images/item/glassShroud_high.bmp", 120, 120, true, RGB(255, 0, 255));

	return S_OK;
}

void playerData::release()
{
}

void playerData::update()
{
	if (hp > maxHp)hp = maxHp;
	else if (hp < 0)hp = 0;

	UI->setHp(hp);
	UI->setCoin(coin);
	UI->setSkillGauge(skillGauge);

	//item special ability
	if (stat.glassMirror)
	{
		if (gShroud.isActives == false)
		{
			gShroud.currentTime++;

			if (gShroud.currentTime >= gShroud.endTime)
			{
				gShroud.isActives = true;
				gShroud.currentTime = 0;
			}
		}
	}
}

void playerData::shroudRender(HDC hdc)
{
	if (stat.glassMirror == false)return;

	if (gShroud.isActives)
	{
		image* img = IMAGEMANAGER->findImage("glassShroud");

		if (gShroud.curSize < gShroud.endSize)
		{
			gShroud.curSize += 0.025f;



			img->stretchRender(hdc,
				WINSIZEX / 2 + 4 - (img->getWidth() * gShroud.curSize / 2) ,
				WINSIZEY / 2 - (img->getHeight() * gShroud.curSize / 2) ,
				gShroud.curSize);
		}

		else
		{
			img->alphaRender(hdc, WINSIZEX / 2 - 62, WINSIZEY / 2 - 50, 100);
		}
	}
	else gShroud.curSize = 0;
}

void playerData::setStat(string itemName)
{
	tagItem item = ITEM->getItem(itemName);

	stat.damage += item.damage;
	stat.criDamage += item.criDamage;
	stat.criChance += item.criChance;
	maxHp += item.maxHp;

	stat.potionDropChance += item.potionDropChance;
	stat.defenceChance += item.defenceChance;
	stat.burnChance += item.burnChance;

	//noraml
	if (item.goldPig)stat.goldPig = true;

	else if (item.CoolTimeReduction != 0)stat.CoolTimeReduction = item.CoolTimeReduction;

	else if (item.glassMirror)
	{
		stat.glassMirror = true;
		gShroud.isActives = true;
	}

	//curse
	else if (item.vampireBlood)stat.vampireBlood = true;

	else if (item.doubleDamage)stat.doubleDamage = true;

	else if (item.ManaRejection)stat.ManaRejection = true;

	else if (item.criRejection)stat.criRejection = true;

	else if (item.reducedVisibility)stat.reducedVisibility = true;

	else if (item.halfFace)stat.halfFace = true;

	else if (item.bloodGold)stat.bloodGold = true;

	if (itemName == "������ �˾�")hp = maxHp;
}

int playerData::damageCul(int damage, bool isCritical)
{
	//cri check -> bloodGold -> halfFace
	int output = 0;

	if (isCritical == true) // cri attack
	{
		float percent = stat.damage + stat.criDamage;

		if (stat.bloodGold)
		{
			output = (float)damage * (percent + (coin / 400));
		}

		else
		{
			output = (float)damage * percent;
		}

		if (stat.halfFace)
		{
			float hpPercent = (float)hp / maxHp;

			if (hpPercent <= 0.5f)
			{
				output = output + ((float)output * 0.5f);
			}
		}
	}

	else // normal attack
	{
		float percent = stat.damage;

		if (stat.bloodGold)
		{
			output = (float)damage * (percent + (coin / 400));
		}

		else
		{
			output = (float)damage * percent;
		}

		if (stat.halfFace)
		{
			float hpPercent = (float)hp / maxHp;

			if (hpPercent <= 0.5f)
			{
				output = output + ((float)output * 0.5f);
			}
		}
	}

	return output;
}

bool playerData::criAppear()
{
	if (stat.criRejection)return false;

	int critical = stat.criChance * 100;

	int ran = RANDOM->range(1, 100);

	if (critical >= ran)
	{
		return true;
	}

	return false;
}