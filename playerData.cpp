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

	//start
	stat.midasHand = false;
	stat.gemDropPersent = 0.f;
	stat.resurrection = false;
	stat.invincibility = false;
	stat.magicStone = false;

	gShroud.currentTime = 0;
	gShroud.curSize = 0;
	gShroud.endSize = 1.f;
	gShroud.endTime = 200;
	gShroud.isActives = false;
	//add image
	IMAGEMANAGER->addImage("glassShroud", "Images/item/glassShroud_high.bmp", 120, 120, true, RGB(255, 0, 255));

	//current Stage
	currentStage = STAGEKIND::STARTSTAGE;

	//cout << "PLAYERDATA INIT" << '\n';
	vArcanaInfo.clear();

	isInvincibility = false;
	inviCount = 0;
	return S_OK;
}

void playerData::release()
{
}

void playerData::update()
{
	if (hp > maxHp)hp = maxHp;
	else if (hp < 0)hp = 0;

	if (coin > 999)coin = 999;

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
				SOUNDMANAGER->play("shieldON", false);
			}
		}
	}


	if (stat.invincibility)
	{
		inviCount++;

		if (isInvincibility == false)
		{
			if (inviCount == 300)
			{
				isInvincibility = true;
				inviCount = 0;
			}
		}

		else // true
		{
			cout << "무적" << '\n';

			if (inviCount == 60)
			{
				isInvincibility = false;
				inviCount = 0;
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
				WINSIZEX / 2 + 4 - (img->getWidth() * gShroud.curSize / 2),
				WINSIZEY / 2 - (img->getHeight() * gShroud.curSize / 2),
				gShroud.curSize);

		}

		else
		{
			img->alphaRender(hdc, WINSIZEX / 2 - 62, WINSIZEY / 2 - 50, 100);
		}
	}
	else gShroud.curSize = 0;
}

void playerData::setGem(int _gem)
{
	char cGem[60];
	sprintf(cGem, "%d", _gem);
	INIDATA->addData("playerGem", "GEM", cGem);
	INIDATA->saveINI("INI/PLAYERDATA");
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

	//
	stat.gemDropPersent += item.gemDropPersent;

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


	else if (item.midasHand)stat.midasHand = true;

	else if (item.invincibility)stat.invincibility = true;

	else if (item.resurrection)stat.resurrection = true;

	else if (item.magicStone)stat.magicStone = true;

	if (itemName == "수상한 알약")hp = maxHp;
}

void playerData::setResurrection()
{
	stat.resurrection = false;

	for (int i = 0; i < vInven.size(); i++)
	{
		if (vInven[i].keyName == "보험증서")
		{
			vInven.erase(vInven.begin() + i);
		}
	}
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

void playerData::setArcanaInfo(int index, string keyName, string info, int coolTime)
{
	if (index > 5) return; // inventory E / R  사용하게 되면 변경 
	
	vArcanaInfo[index].skillName = keyName;
	vArcanaInfo[index].explanation = info;
	vArcanaInfo[index].coolTime = coolTime;
	UI->setSkillSlot(vArcanaInfo[index].skillName, vArcanaInfo[index].coolTime);

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