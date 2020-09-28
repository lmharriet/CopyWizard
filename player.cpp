#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("playerFrame", "wizard/player.bmp", 1700, 4080, 10, 24);
	posX = WINSIZEX / 2;
	posY = WINSIZEY / 2;
	rc = RectMakeCenter(posX, posY, 170, 170);
	speed = 0;
	isLeft = false;
	isUp = false;
	state = IDLE;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (INPUT->GetKey(VK_LEFT))
	{
		posX -= 5;
		rc = RectMakeCenter(posX, posY, 170, 170);
		isLeft = true;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		posX += 5;
		rc = RectMakeCenter(posX, posY, 170, 170);
		isLeft = false;
	}
	if (INPUT->GetKey(VK_UP))
	{
		posY -= 5;
		rc = RectMakeCenter(posX, posY, 170, 170);
		isUp = true;
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		posY += 5;
		rc = RectMakeCenter(posX, posY, 170, 170);
		isUp = false;
	}

	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(posX, posY);
}

void player::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), rc.left, rc.top, 0, 0);
}