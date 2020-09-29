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
	state = STATE::IDLE;
	move = MOVE::DOWN;


	makeCol((int)DIRECTION::TOP, 0, -55);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);


	memset(tileCheck, 0, sizeof(tileCheck));

	dashLeft = dashRight = dashUp = dashDown = false;
	isLeft = isRight = isUp = isDown = false;
	return S_OK;
}

void player::release()
{

}

void player::update()
{
	count++;

	dashFunction();

	if (speed == 0) controller();

	tileCol();

	makeCol((int)DIRECTION::TOP, 0, -60);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);


	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);

	changeState();
	//don't touch!
	buttonDown();
}


void player::render()
{
	animation();


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

	wsprintf(str, "player state : %d", move);
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
	//왼쪽
	if (isLeft || INPUT->GetKey('A'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol)posX -= 6;
	}

	if (isRight || INPUT->GetKey('D'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol) posX += 6;
	}
	if (isUp || INPUT->GetKey('W'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol) posY -= 6;
	}

	if (isDown || INPUT->GetKey('S'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol) posY += 6;
	}


	if (INPUT->GetKeyDown(VK_SPACE) && speed == 0)
	{
		switch (move)
		{
		case MOVE::LEFT:
			dashLeft = true;
			break;
		case MOVE::LEFT_TOP:
			dashLeft = true;
			dashUp = true;
			break;
		case MOVE::RIGHT:
			dashRight = true;
			break;
		case MOVE::RIGHT_TOP:
			dashRight = true;
			dashUp = true;
			break;
		case MOVE::UP:
			dashUp = true;
			break;
		case MOVE::DOWN:
			dashDown = true;
			break;
		case MOVE::LEFT_DOWN:
			dashLeft = true;
			dashDown = true;
			break;
		case MOVE::RIGHT_DOWN:
			dashRight = true;
			dashDown = true;
			break;
		default:
			break;
		}
		speed = 20;
	}
}

void player::dashFunction()
{
	if (speed == 0)return;
	// enum + switch 사용해서 dash 구현하기
	// 벽에 닿으면 대쉬 불가능, 게임 내에서 대쉬해서 낭떠러지에 닿으면 떨어짐. 
	// 벽끼임 예외처리 필요

	/*switch (pState)
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
	}*/

	if (dashLeft)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_TOP].isCol)
			{
				posX -= speed;
				posY -= speed;
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_DOWN].isCol)
			{
				posX -= speed;
				posY += speed;
			}
		}
		else // 그냥 순수 LEFT
		{
			if (!tileCheck[(int)DIRECTION::LEFT].isCol)
				posX -= speed;
		}
	}

	else if (dashRight)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::RIGHT_TOP].isCol)
			{
				posX += speed;
				posY -= speed;
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol)
			{
				posX += speed;
				posY += speed;
			}
		}
		else // 그냥 순수 RIGHT
		{
			if (!tileCheck[(int)DIRECTION::RIGHT].isCol)
				posX += speed;
		}
	}

	else if (dashUp)
	{
		//순수 UP
		if (!tileCheck[(int)DIRECTION::TOP].isCol)
			posY -= speed;
	}

	else if (dashDown)
	{
		//순수 DOWN
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol)
			posY += speed;
	}

	speed--;
	if (speed == 0) resetKey();
}

void player::animation()
{

	switch (state)
	{
	case STATE::IDLE:

		if (move ==MOVE::LEFT) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 3, 0);
		else if (move==MOVE::RIGHT) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 2, 0);
		else if (move ==MOVE::UP) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 1, 0);
		else CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);
		
		break;
	case STATE::RUN:
		break;
	case STATE::DASH:
		break;
	default:
		break;
	}


	//switch (move)
	//{
	//	//case STATE::IDLE:
	//	//	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);
	//	//	break;
	//case MOVE::LEFT:
	//	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 3, 0);
	//	break;
	//case MOVE::RIGHT:
	//	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 2, 0);
	//	break;
	//case MOVE::UP:
	//	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 1, 0);
	//	break;
	//case MOVE::DOWN:
	//	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);
	//	break;
	//	//case STATE::RUN:
	//	//	if (isLeft)
	//	//	{
	//	//		if (count % 5 == 0)
	//	//		{
	//	//			index--;
	//	//			if (index < 0) index = 9;
	//	//		}
	//	//		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 4);
	//	//	}
	//	//	else
	//	//	{
	//	//		if (count % 5 == 0)
	//	//		{
	//	//			index++;
	//	//			if (index > 9) index = 0;
	//	//		}
	//	//		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 3);
	//	//	}
	//	//	break;
	//default:
	//	break;
	//}
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

void player::resetKey()
{
	dashLeft = false;
	dashRight = false;
	dashUp = false;
	dashDown = false;
}

void player::changeState()
{
	if (speed > 0)return;

	if (isLeft)
	{
		if (isUp)
		{
			move = MOVE::LEFT_TOP;
		}
		else if (isDown)
		{
			move = MOVE::LEFT_DOWN;
		}
		else // 그냥 순수히 LEFT
		{
			move = MOVE::LEFT;
		}
	}

	else if (isRight)
	{
		if (isUp)
		{
			move = MOVE::RIGHT_TOP;
		}
		else if (isDown)
		{
			move = MOVE::RIGHT_DOWN;
		}
		else // 그냥 순수히 LEFT
		{
			move = MOVE::RIGHT;
		}
	}

	else if (isUp) // 그냥 순수히 UP
	{
		move = MOVE::UP;
	}

	else if (isDown) // 그냥 순수히 DOWN
	{
		move = MOVE::DOWN;
	}
}

void player::buttonDown()
{
	if (INPUT->GetKey(VK_LEFT))isLeft = true;
	else isLeft = false;
	if (INPUT->GetKey(VK_RIGHT))isRight = true;
	else isRight = false;
	if (INPUT->GetKey(VK_UP))isUp = true;
	else isUp = false;
	if (INPUT->GetKey(VK_DOWN))isDown = true;
	else isDown = false;
}