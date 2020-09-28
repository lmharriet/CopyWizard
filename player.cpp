#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small.bmp", 1000, 2400, 10, 24);
	posX = WINSIZEX / 2;
	posY = WINSIZEY / 2;
	rc = RectMakeCenter(posX, posY, 100, 100);

	speed = index = count = 0;
	isLeft = isUp = false;

	state = IDLE;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	controller();
	tileCol();


	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}


void player::render()
{
	for (int i = 0; i < 8; i++)
	{
		CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i].rc);
		//Rectangle(getMemDC(), pixelCheck[i].rc);
	}


	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), rc.left, rc.top, 0, 0);



	char text[126];

	for (int i = 0; i < 8; i++)
	{
		wsprintf(text, "pixelRc %d ,isCol =%d, pixelRc.left %d, pixelRc.top %d", i, tileCheck[i].isCol, tileCheck[i].rc.left, tileCheck[i].rc.top);
		textOut(getMemDC(), 80, 50 + i * 20, text, BLACK);
	}


}

void player::controller()
{
	if (INPUT->GetKey(VK_LEFT)&&!tileCheck[0].isCol && !tileCheck[1].isCol && !tileCheck[2].isCol)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!tileCheck[i].isCol)
			{
				posX -= 5;
				rc = RectMakeCenter(posX, posY, 100, 100);
				isLeft = true;
			}
		}
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		for (int i = 3; i < 6; i++)
		{
			if (!tileCheck[i].isCol)
			{
				posX += 5;
				rc = RectMakeCenter(posX, posY, 100, 100);
				isLeft = false;
			}
		}
	}
	if (INPUT->GetKey(VK_UP)&& !tileCheck[0].isCol && !tileCheck[3].isCol && !tileCheck[6].isCol)
	{

		posY -= 5;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = true;

	}
	if (INPUT->GetKey(VK_DOWN)&& !tileCheck[2].isCol && !tileCheck[5].isCol && !tileCheck[7].isCol)
	{
		posY += 5;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = false;
	}

}

void player::tileCol()
{
	for (int i = 0; i < 8; i++)
	{
		tileCheck[i].isCol = false;
		for (int j = 0; j < MAXTILE; j++)
		{
			if (tile[j].keyName != "") continue;

			if (colCheck(tileCheck[i].rc, tile[j].rc))
			{
				tileCheck[i].isCol = true;
				break;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30), 10, 10);
		tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30), 10, 10);
	}
	tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30), 10, 10);
	tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);

	//tileCheck[0].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30), 10, 10);
	//tileCheck[1].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30), 10, 10);
	//tileCheck[2].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
	//tileCheck[3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
	//tileCheck[4].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
	//tileCheck[5].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
	//tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
	//tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 20), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30), 10, 10);
}
