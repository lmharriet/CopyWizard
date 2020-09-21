#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	initTile();

	initSelectTerrain();

	initMaptool();

	initButton();

	initDrag();

	initUser();

	initSaveAndLoad();

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

	//delay를 주어 메뉴가 바뀔 때 바로 선택 되는 것을 방지
	if (user.delay < 10) user.delay++;

	//마우스가 맵툴ui에 들어오면 draw,erase 등 작업이 비활성화
	if (PtInRect(&maptool.rc, _ptMouse))
	{
		maptool.isCol = true;

		//user가 ui안에 들어오면 드래그 비활성화
		if (drag.start.x != drag.end.x ||
			drag.start.y != drag.end.y)
		{
			drag.start = drag.end = { 0,0 };
			drag.rc = RectMake(drag.end.x, drag.start.y, drag.start.x - drag.end.x, drag.end.y - drag.start.y);
		}
	}
	else maptool.isCol = false;

	//drag function
	if (dragButton.isCol && !maptool.isCol)
	{
		if (isLeftDown)
		{
			drag.start = _ptMouse;
		}
		if (isLeft)
		{
			drag.end = _ptMouse;
		}

		if (drag.start.y < drag.end.y)
		{
			if (drag.start.x < drag.end.x) drag.rc = RectMake(drag.start.x, drag.start.y, drag.end.x - drag.start.x, drag.end.y - drag.start.y);
			else drag.rc = RectMake(drag.end.x, drag.start.y, drag.start.x - drag.end.x, drag.end.y - drag.start.y);
		}
		else
		{
			if (drag.start.x > drag.end.x) drag.rc = RectMake(drag.end.x, drag.end.y, drag.start.x - drag.end.x, drag.start.y - drag.end.y);
			else drag.rc = RectMake(drag.start.x, drag.end.y, drag.end.x - drag.start.x, drag.start.y - drag.end.y);
		}
		if (isLeftUp) drag.start = drag.end = { 0,0 };
	}
	else
	{
		drag.start = drag.end = { 0,0 };
	}

	// 세이브 
	if (isSave)
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&fileWin[i], _ptMouse) && isLeftDown)
			{
				mapSave(i);
				isSave = false;
			}
		}

	// 로드
	if (isLoad)
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&fileWin[i], _ptMouse) && isLeftDown)
			{
				mapLoad(i);
				isLoad = false;
			}
		}
}

void mapToolScene::render()
{
	//타일 그리기
	tileRender();

	//옵션의 상태에 따른 UI이미지 변경
	UIRender();

	rcRender();

	checkRender();

	//메뉴 화면 아니고 user에 정보가 없으면 브러쉬 비활성화
	if (option != OPTION::SELECT_MENU && user.KeyName == "" && user.kind == TERRAIN::NONE) 
		imageAlphaRender("active", { icon[1].left,icon[1].top });

	//userRender
	if (user.kind != TERRAIN::NONE)
	{
		imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 }, 200);
	}

	textRender();

	FrameRect(getMemDC(), drag.rc, WHITE);

	//세이브 & 로드 렌더 & image 렌더
	if (isSave)
	{
		for (int i = 0; i < 3; i++)
		{
			IMAGEMANAGER->findImage("saveimg")->render(getMemDC(), fileWin[i].left, fileWin[i].top);
		}
	}
	if (isLoad)
	{
		for (int i = 0; i < 3; i++)
		{
			IMAGEMANAGER->findImage("loadimg")->render(getMemDC(), fileWin[i].left, fileWin[i].top);
		}
	}

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
				user.delay = 0;
				option = (OPTION)i;
			}
		}
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("시작화면");
		this->saveCheck();
		this->loadCheck();
		break;
	case OPTION::WALL_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			//유저가 가지고있는 데이터 초기화
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;

			//다시 들어왔을 때 icon이 미리 활성화 되어있음을 방지
			tool = TOOL::NONE;
			dragButton.isCol = false;
		}
		this->saveCheck();
		this->loadCheck();
		break;
	case OPTION::TILE_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;

			tool = TOOL::NONE;
			dragButton.isCol = false;
		}
		this->saveCheck();
		this->loadCheck();
		break;
	case OPTION::OBJECT_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;

			tool = TOOL::NONE;
			dragButton.isCol = false;
		}
		this->saveCheck();
		this->loadCheck();
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
			if (PtInRect(&icon[i], _ptMouse) && isLeftDown)
			{
				switch (i)
				{
				case 0:
					tool = TOOL::NONE;
					resetUserData();
					break;
				case 1:
					if (user.KeyName != "" && user.kind != TERRAIN::NONE)
						tool = TOOL::DRAW;
					break;
				case 2:
					tool = TOOL::ERASE;
					resetUserData();
					break;
				case 3:
					tool = TOOL::SPOID;
					resetUserData();
					break;
				}
			}
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown)
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		break;
	case OPTION::TILE_MENU:
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i], _ptMouse) && isLeftDown)
			{
				switch (i)
				{
				case 0:
					tool = TOOL::NONE;
					resetUserData();
					break;
				case 1:
					if (user.KeyName != "" && user.kind != TERRAIN::NONE)
						tool = TOOL::DRAW;
					break;
				case 2:
					tool = TOOL::ERASE;
					resetUserData();
					break;
				case 3:
					tool = TOOL::SPOID;
					resetUserData();
					break;
				}
			}
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown)
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		break;
	case OPTION::OBJECT_MENU:
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&icon[i], _ptMouse) && isLeftDown)
			{
				switch (i)
				{
				case 0:
					tool = TOOL::NONE;
					resetUserData();
					break;
				case 1:
					if (user.KeyName != "" && user.kind != TERRAIN::NONE)
						tool = TOOL::DRAW;
					break;
				case 2:
					tool = TOOL::ERASE;
					resetUserData();
					break;
				case 3:
					tool = TOOL::SPOID;
					resetUserData();
					break;
				}
			}
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown)
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		break;
	}
}

void mapToolScene::resetUserData()
{
	user.KeyName = "";
	user.kind = TERRAIN::NONE;
	user.delay = 0;
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

void mapToolScene::initMaptool()
{
	maptool.rc = RectMake(920, 0, WINSIZEX - 920, WINSIZEY);
	maptool.isCol = false;
}

void mapToolScene::initButton()
{
	//initButton
	for (int i = 0; i < 3; i++) mapOption[i] = RectMake(1020, 197 + (i * 83), 160, 60);

	for (int i = 0; i < 4; i++) icon[i] = RectMake(928, 138 + (i * 50), 50, 48);
}

void mapToolScene::initDrag()
{
	//initDrag
	drag.start = drag.end = { 0,0 };

	dragButton.rc = RectMake(928, 87, 50, 48);
	dragButton.isCol = false;
}

void mapToolScene::initUser()
{
	//initUser
	user.KeyName = "";
	user.kind = TERRAIN::NONE;
	user.delay = 0;
	tool = TOOL::NONE;
}

void mapToolScene::initSaveAndLoad()
{
	//initSaveAndLoad
	for (int i = 0; i < 3; i++) fileWin[i] = RectMake(125 + i * 250, 228, 200, 300);

	isSave = isLoad = false;
	SAVE = RectMake(955, 563, 100, 45);
	LOAD = RectMake(955, 613, 100, 45);
	BACK = RectMake(955, 663, 100, 45);
}

void mapToolScene::initSelectTerrain()
{
	//WALL//
	string wallName[6] = { "wall0","wall1","wall2","wall3","wall4","wallTile" };
	for (int i = 0; i < 6; i++)
	{
		wall[i].rc = RectMake(993 + (i % 2) * 135, 124 + (i / 2) * 135, 125, 100);
		wall[i].kind = TERRAIN::WALL;
		wall[i].keyName = wallName[i];
	}

	//TILE//
	string str[4] = { "grass0","ground0","tile7","tile9" };
	for (int i = 0; i < 4; i++)
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
	IMAGEMANAGER->addImage("saveimg", "maptool/save.bmp", 200, 300, false);
	IMAGEMANAGER->addImage("loadimg", "maptool/load.bmp", 200, 300, false);
	IMAGEMANAGER->addImage("active", "maptool/ui/active.bmp", 49, 47);

	//WALL//
	IMAGEMANAGER->addFrameImage("wall0", "maptool/wall/wall0.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall1", "maptool/wall/wall1.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall2", "maptool/wall/wall2.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall3", "maptool/wall/wall3.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall4", "maptool/wall/wall4.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wallTile", "maptool/wall/wallTile.bmp", 32, 32, 1, 1, false);

	//TILE//
	IMAGEMANAGER->addFrameImage("grass0", "maptool/tile/grass0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("ground0", "maptool/tile/ground0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("tile7", "maptool/tile/tile7.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("tile9", "maptool/tile/tile9.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
}

void mapToolScene::mapSave(int index)
{
	char str[50];
	sprintf(str, "mapData/map%d.map", index);

	HANDLE file;
	DWORD write;

	file = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, tile, sizeof(tagTile) * MAXTILE, &write, NULL);
	CloseHandle(file);
}

void mapToolScene::mapLoad(int index)
{
	char str[50];
	sprintf(str, "mapData/map%d.map", index);

	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void mapToolScene::saveCheck()
{
	if (PtInRect(&SAVE, _ptMouse) && isLeftDown)
	{
		if (!isSave)
			isSave = true;
		else
			isSave = false;
	}

}

void mapToolScene::loadCheck()
{
	if (PtInRect(&LOAD, _ptMouse) && isLeftDown)
	{
		if (!isLoad)
			isLoad = true;
		else
			isLoad = false;
	}
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

			if (tile[i].keyName != "") imageFrameRender(tile[i].keyName, { tile[i].rc.left,tile[i].rc.top }, tile[i].frame.x, tile[i].frame.y);

			if (tile[i].kind != TERRAIN::NONE)
			{
				if (tile[i].kind == TERRAIN::TILE) FrameRect(getMemDC(), tile[i].rc, SKYBLUE);
				else if (tile[i].kind == TERRAIN::WALL)FrameRect(getMemDC(), tile[i].rc, YELLOW);
				else if (tile[i].kind == TERRAIN::IMG)FrameRect(getMemDC(), tile[i].rc, ORANGE);
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

	char str[126];
	wsprintf(str, "delay : %d", user.delay);
	TextOut(getMemDC(), 10, 70, str, strlen(str));
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
		Rectangle(getMemDC(), maptool.rc);

		switch (option)
		{
		case OPTION::SELECT_MENU:
			for (int i = 0; i < 3; i++) Rectangle(getMemDC(), mapOption[i]);
			break;
		case OPTION::WALL_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);
			Rectangle(getMemDC(), dragButton.rc);
			for (int i = 0; i < 6; i++) Rectangle(getMemDC(), wall[i].rc);
			break;
		case OPTION::TILE_MENU:
			for (int i = 0; i < 4; i++)
			{
				Rectangle(getMemDC(), icon[i]);
				Rectangle(getMemDC(), bigTile[i].rc);
			}

			Rectangle(getMemDC(), dragButton.rc);
			break;
		case OPTION::OBJECT_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);
			Rectangle(getMemDC(), dragButton.rc);
			break;
		}
	}
}

void mapToolScene::checkRender()
{
	//메뉴에는 옵션버튼이 없다
	if (option != OPTION::SELECT_MENU)
	{
		switch (tool)
		{
		case TOOL::NONE:
			imageRender("checkIcon", { icon[0].left,icon[0].top });
			break;
		case TOOL::DRAW:
			imageRender("checkIcon", { icon[1].left,icon[1].top });
			break;
		case TOOL::ERASE:
			imageRender("checkIcon", { icon[2].left,icon[2].top });
			break;
		case TOOL::SPOID:
			imageRender("checkIcon", { icon[3].left,icon[3].top });
			break;
		}

		if (dragButton.isCol) imageRender("checkIcon", { dragButton.rc.left,dragButton.rc.top });
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
	IMAGEMANAGER->findImage(keyName)->frameRender(getMemDC(), pt.x, pt.y, frameX, frameY);
}

void mapToolScene::controller()
{
	if (INPUT->GetKey('W') && abs(cam.pt.y) > 0)
	{
		cam.pt.y += 5;
		moveRect();

		if (dragButton.isCol && isLeft)drag.start.y += 5;
	}
	if (INPUT->GetKey('A') && abs(cam.pt.x) > 0)
	{
		cam.pt.x += 5;
		moveRect();

		if (dragButton.isCol && isLeft)drag.start.x += 5;
	}
	if (INPUT->GetKey('S') && abs(cam.pt.y) < MAXTILE_HEIGHT * TILESIZE - WINSIZEY)
	{
		cam.pt.y -= 5;
		moveRect();

		if (dragButton.isCol && isLeft)drag.start.y -= 5;
	}
	if (INPUT->GetKey('D') && abs(cam.pt.x) < MAXTILE_WIDTH * TILESIZE - 920)
	{
		cam.pt.x -= 5;
		moveRect();

		if (dragButton.isCol && isLeft)drag.start.x -= 5;
	}

	//none drag draw
	if (isLeftDown) // 타일의 정보를 가져오는 기능만 수행
	{
		switch (option)
		{
		case OPTION::WALL_MENU:
			//get wall
			for (int i = 0; i < 6; i++)
			{
				if (PtInRect(&wall[i].rc, _ptMouse) && user.delay == 10)
				{
					user.delay = 0;

					user.KeyName = wall[i].keyName;
					user.kind = wall[i].kind;
					tool = TOOL::DRAW;
				}
			}
			//set wall
			switch (tool)
			{
			case TOOL::DRAW:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (maptool.isCol || dragButton.isCol)continue;

					if (PtInRect(&tile[i].rc, _ptMouse))
					{
						if (user.KeyName != "wallTile")
						{
							for (int j = 0; j < 4; j++)
							{
								for (int k = 0; k < 5; k++)
								{
									tile[i + (MAXTILE_WIDTH * j) + k].keyName = user.KeyName;
									tile[i + (MAXTILE_WIDTH * j) + k].frame = { k,j };

									if (j == 3) tile[i + (MAXTILE_WIDTH * j) + k].kind = user.kind;
									else tile[i + (MAXTILE_WIDTH * j) + k].kind = TERRAIN::IMG;
								}
							}
						}
						else
						{
							tile[i].keyName = user.KeyName;
							tile[i].kind = user.kind;
						}
					}
				}
				break;
			case TOOL::SPOID:
				for (int i = 0; i < MAXTILE; i++)
				{
					//user가 ui 안에 있거나, 타일 정보가 없으면 스포이드 기능 비활성화
					if (maptool.isCol || tile[i].kind == TERRAIN::NONE)continue;

					if (PtInRect(&tile[i].rc, _ptMouse))
					{
						user.KeyName = tile[i].keyName;
						user.kind = TERRAIN::WALL;
					}
				}
				break;
			}
			break;
		case OPTION::TILE_MENU:
			//get tile
			for (int i = 0; i < 4; i++)
			{
				if (PtInRect(&bigTile[i].rc, _ptMouse) && user.delay == 10)
				{
					user.delay = 0;

					user.KeyName = bigTile[i].keyName;
					user.kind = bigTile[i].kind;
					// 브러쉬 아이콘에 check 표시
					tool = TOOL::DRAW;
				}
			}
			//set tile
			if (!dragButton.isCol)
			{
				switch (tool)
				{
				case TOOL::DRAW:
					for (int i = 0; i < MAXTILE; i++)
					{
						if (maptool.isCol)continue;

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
									//생성하려는 지형의 타입이 IMG이거나, WALL일 때 생성되지 않도록 CONTINUE
									if (tile[i + (MAXTILE_WIDTH * j) + k].kind == TERRAIN::IMG ||
										tile[i + (MAXTILE_WIDTH * j) + k].kind == TERRAIN::WALL)continue;

									tile[i + (MAXTILE_WIDTH * j) + k].keyName = user.KeyName;
									tile[i + (MAXTILE_WIDTH * j) + k].kind = user.kind;
									tile[i + (MAXTILE_WIDTH * j) + k].frame = { k,j };
								}
							}
						}
					}
					break;
				case TOOL::ERASE: // 아래에 isLeft 일 때 처리
					break;
				case TOOL::SPOID:
					if (!dragButton.isCol && !maptool.isCol)
					{
						for (int i = 0; i < MAXTILE; i++)
						{
							if (maptool.isCol || tile[i].kind == TERRAIN::NONE)continue;

							if (PtInRect(&tile[i].rc, _ptMouse))
							{
								user.KeyName = tile[i].keyName;
								user.kind = tile[i].kind;
							}
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case OPTION::OBJECT_MENU:
			break;
		}
	}
	if (isLeft && option != OPTION::SELECT_MENU)
	{
		switch (tool)
		{
		case TOOL::ERASE:
			if (!dragButton.isCol)
			{
				for (int i = 0; i < MAXTILE; i++)
				{
					if (maptool.isCol)continue;

					if (PtInRect(&tile[i].rc, _ptMouse))
					{
						if (tile[i].kind == TERRAIN::NONE && tile[i].keyName == "")continue;

						tile[i].keyName = "";
						tile[i].kind = TERRAIN::NONE;
						tile[i].frame = { 0,0 };
					}
				}
			}
			break;
		}
	}

	//drag draw function
	if (isLeftUp)
	{
		if (dragButton.isCol)
		{
			switch (tool)
			{
			case TOOL::DRAW:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (user.kind == TERRAIN::WALL && user.KeyName != "wallTile")continue;

					if (tile[i].kind == TERRAIN::OBJECT || tile[i].kind == TERRAIN::WALL || tile[i].kind == TERRAIN::IMG)continue;

					if (colCheck(tile[i].rc, drag.rc))
					{
						tile[i].keyName = user.KeyName;
						tile[i].kind = user.kind;

						//첫좌표 위치 tile[i], drag.start
						//마지막좌표 위치 tile[i], drag.end

						//cout << "충돌된 부분 : " << i << '\n';
						cul.push_back(i);

						tile[i].frame = { 1,1 };
					}
				}
				if (cul.size() > 4)
				{
					//가로, 세로 구하기
					int height = abs((cul.back() / 100) - (cul.front() / 100)) + 1; // 가로 x칸
					int width = (cul.back() - ((height - 1) * 100)) - cul.front() + 1; // 세로 y칸

					tile[cul.front()].frame = { 0,0 }; // 왼쪽 위

					for (int i = 1; i < width - 1; i++)
					{
						tile[cul.front() + i].frame = { 1,0 }; // 위
					}

					tile[cul.front() + (width - 1)].frame = { 2,0 }; // 오른쪽 위

					for (int i = 1; i < height - 1; i++)
					{
						tile[cul.back() - (MAXTILE_HEIGHT * i)].frame = { 2,1 }; // 오른쪽
					}

					tile[cul.back()].frame = { 2,2 }; // 오른쪽 아래

					for (int i = 1; i < width - 1; i++)
					{
						tile[cul.back() - i].frame = { 1,2 }; // 아래
					}

					tile[cul.back() - (width - 1)].frame = { 0,2 }; // 왼쪽 아래

					for (int i = 1; i < height - 1; i++)
					{
						tile[cul.front() + (MAXTILE_HEIGHT * i)].frame = { 0,1 }; // 왼쪽
					}

				}
				cul.clear();
				break;
			case TOOL::ERASE:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (colCheck(tile[i].rc, drag.rc))
					{
						tile[i].keyName = "";
						tile[i].kind = TERRAIN::NONE;
						tile[i].frame = { 0,0 };
					}
				}
				drag.start = drag.end = { 0,0 };
				break;
			}
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