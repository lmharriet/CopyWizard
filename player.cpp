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


	pState = STATE::IDLE;


	//for (int i = 0; i < 3; i++)
	//{
	//	tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30), 7, 7);
	//	tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30), 7, 7);
	//}
	//tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30), 7, 7);
	//tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 50), 7, 7);
	makeCol((int)DIRECTION::TOP, 0, -30);
	makeCol((int)DIRECTION::BOTTOM, 0, +30);
	makeCol((int)DIRECTION::LEFT, -30, 0);
	makeCol((int)DIRECTION::RIGHT, +30, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -15, -15);
	makeCol((int)DIRECTION::RIGHT_TOP, 15, -15);
	makeCol((int)DIRECTION::LEFT_DOWN, -15, 15);
	makeCol((int)DIRECTION::RIGHT_DOWN, 15, 15);


	memset(tileCheck, 0, sizeof(tileCheck));

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	controller();
	dashFunction();
	tileCol();

	makeCol((int)DIRECTION::TOP, 0, -50);
	makeCol((int)DIRECTION::BOTTOM, 0, 50);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 30);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 30);

	/*for (int i = 0; i < 3; i++)
	{
		tileCheck[i].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 -30 + i * 30), 10, 10);
		tileCheck[i + 3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30 + i * 30), 10, 10);
	}
	tileCheck[6].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 30), 10, 10);
	tileCheck[7].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 50), 10, 10);*/

	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}


void player::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);

	char str[126];
	for (int i = 0; i < 8; i++)
	{

		CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i].rc);
		//CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(tileCheck[i].pos.x, tileCheck[i].pos.y, 7, 7));
		//Rectangle(getMemDC(), tileCheck[i].rc);
		/*wsprintf(str, "check %d : %d", i, tileCheck[i].isCol);
		TextOut(getMemDC(), 10, 120 + (i * 20), str, strlen(str));

		wsprintf(str, " tilecheck pos[%d] : %d", i, tileCheck[i].pos.x);
		TextOut(getMemDC(), 10, 300 + (i * 20), str, strlen(str));*/
	}

	wsprintf(str, "player state : %d", pState);
	textOut(getMemDC(), 10, 120, str, WHITE);

	wsprintf(str, "speed : %d", speed);
	textOut(getMemDC(), 10, 150, str, WHITE);
	int x = _ptMouse.x;
	int y = _ptMouse.y;

	char text[126];

	wsprintf(text, "mouse.x : %d ,= mouse.y : %d", CAMERAMANAGER->GetAbsoluteX(x), CAMERAMANAGER->GetAbsoluteY(y));
	textOut(getMemDC(), _ptMouse.x, _ptMouse.y, text, WHITE);

}

void player::controller()
{
	if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey('A'))
	{
		if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol)posX -= 6;

		isLeft = true;
		rc = RectMakeCenter(posX, posY, 100, 100);

	}
	if (INPUT->GetKey(VK_RIGHT) || INPUT->GetKey('D'))
	{
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol) posX += 6;

		isLeft = false;
		rc = RectMakeCenter(posX, posY, 100, 100);
	}

	if (INPUT->GetKey(VK_UP) || INPUT->GetKey('W'))
	{
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol) posY -= 6;

		isUp = true;
		rc = RectMakeCenter(posX, posY, 100, 100);
	}
		cout << isUp << '\n';

	if (INPUT->GetKey(VK_DOWN) || INPUT->GetKey('S'))
	{
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol) posY += 6;

		isUp = false;
		rc = RectMakeCenter(posX, posY, 100, 100);
	}

	//dash

	if (INPUT->GetKeyDown(VK_SPACE))
	{
		speed = 20;
		if (INPUT->GetKey(VK_LEFT)) pState = STATE::DASH_LEFT;
		if (INPUT->GetKey(VK_RIGHT)) pState = STATE::DASH_RIGHT;
		if (INPUT->GetKey(VK_UP))pState = STATE::DASH_UP;
		if (INPUT->GetKey(VK_DOWN)) pState = STATE::DASH_DOWN;
	}


}
void player::dashFunction()
{
	// enum + switch 사용해서 dash 구현하기

	switch (pState)
	{
	case STATE::DASH_LEFT:
		if (speed > 0) speed--;
		posX -= speed;
		break;
	case STATE::DASH_RIGHT:
		if (speed > 0)speed--;
		posX += speed;
		break;
	case STATE::DASH_UP:
		if (speed > 0) speed--;
		posY -= speed;
		break;
	case STATE::DASH_DOWN:
		if (speed > 0)speed--;
		posY += speed;
		break;
	}

	//if (INPUT->GetKeyDown(VK_SPACE) && !isDash)
	//{
	//	speed = 20;
	//	isDash = true;
	//}
	//if (speed > 0)
	//{
	//	speed--;
	//}
	//if (speed == 0) isDash = false;
	//if (isLeft && checkDirectionX() && isDash) posX -= speed;
	//if (!isLeft && checkDirectionX() && isDash)posX += speed;
	//if (isUp && !checkDirectionX() && isDash)posY -= speed;
	//if (!isUp && !checkDirectionX() && isDash)posY += speed;

}

void player::tileCol()
{

	for (int i = 0; i < 8; i++) tileCheck[i].isCol = false;

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].keyName != "") continue;
		for (int j = 0; j < 8; j++)
		{
			if (colCheck(tileCheck[j].rc, tile[i].rc))
			{
				tileCheck[j].isCol = true;
			}
		}
	}
}

void player::makeCol(int index, int destX, int destY, int rcSize)
{
	tileCheck[index].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2) + destX, CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) + destY, rcSize, rcSize);
}