#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small.bmp", 1000, 2400, 10, 24);
	posX = WINSIZEX / 2;
	posY = WINSIZEY / 2;
	rc = RectMakeCenter(posX, posY, 100, 100);

	speed = index = dashIndex = count = dashCount= 0;
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
	dashCount++;
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


	// camera�� ���󰡴� ���
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

	wsprintf(str, "player move : %d", move);
	textOut(getMemDC(), 10, 120, str, WHITE);

	wsprintf(str, "player index : %d", dashIndex);
	textOut(getMemDC(), 10, 100, str, WHITE);

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
	if (isLeft || INPUT->GetKey('A'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol)posX -= 8;
	}

	if (isRight || INPUT->GetKey('D'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol) posX += 8;
	}
	if (isUp || INPUT->GetKey('W'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol) posY -= 8;
	}

	if (isDown || INPUT->GetKey('S'))
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol) posY += 8;
	}


	if (INPUT->GetKeyDown(VK_SPACE) && speed == 0)
	{
		state = STATE::DASH;

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
	// enum + switch ����ؼ� dash �����ϱ�
	// ���� ������ �뽬 �Ұ���, ���� ������ �뽬�ؼ� ���������� ������ ������. 
	// ������ ����ó�� �ʿ�
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
		else // �׳� ���� LEFT
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
		else // �׳� ���� RIGHT
		{
			if (!tileCheck[(int)DIRECTION::RIGHT].isCol)
				posX += speed;
		}
	}

	else if (dashUp)
	{
		//���� UP
		if (!tileCheck[(int)DIRECTION::TOP].isCol)
			posY -= speed;
	}

	else if (dashDown)
	{
		//���� DOWN
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

		if (move == MOVE::LEFT) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 3, 0);
		else if (move == MOVE::RIGHT) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 2, 0);
		else if (move == MOVE::UP) CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 1, 0);
		else CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, 0, 0);

		break;
	case STATE::RUN:
		if (move == MOVE::LEFT)
		{
			if (count % 5 == 0)
			{
				index--;
				if (index < 0)index = 9;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 4);
		}
		else if (move == MOVE::RIGHT)
		{
			if (count % 5 == 0)
			{
				index++;
				if (index > 9) index = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 3);
		}
		else if (move == MOVE::UP ||move==MOVE::LEFT_TOP ||move == MOVE::RIGHT_TOP)
		{
			if (count % 5 == 0)
			{
				index++;
				if (index > 9)index = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 2);
		}
		else if(move ==MOVE::DOWN ||move == MOVE::LEFT_DOWN ||move == MOVE::RIGHT_DOWN)
		{
			if (count % 5 == 0)
			{
				index++;
				if (index > 9)index = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, index, 1);
		}
		break;
	case STATE::DASH:
		if (dashLeft)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex--;
				if (dashIndex < 0 ||speed ==0)dashIndex = 7;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, dashIndex, 8);
		}
		else if (dashRight)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex >7 || speed == 0)dashIndex = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, dashIndex, 7);
		}
		else if (dashUp)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 7 || speed == 0)dashIndex = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, dashIndex, 2);
		}
		else if (dashDown)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 7 || speed == 0)dashIndex = 0;
			}
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, dashIndex, 1);
		}
		break;
	default:
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
		state = STATE::RUN;
		if (isUp)
		{
			move = MOVE::LEFT_TOP;
		}
		else if (isDown)
		{
			move = MOVE::LEFT_DOWN;
		}
		else // �׳� ���� LEFT
		{
			move = MOVE::LEFT;
		}
	}

	else if (isRight)
	{
		state = STATE::RUN;
		if (isUp)
		{
			move = MOVE::RIGHT_TOP;
		}
		else if (isDown)
		{
			move = MOVE::RIGHT_DOWN;
		}
		else // �׳� ������ LEFT
		{
			move = MOVE::RIGHT;
		}
	}

	else if (isUp) // �׳� ������ UP
	{
		state = STATE::RUN;
		move = MOVE::UP;
	}

	else if (isDown) // �׳� ������ DOWN
	{
		state = STATE::RUN;
		move = MOVE::DOWN;
	}
	else
	{
		state = STATE::IDLE;
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