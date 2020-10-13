#include "stdafx.h"
#include "inventory.h"

HRESULT inventory::init()
{
	_vSlot.clear();
	IMAGEMANAGER->addImage("info", "resource/UI/playerinfoBoxSmall.bmp", 400, 588, true, RGB(255, 0, 255));
	rc = RectMakeCenter(WINSIZEX / 2 - ((WINSIZEX / 2) / 2), WINSIZEY / 2, 400, 588);
	isOpen = false;

	for (int i = 0; i < 6; i++)
	{
		slot = RectMakeCenter(rc.left + 45+(i * 60), rc.top + 100, 50, 50);
		_vSlot.push_back(slot);
	}
	return S_OK;
}

void inventory::release()
{
}

void inventory::update()
{
	if (!isOpen && INPUT->GetKeyDown(VK_TAB))
	{
		isOpen = true;
	}
	if (isOpen && INPUT->GetKeyDown(VK_TAB)) isOpen = false;
}

void inventory::render()
{
	if (isOpen) //Rectangle(getMemDC(), rc);
	{
		IMAGEMANAGER->findImage("info")->render(getMemDC(), rc.left, rc.top);
		for (int i = 0; i < _vSlot.size(); i++)
		{
			Rectangle(getMemDC(), _vSlot[i]);
		}

	}



}
