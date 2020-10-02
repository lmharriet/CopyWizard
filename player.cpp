#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{

	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small.bmp", 1000, 2400, 10, 24);
	IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
	posX = WINSIZEX / 2;
	posY = WINSIZEY / 2;
	rc = RectMakeCenter(posX, posY, 100, 100);


	//enum �ʱ�ȭ
	state = STATE::IDLE;
	move = MOVE::DOWN;

	memset(tileCheck, 0, sizeof(tileCheck));
	memset(diagonalCheck, 0, sizeof(diagonalCheck));

	//colision detection Rect
	makeCol((int)DIRECTION::TOP, 0, -55);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);

	//3��..
	makeCol2(0, -45, -55);
	makeCol2(1, 45, -55);
	makeCol2(2, -45, 60);
	makeCol2(3, 45, 60);

	//dash, direction
	dashLeft = dashRight = dashUp = dashDown = false;
	isLeft = isRight = isUp = isDown = false;
	//attack type
	basic = standard = signature = false;

	speed = index = dashIndex = count = dashCount = stateCool = 0;

	//�ҷ� Ŭ����
	blaze = new bomb;
	blaze->init(100, 200);
	
	flares = new homingFlares;
	flares->init(100);
	//angle between mouse & player
	attackAngle = saveAngle = 0;
	angleTenth = 0;

	return S_OK;
}

void player::release()
{
	blaze->release();
	SAFE_DELETE(blaze);
}

void player::update()
{
	blaze->update();
	flares->update();
	//animation count
	count++;
	dashCount++;

	// angle(mouse-player), angleTenth
	attackAngle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));
	angleTenth = (int)(saveAngle * (18 / PI));


	if (speed == 0 && stateCool == 0) controller();

	tileCol();

	makeCol((int)DIRECTION::TOP, 0, -60);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -25, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -25, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);

	makeCol2(0, -45, -55);
	makeCol2(1, 45, -55);
	makeCol2(2, -45, 60);
	makeCol2(3, 45, 60);


	dashFunction();
	changeState();
	blazeSetUp();
	standardSetUp();




	// camera�� ���󰡴� ���
	CAMERAMANAGER->MovePivot(posX, posY);

	//don't touch!
	buttonDown();
}

void player::render()
{
	image* img = IMAGEMANAGER->findImage("PlayerAttackCircle");
	CAMERAMANAGER->AlphaFrameRender(getMemDC(), img, posX - 50, posY - 20, angleTenth, 0, 50);

	//CAMERAMANAGER->Rectangle(getMemDC(), rc);
	//Rectangle(getMemDC(), rc);

	animation();
	viewText();

	blaze->render();
	flares->render();
}

void player::controller()
{
	//����
	if (isLeft)
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol)posX -= 8;
	}
	//������
	if (isRight)
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol) posX += 8;
	}
	//��
	if (isUp)
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol) posY -= 8;
	}
	//�Ʒ�
	if (isDown)
	{
		rc = RectMakeCenter(posX, posY, 100, 100);
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol) posY += 8;
	}

	//DASH
	if (INPUT->GetKeyDown(VK_SPACE) && speed == 0)
	{
		//�뽬 ����Ʈ ����
		EFFECT->dashEffect(move, { (long)posX,(long)posY });
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
	// ���� ������ �뽬 �Ұ���, ���� ������ �뽬�ؼ� ���������� ������ ������. 
	// ������ ����ó�� �ʿ�

	if (speed == 0)return;

	if (dashLeft)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_TOP].isCol && 
				!diagonalCheck[0].isCol)
			{
				posX -= speed;
				posY -= speed;
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_DOWN].isCol &&
				!diagonalCheck[2].isCol)
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
			if (!tileCheck[(int)DIRECTION::RIGHT_TOP].isCol &&
				!diagonalCheck[1].isCol)
			{
				posX += speed;
				posY -= speed;
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol &&
				!diagonalCheck[3].isCol )
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

void player::blazeSetUp()
{
	if (stateCool == 0 && basic)
	{
		if (basic)
		{
			//basic ���� �� �� �ޱ��� ����
			saveAngle = attackAngle;
		}
		stateCool = 15;
		blaze->fire(posX - 30, posY, 10, attackAngle, 30);
	}

	if (stateCool > 0)
	{
		stateCool--;
		state = STATE::BASIC;
		// ����� �ޱ� �������� �����̱�
		posX += cosf(saveAngle);
		posY += -sinf(saveAngle);
	}

}

void player::standardSetUp()
{
	if (standard)
	{
		flares->fire(posX, posY);
	}
}

void player::animation()
{
	switch (state)
	{
	case STATE::IDLE:

		if (move == MOVE::LEFT)frameAnimation(3, 0);
		else if (move == MOVE::RIGHT) frameAnimation(2, 0);
		else if (move == MOVE::UP) frameAnimation(1, 0);
		else frameAnimation(0, 0);

		break;
	case STATE::RUN:
		if (move == MOVE::LEFT || move == MOVE::LEFT_TOP || move == MOVE::LEFT_DOWN)
		{
			if (count % 6 == 0)
			{
				index--;
				if (index < 0)index = 4;
			}
			frameAnimation(index, 4);
		}
		else if (move == MOVE::RIGHT || move == MOVE::RIGHT_TOP || move == MOVE::RIGHT_DOWN)
		{
			if (count % 6 == 0)
			{
				index++;
				if (index > 4) index = 0;
			}
			frameAnimation(index, 3);
		}
		else if (move == MOVE::UP)
		{
			if (count % 5 == 0)
			{
				index++;
				if (index > 9)index = 0;
			}
			frameAnimation(index, 2);
		}
		else if (move == MOVE::DOWN)
		{
			if (count % 5 == 0)
			{
				index++;
				if (index > 9)index = 0;
			}
			frameAnimation(index, 1);
		}
		break;
	case STATE::DASH:
		if (dashLeft)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex--;
				if (dashIndex < 0 || speed == 0)dashIndex = 7;
			}
			frameAnimation(dashIndex, 8);
		}
		else if (dashRight)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 7 || speed == 0)dashIndex = 0;
			}
			frameAnimation(dashIndex, 7);
		}
		else if (dashUp)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 7 || speed == 0)dashIndex = 0;
			}
			frameAnimation(dashIndex, 2);
		}
		else if (dashDown)
		{
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 7 || speed == 0)dashIndex = 0;
			}
			frameAnimation(dashIndex, 1);
		}
		break;

	case STATE::BASIC:
	{
		if (angleTenth > 14 && angleTenth <= 23)//left
		{
			if (count % 3 == 0)
			{
				index++;
				if (index > 7 || stateCool == 0) index = 0;
			}
			frameAnimation(index, 5);
			//���� ���� ������ ���� ���ϱ�
			if (stateCool == 0)
				move = MOVE::LEFT;
		}
		else if (angleTenth <= 4 || angleTenth > 32) //right
		{
			if (count % 3 == 0)
			{
				index++;
				if (index > 7 || stateCool == 0) index = 0;
			}
			frameAnimation(index, 6);

			// ������ ���� ������ ������ ���ϱ�
			if (stateCool == 0)
				move = MOVE::RIGHT;
		}
		else if (angleTenth > 4 && angleTenth <= 14) //up
		{
			if (count % 3 == 0)
			{
				index++;
				if (index > 7)index = 0;
			}
			frameAnimation(index, 14);
			//���� ���� ������ ���� ���ϱ�
			if (stateCool == 0)
				move = MOVE::UP;
		}
		else if (angleTenth > 23 && angleTenth <= 32) //down
		{
			if (count % 3 == 0)
			{
				index++;
				if (index > 7)index = 0;
			}
			frameAnimation(index, 6);
			//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
			if (stateCool == 0)
				move = MOVE::DOWN;
		}
		break;
	case STATE::STANDARD:
		break;
	case STATE::SIGNATURE:
		break;
	}

	}
}

void player::frameAnimation(int frameX, int frameY)
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, frameX, frameY);
}

void player::tileCol()
{
	for (int i = 0; i < 8; i++)
	{
		tileCheck[i].isCol = false;
		if (i < 4) diagonalCheck[i].isCol = false;
	}

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].keyName != "" && tile[i].kind != TERRAIN::WALL) continue;
		for (int j = 0; j < 8; j++)
		{
			if (colCheck(tileCheck[j].rc, tile[i].rc))
			{
				tileCheck[j].isCol = true;
			}

			if (j < 4 && colCheck(diagonalCheck[j].rc,tile[i].rc) && state == STATE::DASH)
			{
				diagonalCheck[j].isCol = true;
			}
		}
	}
}

void player::makeCol(int index, int destX, int destY, int rcSize)
{
	tileCheck[index].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2) + destX, CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) + destY, rcSize, rcSize);
}

void player::makeCol2(int index, int destX, int destY, int rcSize)
{
	diagonalCheck[index].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2) + destX, CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) + destY, rcSize, rcSize);
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
		else // �׳� ���� LEFT
		{
			move = MOVE::RIGHT;
		}
	}

	else if (isUp) // �׳� ���� UP
	{
		state = STATE::RUN;
		move = MOVE::UP;
	}

	else if (isDown) // �׳� ���� DOWN
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
	//key
	if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey('A'))isLeft = true;
	else isLeft = false;
	if (INPUT->GetKey(VK_RIGHT) || INPUT->GetKey('D'))isRight = true;
	else isRight = false;
	if (INPUT->GetKey(VK_UP) || INPUT->GetKey('W'))isUp = true;
	else isUp = false;
	if (INPUT->GetKey(VK_DOWN) || INPUT->GetKey('S'))isDown = true;
	else isDown = false;

	//Attack
	if (INPUT->GetKeyDown(VK_LBUTTON)) basic = true;
	else basic = false;
	if (INPUT->GetKeyDown(MK_RBUTTON)) standard = true;
	else standard = false;
	if (INPUT->GetKey('Q')) signature = true;
	else signature = false;

}

//del
void player::viewText()
{

	char str[126];
	//for (int i = 0; i < 8; i++)
	//{
	//	if (i < 4)
	//	{
	//		CAMERAMANAGER->Rectangle(getMemDC(), diagonalCheck[i].rc);
	//	}

	//	CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i].rc);
	//}

	wsprintf(str, "player move : %d", move);
	textOut(getMemDC(), 10, 200, str, WHITE);

	wsprintf(str, "player basic  : %d", dashIndex);
	textOut(getMemDC(), 10, 220, str, WHITE);

	wsprintf(str, "player state: %d \n basic :3", state);
	textOut(getMemDC(), 10, 250, str, WHITE);
	//int x = _ptMouse.x;
	//int y = _ptMouse.y;

	//char text[126];

	//wsprintf(text, "mouse.x : %d ,= mouse.y : %d", CAMERAMANAGER->GetAbsoluteX(x), CAMERAMANAGER->GetAbsoluteY(y));
	//textOut(getMemDC(), _ptMouse.x, _ptMouse.y, text, WHITE);
}