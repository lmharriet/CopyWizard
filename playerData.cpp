#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	maxHp = 500;
	hp = 500;
	coin = 0;
	_gaugeTime = 0;
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
