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
		slot = RectMakeCenter(rc.left + 45 + (i * 60), rc.top + 100, 50, 50);
		_vSlot.push_back(slot);
	}

	for (int i = 0; i < 14; i++)
	{
		col[i].isCol = false;
		col[i].pos.x = rc.left + 50 + ((i % 7) * 50);
		col[i].pos.y = rc.top + 235 + (i / 7) * 50;
		col[i].rc = RectMakeCenter(col[i].pos.x, col[i].pos.y, 40, 40);
	}
	descriptBox = RectMake(rc.left + 20, 450, 360, 165);
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

	if (isOpen)
	{
		for (int i = 0; i < vInven.size(); i++)
		{
			if (PtInRect(&col[i].rc, _ptMouse))
			{
				col[i].isCol = true;

			}
			else col[i].isCol = false;
		}

	}
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
		image* itemFrame = IMAGEMANAGER->findImage("itemFrame");
		vInven = PLAYERDATA->getInven();

		for (int i = 0; i < vInven.size(); i++)
		{
			itemFrame->frameRender(getMemDC(), col[i].rc.left, col[i].rc.top, vInven[i].frame.x, vInven[i].frame.y);
		}
		//Rectangle(getMemDC(), descriptBox);

		itemExplanation();
	}
}


void inventory::itemExplanation()
{
	//SetTextColor(getMemDC(), RGB(255, 255, 255));

	for (int i = 0; i < 14; i++)
	{
		if (col[i].isCol)
		{
			const char* ch = vInven[i].Explanation.c_str();
			
			DrawText(getMemDC(), ch, -1, &descriptBox, DT_CENTER | DT_WORDBREAK);
		}
	}
}