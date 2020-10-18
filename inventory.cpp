#include "stdafx.h"
#include "inventory.h"

HRESULT inventory::init()
{
	IMAGEMANAGER->addImage("info", "resource/UI/playerinfoBoxSmall.bmp", 400, 588, true, RGB(255, 0, 255));
	rc = RectMakeCenter(WINSIZEX / 2 - ((WINSIZEX / 2) / 2), WINSIZEY / 2, 400, 588);
	isOpen = false;
	
	//arcana
	for (int i = 0; i < 6; i++)
	{
		arcanaSlot[i].isCol = false;
		arcanaSlot[i].pos.x = rc.left + 50 + (i * 61);
		arcanaSlot[i].pos.y = rc.top + 115;
		arcanaSlot[i].rc = RectMakeCenter(arcanaSlot[i].pos.x, arcanaSlot[i].pos.y, 34, 34);
	}

	//item
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
		SOUNDMANAGER->play("invenOpen", false);
	}
	if (isOpen && INPUT->GetKeyDown(VK_TAB)) { isOpen = false; SOUNDMANAGER->play("invenClose", false);	}

	if (isOpen)
	{
		//item info
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
		
		
		for (int i = 0; i < 6; i++)
		{
			Rectangle(getMemDC(), arcanaSlot[i].rc);
		}
		vArcana = PLAYERDATA->getAracaInfo();

		for (int i = 0; i < vArcana.size(); i++)
		{
			IMAGEMANAGER->render(vArcana[i].skillName, getMemDC(),
				arcanaSlot[i].rc.left, arcanaSlot[i].rc.top);
		}


		

		//item
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