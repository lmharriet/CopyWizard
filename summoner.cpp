#include "stdafx.h"
#include "summoner.h"



void summoner::addInit()
{
	time = 0;
	isDetect = false;
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());





}

void summoner::render()
{
	CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(pos.x, pos.y, 100, 100));

	float x = CAMERAMANAGER->GetRelativeX(pos.x);
	float y = CAMERAMANAGER->GetRelativeY(pos.y);
	img->frameRender(getMemDC(), x - img->getFrameWidth()/2, y - img->getFrameHeight()/2, frameIndex.x, frameIndex.y);
	
}

void summoner::update()
{
	if(isDetect) time++;

	skill();
}

void summoner::skill()
{
	if (time > 50)
	{
		cout << "스킬 발사" << '\n';
		isATK = true;

		//초기화
		time = 0;
	}
}


