#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("playerFrame", "wizard/player.bmp", 1700, 4080, 10, 24);
	x = WINSIZEX / 2;
	y = WINSIZEY / 2;
	rc = RectMakeCenter(x, y, 170, 170);
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
		x -= 5;
		rc = RectMakeCenter(x, y, 170, 170);
		isLeft = true;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		x += 5;
		rc= RectMakeCenter(x, y, 170, 170);
		isLeft = false;
	}
	if (INPUT->GetKey(VK_UP))
	{
		y -= 5;
		rc = RectMakeCenter(x, y, 170, 170);
		isUp = true;
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		y += 5;
		rc = RectMakeCenter(x, y, 170, 170);
		isUp = false;
	}




}

void player::render()
{
	IMAGEMANAGER->findImage("playerFrame")->frameRender(getMemDC(), rc.left, rc.top,0,0);

}


