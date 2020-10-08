#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	maxHp = 500;
	hp = 500;
	coin = 123;
	_gaugeTime = 0;
	return S_OK;
}

void playerData::release()
{
}
