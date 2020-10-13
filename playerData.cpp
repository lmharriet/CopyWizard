#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	maxHp = 500;
	hp = 500;
	coin = 0;
	_gaugeTime = 0;
	skillGauge = 0;
	stat.damage = 1.f;				//100%
	stat.criDamage = 1.5f;			//150%
	stat.criChance = 0.15f;			// 15%

	stat.potionDropChance = 0.f;	//  0%
	stat.defenceChance = 0.1f;		// 10%
	stat.burnChance = 0.f;			//  0%

	stat.vampireBlood = false;

	return S_OK;
}

void playerData::release()
{
}

void playerData::update()
{
	if (hp > maxHp)hp = maxHp;

	UI->setHp(hp);
	UI->setCoin(coin);
	UI->setSkillGauge(skillGauge);
}

void playerData::setStat(string itemName)
{
	tagItem item = ITEM->getItem(itemName);

	stat.damage += item.damage;
	stat.criDamage += item.criDamage;
	stat.criChance += item.criChance;
	stat.burnChance += item.burnChance;
	stat.defenceChance += item.defenceChance;
	stat.potionDropChance += item.potionDropChance;

	if (item.vampireBlood == true) stat.vampireBlood = true;
}

bool playerData::criAppear()
{
	int critical = stat.criChance * 100;

	int ran = RANDOM->range(1, 100);

	if (critical >= ran)
	{
		return true;
	}

	return false;
}