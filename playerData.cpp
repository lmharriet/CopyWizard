#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	maxHp = 500;
	hp = 500;
	coin = 0;
	_gaugeTime = 0;
	
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
}

void playerData::setStat(string statName, float value)
{
	if (statName == "damage") stat.damage += value;
	else if (statName == "criDamage") stat.criDamage += value;
	else if (statName == "criChance") stat.criChance += value;
	else if (statName == "potionDropChance") stat.potionDropChance += value;
	else if (statName == "defenceChance") stat.defenceChance += value;
	else if (statName == "burnChance") stat.burnChance += value;
	else if (statName == "vampireBlood") stat.vampireBlood = true;

	else cout << "ÂÄ½Å¾ÈÂ÷·Á?" << '\n';
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