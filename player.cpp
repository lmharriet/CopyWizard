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


	makeCol((int)DIRECTION::TOP, 0, -55);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);


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

	makeCol((int)DIRECTION::TOP, 0, -60);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);


	// camera�� ���󰡴� ���
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
	//����
	if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey('A'))
	{
		isLeft = true;
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol)posX -= 6;
	}

	if (INPUT->GetKey(VK_RIGHT) || INPUT->GetKey('D'))
	{
		isLeft = false;
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol) posX += 6;
	}
	if (INPUT->GetKey(VK_UP) || INPUT->GetKey('W'))
	{
		isUp = true;
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol) posY -= 6;
	}

	if (INPUT->GetKey(VK_DOWN) || INPUT->GetKey('S'))
	{
		isUp = false;
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol) posY += 6;
	}

	//state == idle left,right
	if (INPUT->GetKeyUp(VK_LEFT) || INPUT->GetKeyUp(VK_RIGHT))
	{
		if (isLeft)pState = STATE::LEFT;
		if (!isLeft)pState = STATE::RIGHT;
	}
	//state == idle up, down

	if (INPUT->GetKeyUp(VK_UP) || INPUT->GetKeyUp(VK_DOWN))
	{
		if (isUp)pState = STATE::UP;
		if (!isUp)pState = STATE::DOWN;
	}

	// state == dash
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		speed = 20;
		if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey('A') || pState == STATE::LEFT) pState = STATE::DASH_LEFT;
		if (INPUT->GetKey(VK_RIGHT) || INPUT->GetKey('D') || pState == STATE::RIGHT) pState = STATE::DASH_RIGHT;
		if (INPUT->GetKey(VK_UP) || INPUT->GetKey('W') || pState == STATE::UP)	pState = STATE::DASH_UP;
		if (INPUT->GetKey(VK_DOWN) || INPUT->GetKey('S') || pState == STATE::DOWN) pState = STATE::DASH_DOWN;
	}
}
void player::dashFunction()
{
	// enum + switch ����ؼ� dash �����ϱ�
	// ���� ������ �뽬 �Ұ���, ���� ������ �뽬�ؼ� ���������� ������ ������. 
	// ������ ����ó�� �ʿ�

	switch (pState)
	{
	case STATE::DASH_LEFT:
		if (!tileCheck[(int)DIRECTION::LEFT].isCol &&
			!tileCheck[(int)DIRECTION::LEFT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol)
		{
			if (speed > 0) speed--;
			posX -= speed;
		}
		else speed = 0;

		break;
	case STATE::DASH_RIGHT:
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol &&
			!tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol)
		{
			if (speed > 0)speed--;
			posX += speed;
		}
		else speed = 0;
		break;
	case STATE::DASH_UP:
		if (!tileCheck[(int)DIRECTION::TOP].isCol
			&& !tileCheck[(int)DIRECTION::LEFT_TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol)
		{
			if (speed > 0) speed--;
			posY -= speed;
		}
		else speed = 0;
		break;
	case STATE::DASH_DOWN:
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol &&
			!tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol)
		{
			if (speed > 0)speed--;
			posY += speed;
		}
		else speed = 0;
		break;
	}
}

void player::tileCol()
{

	for (int i = 0; i < 8; i++) tileCheck[i].isCol = false;

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].keyName != "" && tile[i].kind != TERRAIN::WALL) continue;
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