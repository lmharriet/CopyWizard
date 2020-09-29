#include "stdafx.h"
#include "uiManager.h"

HRESULT uiManager::init()
{
	IMAGEMANAGER->addImage("asfe","", 100, 100);

	coin = 0;
	hp = 0;
	return S_OK;
}

