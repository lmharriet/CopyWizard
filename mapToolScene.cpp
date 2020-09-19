#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu.bmp", 360, 720);
	for (int i = 0; i < 3; i++)
	{
		mapOption[i] = RectMake(1020, 197 + (i * 83), 160, 60);
	}
	SAVE = RectMake(955, 563, 100, 45);
	LOAD = RectMake(955, 613, 100, 45);
	BACK = RectMake(955, 663, 100, 45);

	option = OPTION::MENU;
	isLeftDown = isLeft = isLeftUp = false;
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
	vkCheck();

	//option이 MENU 일 때
	
	switch (option)
	{
	case OPTION::MENU:
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&mapOption[i], _ptMouse) && isLeftDown)
			{
				// 0 :  wall  //
				// 1 :  tile  //
				// 2 : object //
				option = (OPTION)i;
			}
		}
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("시작화면");
		break;
	case OPTION::WALL:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) option = OPTION::MENU;
		break;
	case OPTION::TILE:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) option = OPTION::MENU;
		break;
	case OPTION::OBJECT:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) option = OPTION::MENU;
		break;

	}
}

void mapToolScene::render()
{
	switch (option)
	{
	case OPTION::WALL:
		break;
	case OPTION::TILE:
		imageRender("tileMenu", { 920,0 });
		break;
	case OPTION::OBJECT:
		break;
	case OPTION::MENU:

		imageRender("mapMenu", { 920,0 });
		break;
	}

	if (INPUT->GetToggleKey(VK_TAB))
	{
		buttonRender();

		if (option == OPTION::MENU)
		{
			for (int i = 0; i < 3; i++) Rectangle(getMemDC(), mapOption[i]);

		}
	}

	textRender();
}

void mapToolScene::imageRender(string keyName, POINT pt)
{
	IMAGEMANAGER->findImage(keyName)->render(getMemDC(), pt.x, pt.y);
}

void mapToolScene::textRender()
{
	ptOut(getMemDC(), { 10,30 }, _ptMouse);
}

void mapToolScene::buttonRender()
{
	Rectangle(getMemDC(), SAVE);
	Rectangle(getMemDC(), LOAD);
	Rectangle(getMemDC(), BACK);
}

void mapToolScene::vkCheck()
{
	if (INPUT->GetKeyDown(VK_LBUTTON))isLeftDown = true;
	else isLeftDown = false;
	if (INPUT->GetKey(VK_LBUTTON)) isLeft = true;
	else isLeft = false;
	if (INPUT->GetKeyUp(VK_LBUTTON))isLeftUp = true;
	else isLeftUp = false;
}