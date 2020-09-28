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

	for (int i = 0; i < 3; i++)
	{
		tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30) };
		tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30) };
	}

	tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30) };
	tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30) };

	for (int i = 0; i < 8; i++)tileCheck[i].isCol = false;
	return S_OK;
}

void player::release()
{

}

void player::update()
{
	controller();

	for (int i = 0; i < 3; i++)
	{
		tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30) };
		tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30) };
	}

	tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30) };
	tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 30) };

	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}


void player::render()
{


	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), rc.left, rc.top, 0, 0);
	//char text[126];

	char str[126];
	for (int i = 0; i < 8; i++)
	{
		CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(tileCheck[i].pos.x, tileCheck[i].pos.y, 7, 7));
		//Rectangle(getMemDC(), tileCheck[i].rc);
		wsprintf(str, "check %d : %d", i, tileCheck[i].isCol);
		TextOut(getMemDC(), 10, 140 + (i * 20), str, strlen(str));
	}

	//wsprintf(text, "pos :%d", tileCheck[0].pos.x);
	//textOut(getMemDC(), 80, 140, text, BLACK);

}

void player::controller()
{
	if (INPUT->GetKey(VK_LEFT))
	{
		tileCol();
		posX -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = true;

	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		tileCol();
		posX += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = false;

	}
	if (INPUT->GetKey(VK_UP))
	{
		tileCol();
		posY -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = true;

	}
	if (INPUT->GetKey(VK_DOWN))
	{
		tileCol();
		posY += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = false;

	}
}

void player::tileCol()
{
	for (int i = 0; i < 8; i++)tileCheck[i].isCol = false;

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].keyName != "") continue;

		for (int j = 0; j < 8; j++)
		{
			if (PtInRect(&tile[i].rc, tileCheck[j].pos))tileCheck[j].isCol = true;
		}
	}
}
