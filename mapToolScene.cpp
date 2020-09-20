#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	initTile();
	initSelectTile();

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

	user.KeyName = "";
	user.kind = TERRAIN::NONE;


	option = OPTION::SELECT_MENU;

	isLeftDown = isLeft = isLeftUp = false;

	initCam();
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
	controller();

	vkCheck();

	buttonCheck();

	//SELECT MENU에는 아이콘(펜, 지우개 등등..)이 없습니당
	if (option != OPTION::SELECT_MENU) iconCheck();
}

void mapToolScene::render()
{
	//타일 그리기
	tileRender();

	//옵션의 상태에 따른 UI이미지 변경
	UIRender();

	rcRender();

	checkRender();


	//userRender
	if (user.kind != TERRAIN::NONE)
	{
		imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 }, 200);
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
			//유저가 가지고있는 데이터 초기화
			user.KeyName = "";
			user.kind = TERRAIN::NONE;

			option = OPTION::SELECT_MENU;
			//다시 들어왔을 때 icon이 미리 활성화 되어있음을 방지
			resetIcon();
			drag.isCol = false;
		}
		break;
	case OPTION::TILE_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;
			resetIcon();
			drag.isCol = false;
		}
		break;
	case OPTION::OBJECT_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;
			resetIcon();
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
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i].rc, _ptMouse) && isLeftDown)
			{
				resetIcon();
				icon[i].isCol = true;
			}
		}
		if (PtInRect(&drag.rc, _ptMouse) && isLeftDown)
		{
			if (!drag.isCol) drag.isCol = true;
			else drag.isCol = false;
		}
		break;
	case OPTION::TILE_MENU:
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i].rc, _ptMouse) && isLeftDown)
			{
				resetIcon();
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
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i].rc, _ptMouse) && isLeftDown)
			{
				resetIcon();
				icon[i].isCol = true;
			}
		}
		if (PtInRect(&drag.rc, _ptMouse) && isLeftDown)
		{
			if (!drag.isCol) drag.isCol = true;
			else drag.isCol = false;
		}
		break;
	}
}

void mapToolScene::resetIcon()
{
	for (int i = 0; i < 4; i++) icon[i].isCol = false;
}

void mapToolScene::initTile()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * TILESIZE), (i / MAXTILE_HEIGHT) * TILESIZE, TILESIZE, TILESIZE);
		tile[i].kind = TERRAIN::NONE;
		tile[i].keyName = "";
		tile[i].frame = { 0,0 };
	}
}

void mapToolScene::initSelectTile()
{
	string str[2] = { "grass0","ground0" };
	for (int i = 0; i < 2; i++)
	{
		bigTile[i].rc = RectMake(990 + (i % 2) * 140, 97 + (i / 2) * 135, 130, 130);
		bigTile[i].kind = TERRAIN::TILE;
		bigTile[i].keyName = str[i];
	}
}

void mapToolScene::initCam()
{
	cam.rc = RectMake(0, 0, 920, WINSIZEY);
	cam.pt = { 0,0 };
}

void mapToolScene::moveRect()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * TILESIZE) + cam.pt.x,
			(i / MAXTILE_HEIGHT) * TILESIZE + cam.pt.y, TILESIZE, TILESIZE);
	}
}
/// IMAGE ///
void mapToolScene::addImage()
{
	//UI//
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("wallMenu", "maptool/ui/wallmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu", "maptool/ui/objectmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("checkIcon", "maptool/ui/check.bmp", 36, 36, true, RGB(255, 0, 255));

	//TILE//
	IMAGEMANAGER->addFrameImage("grass0", "maptool/tile/grass0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("ground0", "maptool/tile/ground0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
}

/// RENDER ///
void mapToolScene::tileRender()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(tile[i].rc, cam.rc))
		{
			//타일에 정보가 없을 때 초록색 도형 출력
			if (tile[i].kind == TERRAIN::NONE)
			{
				FrameRect(getMemDC(), tile[i].rc, DARKGREEN);
				//char num[10];
				//textOut(getMemDC(), tile[i].rc.left, tile[i].rc.top, itoa(i, num, 10), GREEN);
			}
			else
			{
				imageFrameRender(tile[i].keyName, { tile[i].rc.left,tile[i].rc.top }, tile[i].frame.x, tile[i].frame.y);
				FrameRect(getMemDC(), tile[i].rc, YELLOW);
			}
		}
	}

}

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
	ptOut(getMemDC(), { 10,50 }, cam.pt, ORANGE);
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
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i].rc);
			Rectangle(getMemDC(), drag.rc);
			break;
		case OPTION::TILE_MENU:
			for (int i = 0; i < 4; i++)
			{
				Rectangle(getMemDC(), icon[i].rc);
				Rectangle(getMemDC(), bigTile[i].rc);
			}

			Rectangle(getMemDC(), drag.rc);
			break;
		case OPTION::OBJECT_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i].rc);
			Rectangle(getMemDC(), drag.rc);
			break;
		}
	}
}

void mapToolScene::checkRender()
{
	//메뉴에는 옵션버튼이 없다
	if (option != OPTION::SELECT_MENU)
	{
		for (int i = 0; i < 4; i++)
		{
			if (icon[i].isCol) imageRender("checkIcon", { icon[i].rc.left,icon[i].rc.top });
		}
		if (drag.isCol) imageRender("checkIcon", { drag.rc.left,drag.rc.top });
	}
}

void mapToolScene::imageRender(string keyName, POINT pt)
{
	IMAGEMANAGER->findImage(keyName)->render(getMemDC(), pt.x, pt.y);
}

void mapToolScene::imageAlphaRender(string keyName, POINT pt, int alpha)
{
	IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), pt.x, pt.y, alpha);
}

void mapToolScene::imageFrameRender(string keyName, POINT pt, int frameX, int frameY)
{
	IMAGEMANAGER->findImage(keyName)->frameRender(getMemDC(),pt.x,pt.y,frameX, frameY);
}

void mapToolScene::controller()
{
	if (INPUT->GetKey('W') && abs(cam.pt.y) > 0)
	{
		cam.pt.y += 5;
		moveRect();
	}
	if (INPUT->GetKey('A') && abs(cam.pt.x) > 0)
	{
		cam.pt.x += 5;
		moveRect();
	}
	if (INPUT->GetKey('S') && abs(cam.pt.y) < MAXTILE_HEIGHT * TILESIZE - WINSIZEY)
	{
		cam.pt.y -= 5;
		moveRect();
	}
	if (INPUT->GetKey('D') && abs(cam.pt.x) < MAXTILE_WIDTH * TILESIZE - 920)
	{
		cam.pt.x -= 5;
		moveRect();
	}

	if (isLeftDown) // 타일의 정보를 가져오는 기능만 수행
	{
		switch (option)
		{
		case OPTION::WALL_MENU:
			break;
		case OPTION::TILE_MENU:
			//get tile
			for (int i = 0; i < 2; i++)
			{
				if (PtInRect(&bigTile[i].rc, _ptMouse))
				{
					user.KeyName = bigTile[i].keyName;
					user.kind = bigTile[i].kind;
					// 브러쉬 아이콘에 check 표시
					resetIcon();
					icon[1].isCol = true;
				}
			}
			for (int i = 0; i < MAXTILE; i++)
			{
				if (PtInRect(&tile[i].rc, _ptMouse))
				{
					//tile[i].keyName = user.KeyName;
					//tile[i].kind = user.kind;

					//tile[i+1].keyName = user.keyName;
					//tile[i+1].kind = user.kind;....
					//여기서 tile[i+1..i+maxtile_width * 2 + 2] 까지 초기화를 해줘야한다.

					////첫번째 줄
					//tile[i].frame = { 0,0 };
					//tile[i + 1].frame = { 1,0 };
					//tile[i + 2].frame = { 2,0 };
					////두번째 줄
					//tile[i + MAXTILE_WIDTH].frame = { 0,1 };
					//tile[i + MAXTILE_WIDTH + 1].frame = { 1,1 };
					//tile[i + MAXTILE_WIDTH + 2].frame = { 2,1 };
					////세번째 줄
					//tile[i + MAXTILE_WIDTH * 2].frame = { 0,2 };
					//tile[i + MAXTILE_WIDTH * 2 + 1].frame = { 1,2 };
					//tile[i + MAXTILE_WIDTH * 2 + 2].frame = { 2,2 };

					for (int j = 0; j < 3; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							tile[i + (MAXTILE_WIDTH * j) + k].keyName = user.KeyName;
							tile[i + (MAXTILE_WIDTH * j) + k].kind = user.kind;
							tile[i + (MAXTILE_WIDTH * j) + k].frame = { k,j };
						}
					}
				}
			}
			break;
		case OPTION::OBJECT_MENU:
			break;
		}

	}
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