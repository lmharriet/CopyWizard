#include "stdafx.h"
#include "minion.h"

void summoner::animation()
{
}

void summoner::draw()
{
	CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(posX, posY, 100, 100));

	float x = CAMERAMANAGER->GetRelativeX(posX);
	float y = CAMERAMANAGER->GetRelativeY(posY);
	_image->frameRender(getMemDC(), x - _image->getFrameWidth()/2, y - _image->getFrameHeight()/2, _currentFrameX, _currentFrameY);
}

void summoner::move()
{
	if(isDetect) time++;

	skill();
}

void summoner::skill()
{
	if (time > 50)
	{
		cout << "스킬 발사" << '\n';
		isAttack = true;

		//초기화
		time = 0;
	}
}