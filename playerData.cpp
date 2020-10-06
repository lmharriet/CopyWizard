#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	hp = 500;
	coin = 123;
	return S_OK;
}

void playerData::release()
{
}
