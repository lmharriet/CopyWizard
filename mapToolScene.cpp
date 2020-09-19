#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	for (int i = 0; i < 3; i++)
	{
		mapOption[i] = RectMake(1020, 197 + (i * 83), 160, 60);
	}
	SAVE = RectMake(955, 563, 100, 45);
	LOAD = RectMake(955, 613, 100, 45);
	BACK = RectMake(955, 663, 100, 45);

	for (int i = 0; i < 5; i++)
	{
		icon[i].rc = RectMake(928, 88 + (i * 50), 50, 48);
		icon[i].isCol = false;
	}
	//option = OPTION::MENU;
	option = OPTION::TILE;
	isLeftDown = isLeft = isLeftUp = false;
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
	vkCheck();

	buttonCheck();

	iconCheck();
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

		switch (option)
		{
		case OPTION::MENU:
			for (int i = 0; i < 3; i++) Rectangle(getMemDC(), mapOption[i]);
			break;
		case OPTION::WALL:
			break;
		case OPTION::TILE:
			for (int i = 0; i < 5; i++) Rectangle(getMemDC(), icon[i].rc);
			break;
		case OPTION::OBJECT:
			break;
		}
	}

	if (option != OPTION::MENU)
	{
		for (int i = 0; i < 5; i++)
		{
			if (icon[i].isCol) imageRender("checkIcon", { icon[i].rc.left,icon[i].rc.top });
		}
	}
	textRender();
}

void mapToolScene::buttonCheck()
{
	//여기는 메뉴 이동, 세이브, 로드 만 처리

	// 0 :  wall  //
	// 1 :  tile  //
	// 2 : object //

	//option이 MENU 일 때

	/// button setting ///
	switch (option)
	{
	case OPTION::MENU:
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&mapOption[i], _ptMouse) && isLeftDown)
			{
				option = (OPTION)i;
			}
		}
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("시작화면");
		break;
	case OPTION::WALL:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::MENU;
			//다시 들어왔을 때 icon이 미리 활성화 되어있음을 방지
			for (int i = 0; i < 5; i++) icon[i].isCol = false;
		}
		break;
	case OPTION::TILE:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::MENU;
			for (int i = 0; i < 5; i++) icon[i].isCol = false;
		}
		break;
	case OPTION::OBJECT:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::MENU;
			for (int i = 0; i < 5; i++) icon[i].isCol = false;
		}
		break;
	}
}

void mapToolScene::iconCheck()
{
	///option != menu///

	//맵툴 아이콘을 누르면 기능 실행

	switch (option)
	{
	case OPTION::WALL:
		break;
	case OPTION::TILE:
		for (int i = 0; i < 5; i++)
		{
			if (PtInRect(&icon[i].rc, _ptMouse) && isLeftDown)
			{
				if (!icon[i].isCol) icon[i].isCol = true;
				else icon[i].isCol = false;
			}
		}
		break;
	case OPTION::OBJECT:
		break;
		//case OPTION::MENU:
		//	break;
	}
}

void mapToolScene::imageRender(string keyName, POINT pt)
{
	IMAGEMANAGER->findImage(keyName)->render(getMemDC(), pt.x, pt.y);
}

void mapToolScene::textRender()
{
	ptOut(getMemDC(), { 10,30 }, _ptMouse);
}

void mapToolScene::addImage()
{
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("checkIcon", "maptool/ui/check.bmp", 36, 36, true, RGB(255, 0, 255));
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