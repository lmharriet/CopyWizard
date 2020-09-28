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
	isLeft = isUp = isDash = false;

	state = IDLE;

	for (int i = 0; i < 3; i++)
	{
		tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		//tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
		tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		//tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
	}

	tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 40) };
	//tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50), 10, 10);
	tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 50) };
	//tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 50), 10, 10);

	memset(tileCheck, 0, sizeof(tileCheck));

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	controller();
	tileCol();
	for (int i = 0; i < 3; i++)
	{
		tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
	}

	tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 10) };
	tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 70) };


	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}


void player::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX-50, posY-50, 0, 0);
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
	if (INPUT->GetKey(VK_LEFT) && !tileCheck[1].isCol 
		&& (!tileCheck[0].isCol || !tileCheck[2].isCol))
	{
		posX -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = true;
	}
	if (INPUT->GetKey(VK_RIGHT) &&!tileCheck[4].isCol 
		&&(!tileCheck[3].isCol || !tileCheck[5].isCol))
	{
		posX += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = false;
	}
	if (INPUT->GetKey(VK_UP)&&!tileCheck[6].isCol)
	{
		//tileCol();
		posY -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = true;
	}
	if (INPUT->GetKey(VK_DOWN) && !tileCheck[7].isCol)
	{
		//tileCol();
		posY += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = false;
	}



	if (INPUT->GetKeyDown(VK_SPACE) && !isDash)
	{
		speed = 20;
		isDash = true;
	}
	if (speed > 0)
	{
		speed--;
	}
	if (speed == 0) isDash = false;
	if (isLeft && checkDirectionX() && isDash) posX -= speed;
	if (!isLeft && checkDirectionX() && isDash)posX += speed;
	if (isUp && !checkDirectionX() && isDash)posY -= speed;
	if (!isUp && !checkDirectionX() && isDash)posY += speed;

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
