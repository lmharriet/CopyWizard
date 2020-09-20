#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	initTile();

	for (int i = 0; i < 3; i++)
	{
		mapOption[i] = RectMake(1020, 197 + (i * 83), 160, 60);
	}
	SAVE = RectMake(955, 563, 100, 45);
	LOAD = RectMake(955, 613, 100, 45);
	BACK = RectMake(955, 663, 100, 45);

	for (int i = 0; i < 4; i++)
	{
		icon[i].rc = RectMake(928, 138 + (i * 50), 50, 48);
		icon[i].isCol = false;
	}
	drag.rc = RectMake(928, 87, 50, 48);
	drag.isCol = false;

	option = OPTION::SELECT_MENU;

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

	//SELECT MENU에는 아이콘(펜, 지우개 등등..)이 없습니당
	if (option != OPTION::SELECT_MENU) iconCheck();



}

void mapToolScene::render()
{
	//tile
	for (int i = 0; i < MAXTILE; i++) FrameRect(getMemDC(), tile[i].rc, DARKGREEN);

	//옵션의 상태에 따른 UI이미지 변경
	UIRender();

	rcRender();

	//메뉴에는 옵션버튼이 없다
	if (option != OPTION::SELECT_MENU)
	{
		for (int i = 0; i < 4; i++)
		{
			if (icon[i].isCol) imageRender("checkIcon", { icon[i].rc.left,icon[i].rc.top });
		}
		if (drag.isCol) imageRender("checkIcon", { drag.rc.left,drag.rc.top });
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
	case OPTION::SELECT_MENU:
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&mapOption[i], _ptMouse) && isLeftDown)
			{
				option = (OPTION)i;
			}
		}
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("시작화면");
		break;
	case OPTION::WALL_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::SELECT_MENU;
			//다시 들어왔을 때 icon이 미리 활성화 되어있음을 방지
			for (int i = 0; i < 4; i++) icon[i].isCol = false;
			drag.isCol = false;
		}
		break;
	case OPTION::TILE_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::SELECT_MENU;
			for (int i = 0; i < 4; i++) icon[i].isCol = false;
			drag.isCol = false;
		}
		break;
	case OPTION::OBJECT_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::SELECT_MENU;
			for (int i = 0; i < 4; i++) icon[i].isCol = false;
			drag.isCol = false;
		}
		break;
	}
}

void mapToolScene::iconCheck()
{
	///option != menu///

	//여기는 아이콘 입력만 처리

	switch (option)
	{
	case OPTION::WALL_MENU:
		break;
	case OPTION::TILE_MENU:
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i].rc, _ptMouse) && isLeftDown)
			{
				for (int j = 0; j < 4; j++)icon[j].isCol = false;

				icon[i].isCol = true;
			}
		}
		if (PtInRect(&drag.rc, _ptMouse) && isLeftDown)
		{
			if (!drag.isCol) drag.isCol = true;
			else drag.isCol = false;
		}
		break;
	case OPTION::OBJECT_MENU:
		break;
	}
}

void mapToolScene::initTile()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * TILESIZE), (i / MAXTILE_HEIGHT) * TILESIZE, TILESIZE, TILESIZE);
	}
}

/// IMAGE ///
void mapToolScene::addImage()
{
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("wallMenu", "maptool/ui/wallmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu", "maptool/ui/objectmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("checkIcon", "maptool/ui/check.bmp", 36, 36, true, RGB(255, 0, 255));
}
/// RENDER ///
void mapToolScene::UIRender()
{
	switch (option)
	{
	case OPTION::WALL_MENU:
		imageRender("wallMenu", { 920,0 });
		break;
	case OPTION::TILE_MENU:
		imageRender("tileMenu", { 920,0 });
		break;
	case OPTION::OBJECT_MENU:
		imageRender("objectMenu", { 920,0 });
		break;
	case OPTION::SELECT_MENU:
		imageRender("mapMenu", { 920,0 });
		break;
	}
}

void mapToolScene::textRender()
{
	ptOut(getMemDC(), { 10,30 }, _ptMouse, WHITE);
}

void mapToolScene::buttonRender()
{
	Rectangle(getMemDC(), SAVE);
	Rectangle(getMemDC(), LOAD);
	Rectangle(getMemDC(), BACK);
}

void mapToolScene::rcRender()
{
	//button
	if (INPUT->GetToggleKey(VK_TAB)) // 렉트를 껏다 켯다 할 수 있음
	{
		buttonRender();

		switch (option)
		{
		case OPTION::SELECT_MENU:
			for (int i = 0; i < 3; i++) Rectangle(getMemDC(), mapOption[i]);
			break;
		case OPTION::WALL_MENU:
			break;
		case OPTION::TILE_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i].rc);
			Rectangle(getMemDC(), drag.rc);
			break;
		case OPTION::OBJECT_MENU:
			break;
		}
	}
}

void mapToolScene::imageRender(string keyName, POINT pt)
{
	IMAGEMANAGER->findImage(keyName)->render(getMemDC(), pt.x, pt.y);
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