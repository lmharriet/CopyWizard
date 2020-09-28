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
		//tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
		//tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
	}

	//tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) };
	tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2-20), 10, 10);
	//tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 70) };
	tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 70), 10, 10);

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
		//tileCheck[i].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
		//tileCheck[i + 3].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50),CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50) };
		tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 50), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 50 + i * 50), 10, 10);
	}

	//tileCheck[6].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) };
	tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 20), 10, 10);
	//tileCheck[7].pos = { CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 70) };
	tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 70), 10, 10);

	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}


void player::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);

	char str[126];
	for (int i = 0; i <8; i++)
	{

		CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i].rc);
		//CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(tileCheck[i].pos.x, tileCheck[i].pos.y, 7, 7));
		//Rectangle(getMemDC(), tileCheck[i].rc);
		wsprintf(str, "check %d : %d", i, tileCheck[i].isCol);
		TextOut(getMemDC(), 10, 120 + (i * 20), str, strlen(str));

		wsprintf(str, " tilecheck pos[%d] : %d", i, tileCheck[i].pos.x);
		TextOut(getMemDC(), 10, 300 + (i * 20), str, strlen(str));
	}

	int x = _ptMouse.x;
	int y = _ptMouse.y;

	char text[126];

	wsprintf(text, "mouse.x : %d ,= mouse.y : %d", CAMERAMANAGER->GetAbsoluteX(x), CAMERAMANAGER->GetAbsoluteY(y));
	textOut(getMemDC(), _ptMouse.x, _ptMouse.y, text, WHITE);

}

void player::controller()
{
	if (INPUT->GetKey(VK_LEFT) )
	{
		posX -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = true;

		//tileCol();
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		posX += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = false;

		//tileCol();
		posX += 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isLeft = false;
	}
	if (INPUT->GetKey(VK_UP))
	{
		//tileCol();
		posY -= 10;
		rc = RectMakeCenter(posX, posY, 100, 100);
		isUp = true;
	}
	if (INPUT->GetKey(VK_DOWN))
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
	
	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].keyName != "") continue;

		for (int j = 0; j < 8; j++)
		{
			if (colCheck(tileCheck[j].rc, tile[i].rc))
			{
				tileCheck[j].isCol = true;
			}
			/*if (PtInRect(&tile[i].rc, tileCheck[j].pos))
			{
				tileCheck[j].isCol = true;
				cout << i << "번째 tile"<<'\n';
				cout << j <<"번째 tile check" <<'\n';
			}*/


		}
	}
}
