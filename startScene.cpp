#include "stdafx.h"
#include "startScene.h"


HRESULT startScene::init()
{
	
	SOUNDMANAGER->play("titleBGM",true);
	SOUNDMANAGER->stop("mapToolBGM");
	
	
	//렉트 위치 초기화
	//_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 200, 200, 100);

	IMAGEMANAGER->addImage("startScreen", "wizard/titleScreen.bmp", 1280, 720);
	IMAGEMANAGER->addImage("start_off", "wizard/start_off.bmp", 125, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("start_on", "wizard/start_on.bmp", 125, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("maptool_off", "wizard/maptool_off.bmp", 180, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("maptool_on", "wizard/maptool_on.bmp", 180, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("exit_off", "wizard/exit_off.bmp", 90, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("exit_on", "wizard/exit_on.bmp", 90, 30, true, RGB(255, 0, 255));


	option[0] = RectMake(593, 455, 125, 30);
	option[1] = RectMake(565, 519, 180, 30);
	option[2] = RectMake(610, 583, 90, 30);

	x = y = 0;
	isCol = false;

	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
	x = _ptMouse.x;
	y = _ptMouse.y;

	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&option[i], _ptMouse) && INPUT->GetKeyDown(VK_LBUTTON))
		{
			isCol = true;


			switch (i)
			{
			case 0:
				SOUNDMANAGER->stop("titleBGM");
				SCENEMANAGER->loadScene("픽셀충돌");
				break;
			case 1:
				SOUNDMANAGER->stop("titleBGM");
				SOUNDMANAGER->play("mapToolBGM",true);
				SCENEMANAGER->loadScene("맵툴제작");
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
	}
}

void startScene::render()
{
	/*Rectangle(getMemDC(), _rc);
	TextOut(getMemDC(), _rc.left + 80, _rc.top + 50, "START", strlen("START"));*/

	IMAGEMANAGER->findImage("startScreen")->render(getMemDC(), 0, 0);

	char pos[126];
	wsprintf(pos, "x: %d, y: %d", x, y);
	textOut(getMemDC(), 10, 30, pos, RGB(255, 156, 0));

	wsprintf(pos, "isCol:%d ", isCol);
	textOut(getMemDC(), 10, 50, pos, RGB(255, 156, 0));

	//for (int i = 0; i < 3; i++)
	//{
	//	FrameRect(getMemDC(), option[i], RGB(0, 0, 0));
	//}
	IMAGEMANAGER->findImage("start_off")->render(getMemDC(), option[0].left, option[0].top);
	IMAGEMANAGER->findImage("maptool_off")->render(getMemDC(), option[1].left, option[1].top);
	IMAGEMANAGER->findImage("exit_off")->render(getMemDC(), option[2].left, option[2].top);
	if (PtInRect(&option[0], _ptMouse)) IMAGEMANAGER->findImage("start_on")->render(getMemDC(), option[0].left, option[0].top);
	if (PtInRect(&option[1], _ptMouse)) IMAGEMANAGER->findImage("maptool_on")->render(getMemDC(), option[1].left, option[1].top);
	if (PtInRect(&option[2], _ptMouse)) IMAGEMANAGER->findImage("exit_on")->render(getMemDC(), option[2].left, option[2].top);
}