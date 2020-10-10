#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	//sound init

	ZeroMemory(&tile, sizeof(tagTile));
	ZeroMemory(&obTile, sizeof(tagTile));


	_tileSize = TILESIZE;
	_imageSize = 0;

	fadeIn = 0.0f;
	isMapToolBGM = true;

	initTile();

	initSelectTerrain();

	initMaptool();

	initButton();

	initDrag();

	initUser();

	initSaveAndLoad();


	//option = OPTION::SELECT_MENU;
	option = OPTION::WALL_MENU;
	isLeftDown = isLeft = isLeftUp = false;

	pageNum = 0;

	curTileSize = 32;
	currentX = currentY = 0;
	camSpeed = 10;
	isMiniMap = false;
	initCam();
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
	//maptoolBGM fade-in
	if (isMapToolBGM)
	{
		SOUNDMANAGER->fadeIn("mapToolBGM", fadeIn);
		fadeIn += 0.001f;
		if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
			isMapToolBGM = false;
	}

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
	//타일 이미지 그리기
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(tile[i].rc, cam.rc) || tile[i].kind == TERRAIN::NONE)continue;

		if (tile[i].keyName != "") imageStretchRender(tile[i].keyName, { tile[i].rc.left, tile[i].rc.top }, tile[i].frame.x, tile[i].frame.y, tile[i].rc);
	}

	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(obTile[i].rc, cam.rc) || obTile[i].kind == TERRAIN::NONE)continue;

		if (tile[i].keyName != "")  imageStretchRender(obTile[i].keyName, { obTile[i].rc.left,obTile[i].rc.top }, obTile[i].frame.x, obTile[i].frame.y, obTile[i].rc);
	}



	objectImgRender();

	////유닛 그리기
	//for (int i = 0; i < vUnit.size(); i++)
	//{
	//	image* img = IMAGEMANAGER->findImage(vUnit[i].keyName);

	//	img->renderResize(getMemDC(),
	//		vUnit[i].pt.x - img->getWidth() / 2,
	//		vUnit[i].pt.y - img->getHeight() / 2,
	//		img->getWidth(), img->getHeight(), vUnit[i].rc, TILESIZE);

	//	FrameRect(getMemDC(), vUnit[i].rc, RED);
	//}

	//타일 그리기
	tileRender();
	//옵션의 상태에 따른 UI이미지 변경
	UIRender();

	rcRender();

	checkRender();

	//메뉴 화면 아니고 user에 정보가 없으면 브러쉬 비활성화
	if (option != OPTION::SELECT_MENU && user.KeyName == "" && user.kind == TERRAIN::NONE)
		imageAlphaRender("active", { icon[1].left,icon[1].top + 5 });

	if (option != OPTION::SELECT_MENU && tool == TOOL::SPOID)
		imageAlphaRender("active", { dragButton.rc.left,dragButton.rc.top + 5 });

	//userRender
	if (user.kind != TERRAIN::NONE)
	{
		imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 + user.transY }, 200);
	}

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
		for (int i = 0; i < 4; i++)
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
			user.transY = 0;
			option = OPTION::SELECT_MENU;

			tool = TOOL::NONE;
			dragButton.isCol = false;
		}
		this->saveCheck();
		this->loadCheck();
		break;
	case OPTION::OTHER_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			user.transY = 0;
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
					dragButton.isCol = false;
					resetUserData();
					break;
				}
			}
		}

		if (INPUT->GetKeyDown('E'))
		{
			tool = TOOL::ERASE;
			resetUserData();
		}
		if (INPUT->GetKeyDown('M'))
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown && tool != TOOL::SPOID)
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
					dragButton.isCol = false;
					resetUserData();
					break;
				}
			}
		}

		if (INPUT->GetKeyDown('E'))
		{
			tool = TOOL::ERASE;
			resetUserData();
		}
		if (INPUT->GetKeyDown('M'))
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}

		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown && tool != TOOL::SPOID)
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
					dragButton.isCol = false;
					resetUserData();
					break;
				}
			}
		}
		if (INPUT->GetKeyDown('E'))
		{
			tool = TOOL::ERASE;
			resetUserData();
		}
		if (INPUT->GetKeyDown('M'))
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown && tool != TOOL::SPOID)
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		break;
	case OPTION::OTHER_MENU:
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
					dragButton.isCol = false;
					resetUserData();
					break;
				}
			}
		}
		if (INPUT->GetKeyDown('E'))
		{
			tool = TOOL::ERASE;
			resetUserData();
		}
		if (INPUT->GetKeyDown('M'))
		{
			if (!dragButton.isCol) dragButton.isCol = true;
			else dragButton.isCol = false;
		}
		if (PtInRect(&dragButton.rc, _ptMouse) && isLeftDown && tool != TOOL::SPOID)
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
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize), (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
		tile[i].kind = TERRAIN::NONE;
		tile[i].uKind = UNIT_KIND::NONE;
		tile[i].keyName = "";
		tile[i].frame = { 0,0 };
		tile[i].pos = { 0,0 };

		obTile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize), (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
		obTile[i].kind = TERRAIN::NONE;
		obTile[i].uKind = UNIT_KIND::NONE;
		obTile[i].keyName = "";
		obTile[i].pos = { obTile[i].rc.left,obTile[i].rc.top };
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
	for (int i = 0; i < 4; i++) mapOption[i] = RectMake(1020, 130 + (i * 83), 160, 60);

	for (int i = 0; i < 4; i++) icon[i] = RectMake(1005 + (i * 70), 70, 50, 48);

	for (int i = 0; i < 2; i++) pageButton[i] = RectMake(1115 + (i * 85), 567, 32, 30);
}

void mapToolScene::initDrag()
{
	//initDrag
	drag.start = drag.end = { 0,0 };

	dragButton.rc = RectMake(933, 70, 50, 48);
	dragButton.isCol = false;
}

void mapToolScene::initUser()
{
	//initUser
	user.KeyName = "";
	user.kind = TERRAIN::NONE;
	user.delay = 0;
	user.transY = 0;
	user.uKind = UNIT_KIND::NONE;
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
	string wallName[4] = { "wall0","wall1","wall2","wallTile" };
	for (int i = 0; i < 4; i++)
	{
		wall[i].rc = RectMake(929 + (i % 2) * 175, 128 + (i / 2) * 140, 165, 117);
		wall[i].kind = TERRAIN::WALL;
		wall[i].keyName = wallName[i];
	}
	//fix
	wall[4].rc = RectMake(929, 398, 78, 67);
	wall[4].kind = TERRAIN::WALL;
	wall[4].keyName = "topWall";

	wall[5].rc = RectMake(1016, 398, 78, 67);
	wall[5].kind = TERRAIN::WALL;
	wall[5].keyName = "bottomWall";

	wall[6].rc = RectMake(958, 478, 38, 38);
	wall[6].kind = TERRAIN::WALL;
	wall[6].keyName = "leftWall";

	wall[7].rc = RectMake(1009, 478, 38, 38);
	wall[7].kind = TERRAIN::WALL;
	wall[7].keyName = "rightWall";

	wall[8].rc = RectMake(1104, 398, 30, 67);
	wall[8].kind = TERRAIN::WALL;
	wall[8].keyName = "shopWall0";

	wall[9].rc = RectMake(1140, 398, 30, 67);
	wall[9].kind = TERRAIN::WALL;
	wall[9].keyName = "shopWall1";

	wall[10].rc = RectMake(1174, 396, 30, 30);
	wall[10].kind = TERRAIN::WALL;
	wall[10].keyName = "shopWall2";

	wall[11].rc = RectMake(1207, 396, 30, 30);
	wall[11].kind = TERRAIN::WALL;
	wall[11].keyName = "shopWall3";

	wall[12].rc = RectMake(1159, 478, 30, 30);
	wall[12].kind = TERRAIN::WALL;
	wall[12].keyName = "wallTile5";

	wall[13].rc = RectMake(1211, 478, 30, 30);
	wall[13].kind = TERRAIN::WALL;
	wall[13].keyName = "wallTile6";

	wall[14].rc = RectMake(1058, 478, 38, 38);
	wall[14].kind = TERRAIN::WALL;
	wall[14].keyName = "wallTile7";

	wall[15].rc = RectMake(1107, 478, 38, 38);
	wall[15].kind = TERRAIN::WALL;
	wall[15].keyName = "wallTile8";

	wall[16].rc = RectMake(1058, 500, 38, 38);
	wall[16].kind = TERRAIN::WALL;
	wall[16].keyName = "wallTile9";

	wall[17].rc = RectMake(1107, 500, 38, 38);
	wall[17].kind = TERRAIN::WALL;
	wall[17].keyName = "wallTile10";



	//TILE//
	string str[6] = { "grass0","grass1","ground0","grass2","ground1","ground2" };
	for (int i = 0; i < 6; i++)
	{
		bigTile[i].rc = RectMake(932 + (i % 3) * 116, 136 + (i / 3) * 115, 100, 100);
		bigTile[i].kind = TERRAIN::TILE;
		bigTile[i].keyName = str[i];
	}

	//OBJECT//
	//page1//
	string objectName[5] = { "flowerbed1","flowerbed2","bossDoor","pillar0","pillar1" };
	object[0].rc = RectMake(1139, 175, 96, 110);
	object[1].rc = RectMake(976, 175, 96, 110);
	object[2].rc = RectMake(944, 335, 120, 150);
	object[3].rc = RectMake(1114, 337, 30, 130);
	object[4].rc = RectMake(1203, 340, 60, 130);

	for (int i = 0; i < 5; i++)
	{
		object[i].kind = TERRAIN::OBJECT;
		object[i].keyName = objectName[i];
	}
	//page2//
	string decoName[9] = { "decoGrass3","decoGrass2","decoGrass1","tomb0","tomb1","flower","window0","window1", "flag0" };
	for (int i = 0; i < 9; i++)
	{
		deco[i].rc = RectMake(941 + (i % 3) * 120, 170 + (i / 3) * 105, 90, 75);
		deco[i].keyName = decoName[i];
		deco[i].kind = TERRAIN::DECO;
	}

	//OTHER

	string otherName[6] = { "knightCard","mageCard","golemCard","ghoulCard","slimeKingCard","playerCard" };
	for (int i = 0; i < 6; i++)
	{
		other[i].kind = TERRAIN::UNIT;
		other[i].keyName = otherName[i];

		if(i == 5)  other[i].rc = RectMake(928, 170 + 210, 105, 95);
		else if(i == 4) other[i].rc = RectMake(928 + 2 * 120, 132 + (i / 3) * 115, 105, 95);
		else other[i].rc = RectMake(928 + (i % 3) * 120, 132 + (i / 3) * 115, 105, 95);

		other[i].unit = UNIT_KIND(i);
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
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize) + cam.pt.x,
			(i / MAXTILE_HEIGHT) * _tileSize + cam.pt.y, _tileSize, _tileSize);

		obTile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize) + cam.pt.x,
			(i / MAXTILE_HEIGHT) * _tileSize + cam.pt.y, _tileSize, _tileSize);
	}
}
/// IMAGE ///
void mapToolScene::addImage()
{
	//UI//
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("otherMenu", "maptool/ui/otherMenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("wallMenu", "maptool/ui/wallmenu1.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu1.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu", "maptool/ui/objectmenu1.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu2", "maptool/ui/objectmenu2.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu3", "maptool/ui/objectmenu3.bmp", 360, 720);
	IMAGEMANAGER->addImage("checkIcon", "maptool/ui/check.bmp", 36, 36, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("saveimg", "maptool/save.bmp", 200, 300, false);
	IMAGEMANAGER->addImage("loadimg", "maptool/load.bmp", 200, 300, false);
	IMAGEMANAGER->addImage("active", "maptool/ui/active.bmp", 40, 40);

	//WALL//
	IMAGEMANAGER->addFrameImage("wall0", "maptool/wall/wall0.bmp", _tileSize * 5, _tileSize * 4, 5, 1);
	IMAGEMANAGER->addFrameImage("wall1", "maptool/wall/wall1.bmp", _tileSize * 5, _tileSize * 4, 5, 1);
	IMAGEMANAGER->addFrameImage("wall2", "maptool/wall/wall2.bmp", _tileSize * 5, _tileSize * 4, 5, 1);
	IMAGEMANAGER->addFrameImage("wallTile", "maptool/wall/wallTile.bmp", 32, 32, 1, 1, true);

	//fix
	IMAGEMANAGER->addImage("topWall", "maptool/wall/topWall.bmp", _tileSize * 16, _tileSize * 9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bottomWall", "maptool/wall/bottomWall.bmp", _tileSize * 16, _tileSize * 9, true, RGB(255, 0, 255));
	//fix 10/07
	IMAGEMANAGER->addImage("leftWall", "maptool/wall/wallTile1.bmp", _tileSize, _tileSize);
	IMAGEMANAGER->addImage("rightWall", "maptool/wall/wallTile2.bmp", _tileSize, _tileSize);
	IMAGEMANAGER->addImage("shopWall0", "maptool/wall/shopWall0.bmp", _tileSize * 2, _tileSize * 6);
	IMAGEMANAGER->addImage("shopWall1", "maptool/wall/shopWall1.bmp", _tileSize * 2, _tileSize * 6);
	//fix 10/08
	IMAGEMANAGER->addImage("shopWall2", "maptool/wall/shopWall2.bmp", _tileSize * 2, _tileSize * 3);
	IMAGEMANAGER->addImage("shopWall3", "maptool/wall/shopWall3.bmp", _tileSize * 2, _tileSize * 3);

	IMAGEMANAGER->addImage("wallTile5", "maptool/wall/wallTile5.bmp", _tileSize, _tileSize);
	IMAGEMANAGER->addImage("wallTile6", "maptool/wall/wallTile6.bmp", _tileSize, _tileSize);

	IMAGEMANAGER->addImage("wallTile7", "maptool/wall/wallTile7.bmp", _tileSize, _tileSize);
	IMAGEMANAGER->addImage("wallTile8", "maptool/wall/wallTile8.bmp", _tileSize, _tileSize);

	IMAGEMANAGER->addImage("wallTile9", "maptool/wall/wallTile9.bmp", _tileSize, _tileSize);
	IMAGEMANAGER->addImage("wallTile10", "maptool/wall/wallTile10.bmp", _tileSize, _tileSize);

	//TILE//
	IMAGEMANAGER->addFrameImage("grass0", "maptool/tile/grass0.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);
	IMAGEMANAGER->addFrameImage("grass1", "maptool/tile/grass1.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);
	IMAGEMANAGER->addFrameImage("grass2", "maptool/tile/grass2.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);
	IMAGEMANAGER->addFrameImage("ground0", "maptool/tile/ground0.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);
	IMAGEMANAGER->addFrameImage("ground1", "maptool/tile/ground1.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);
	IMAGEMANAGER->addFrameImage("ground2", "maptool/tile/ground2.bmp", _tileSize * 3, _tileSize * 3, 3, 3, true);

	//OBJECT//
	IMAGEMANAGER->addFrameImage("flowerbed1", "maptool/object/flowerbed1.bmp", _tileSize * 2, _tileSize * 2, 2, 2, true);
	IMAGEMANAGER->addFrameImage("flowerbed2", "maptool/object/flowerbed1.bmp", _tileSize * 2, _tileSize * 2, 2, 2, true);
	IMAGEMANAGER->addImage("tree0", "maptool/object/tree0.bmp", _tileSize * 6, _tileSize * 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tree1", "maptool/object/tree1.bmp", _tileSize * 6, _tileSize * 7, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bossDoor", "maptool/object/bossDoor-col.bmp", _tileSize * 6, _tileSize * 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("pillar0", "maptool/object/pillar0.bmp", _tileSize * 1, _tileSize * 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("pillar1", "maptool/object/pillar1.bmp", _tileSize * 2, _tileSize * 5, true, RGB(255, 0, 255));

	//DECO//
	IMAGEMANAGER->addImage("decoGrass3", "maptool/deco/decoGrass3.bmp", 50, 26, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("decoGrass2", "maptool/deco/decoGrass2.bmp", 56, 44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("decoGrass1", "maptool/deco/decoGrass1.bmp", 64, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tomb0", "maptool/deco/tomb0.bmp", 116, 84, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tomb1", "maptool/deco/tomb1.bmp", 116, 80, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("flower", "maptool/deco/flower.bmp", 70, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("window0", "maptool/deco/window0.bmp", 82, 74, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("window1", "maptool/deco/window1.bmp", 82, 74, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("flag0", "maptool/deco/flag0.bmp", 68, 96, true, RGB(255, 0, 255));

	//OTHER//
	IMAGEMANAGER->addImage("ghoulCard", "maptool/other/ghoul.bmp", 48, 73, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("knightCard", "maptool/other/knight.bmp", 48, 73, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("mageCard", "maptool/other/mage.bmp", 48, 73, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("playerCard", "maptool/other/player.bmp", 48, 73, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("golemCard", "maptool/other/golem.bmp", 96, 146, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("slimeKingCard", "maptool/other/slimeKing.bmp", 96, 146, true, RGB(255, 0, 255));

}

void mapToolScene::mapSave(int index)
{
	char str[50], str1[50], str2[50];
	sprintf(str, "mapData/map%d.map", index);
	sprintf(str1, "mapData/object%d.map", index);

	HANDLE file;
	DWORD write;

	file = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, tile, sizeof(tagTile) * MAXTILE, &write, NULL);
	CloseHandle(file);

	file = CreateFile(str1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, obTile, sizeof(tagTile) * MAXTILE, &write, NULL);
	CloseHandle(file);
}

void mapToolScene::mapLoad(int index)
{
	char str[50], str1[50], str2[50];
	sprintf(str, "mapData/map%d.map", index);
	sprintf(str1, "mapData/object%d.map", index);

	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);

	file = CreateFile(str1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, obTile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void mapToolScene::saveCheck()
{
	if (PtInRect(&SAVE, _ptMouse) && isLeftDown)
	{
		if (!isSave)
		{
			isSave = true;
			isLoad = false;
		}

		else
			isSave = false;
	}

}

void mapToolScene::loadCheck()
{
	if (PtInRect(&LOAD, _ptMouse) && isLeftDown)
	{
		if (!isLoad)
		{
			isLoad = true;
			isSave = false;
		}
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
			}

			if (tile[i].kind != TERRAIN::NONE)
			{
				switch (tile[i].kind)
				{
				case TERRAIN::TILE:
					FrameRect(getMemDC(), tile[i].rc, NAVY);
					break;
				case TERRAIN::WALL:
					FrameRect(getMemDC(), tile[i].rc, SUNLIGHT);
					break;
				case TERRAIN::IMG:
					FrameRect(getMemDC(), tile[i].rc, BROWN);
					break;
				case TERRAIN::OBJECT:
					FrameRect(getMemDC(), tile[i].rc, PINK);
					break;
				}
			}
		}

		if (colCheck(obTile[i].rc, cam.rc))
		{
			if (obTile[i].kind == TERRAIN::DECO || obTile[i].kind == TERRAIN::UNIT)
			{
				FrameRect(getMemDC(), obTile[i].rc, WHITE);
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

		//obeject page 넘기기
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&pageButton[0], _ptMouse) && isLeftDown)
			{
				pageNum--;

				if (pageNum < 0) pageNum = 0;
				break;
			}
			else if (PtInRect(&pageButton[1], _ptMouse) && isLeftDown)
			{
				pageNum++;
				if (pageNum > 2) pageNum = 2;
				break;
			}
		}
		imageRender(page[pageNum], { 920,0 });
		break;
	case OPTION::SELECT_MENU:
		imageRender("mapMenu", { 920,0 });
		break;
	case OPTION::OTHER_MENU:
		imageRender("otherMenu", { 920,0 }); // fix
		break;
	}
}

void mapToolScene::textRender()
{
	ptOut(getMemDC(), _ptMouse, _ptMouse, WHITE);

	char str[126];
	wsprintf(str, "_tileSize : %d", _tileSize);
	TextOut(getMemDC(), 10, 70, str, strlen(str));
}

void mapToolScene::buttonRender()
{
	Rectangle(getMemDC(), SAVE);
	Rectangle(getMemDC(), LOAD);
	Rectangle(getMemDC(), BACK);

	if (option != OPTION::SELECT_MENU)
	{
		for (int i = 0; i < 2; i++)Rectangle(getMemDC(), pageButton[i]);
	}
}

void mapToolScene::rcRender()
{
	//button
	if (INPUT->GetToggleKey(VK_F1)) // 렉트를 껏다 켯다 할 수 있음
	{
		buttonRender();
		FrameRect(getMemDC(), maptool.rc, WHITE);

		switch (option)
		{
		case OPTION::SELECT_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), mapOption[i]);
			break;
		case OPTION::WALL_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);
			Rectangle(getMemDC(), dragButton.rc);
			for (int i = 0; i < 18; i++) Rectangle(getMemDC(), wall[i].rc); // fix 10 - 16
			break;
		case OPTION::TILE_MENU:
			for (int i = 0; i < 4; i++)	Rectangle(getMemDC(), icon[i]);
			for (int i = 0; i < 6; i++) Rectangle(getMemDC(), bigTile[i].rc);

			Rectangle(getMemDC(), dragButton.rc);
			break;
		case OPTION::OBJECT_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);

			for (int i = 0; i < 5; i++) Rectangle(getMemDC(), object[i].rc);

			for (int i = 0; i < 9; i++) Rectangle(getMemDC(), deco[i].rc);
			Rectangle(getMemDC(), dragButton.rc);
			break;
		case OPTION::OTHER_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);

			for (int i = 0; i < 6; i++)Rectangle(getMemDC(), other[i].rc);
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

void mapToolScene::objectImgRender()
{
	//object image render
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(tile[i].rc, cam.rc) || (obTile[i].kind != TERRAIN::DECO && obTile[i].kind != TERRAIN::UNIT)) continue;

		if (obTile[i].kind == TERRAIN::DECO)
		{
			image* img = IMAGEMANAGER->findImage(obTile[i].keyName);

			img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
		}

		else if (obTile[i].kind == TERRAIN::UNIT)
		{
			image* img = IMAGEMANAGER->findImage(obTile[i].keyName);

			img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
		}
	}

	string key;
	image* img;
	int width, height;
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(tile[i].rc, cam.rc) ||
			(tile[i].kind != TERRAIN::WALL && tile[i].kind != TERRAIN::OBJECT))continue;

		int w = MAXTILE_WIDTH;
		int w2 = MAXTILE_WIDTH * 2;
		int w3 = MAXTILE_WIDTH * 3;
		int w4 = MAXTILE_WIDTH * 4;
		int w5 = MAXTILE_WIDTH * 5;
		int w6 = MAXTILE_WIDTH * 6;

		switch (tile[i].kind)
		{
		case TERRAIN::WALL:
			key = tile[i].keyName;

			if (key == "topWall") // fix
			{
				if ((key == tile[i + 1].keyName &&
					key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
					key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
					key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
					key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&

					key == tile[i - w3 + 2].keyName && key == tile[i - w3 + 3].keyName &&
					key == tile[i - w3 + 4].keyName && key == tile[i - w3 + 5].keyName &&
					key == tile[i - w4 + 2].keyName && key == tile[i - w4 + 3].keyName &&
					key == tile[i - w4 + 4].keyName && key == tile[i - w4 + 5].keyName &&

					key == tile[i - w4 + 10].keyName && key == tile[i - w4 + 11].keyName &&
					key == tile[i - w4 + 12].keyName && key == tile[i - w4 + 13].keyName &&
					key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&

					key == tile[i - w3 + 10].keyName && key == tile[i - w3 + 11].keyName &&
					key == tile[i - w3 + 12].keyName && key == tile[i - w3 + 13].keyName &&
					key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&

					key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
					key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
					key == tile[i + 14].keyName && key == tile[i + 15].keyName)
					== false)
				{
					continue;
				}

				height = 8 * _tileSize;

				img = IMAGEMANAGER->findImage(key);
				img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}

			else if (key == "bottomWall") // fix
			{
				if ((key == tile[i + 1].keyName &&
					key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
					key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
					key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
					key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&
					key == tile[i - w5].keyName && key == tile[i - w5 + 1].keyName &&
					key == tile[i - w6].keyName && key == tile[i - w6 + 1].keyName &&

					key == tile[i - w + 2].keyName && key == tile[i - w + 3].keyName &&
					key == tile[i - w + 4].keyName &&
					key == tile[i + 2].keyName && key == tile[i + 3].keyName &&
					key == tile[i + 4].keyName &&


					key == tile[i - w + 11].keyName && key == tile[i - w + 12].keyName &&
					key == tile[i - w + 13].keyName &&
					key == tile[i + 11].keyName && key == tile[i + 12].keyName &&
					key == tile[i + 13].keyName &&

					key == tile[i + 14].keyName && key == tile[i + 15].keyName &&
					key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
					key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
					key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&
					key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&
					key == tile[i - w5 + 14].keyName && key == tile[i - w5 + 15].keyName &&
					key == tile[i - w6 + 14].keyName && key == tile[i - w6 + 15].keyName
					) == false)
				{
					continue;
				}
				height = 8 * _tileSize;

				img = IMAGEMANAGER->findImage(key);
				img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}

			else if (key == "leftWall" || key == "rightWall" || key == "wallTile"
				|| key == "wallTile5" || key == "wallTile6" || key == "wallTile7"
				|| key == "wallTile8" || key == "wallTile9" || key == "wallTile10")
			{
				float scale = (float)(tile[i].rc.right - tile[i].rc.left) / TILESIZE;

				image* img = IMAGEMANAGER->findImage(tile[i].keyName);

				img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}

			else if (key == "shopWall0" || key == "shopWall1")
			{

				height = 5 * _tileSize;

				if (key == "shopWall0")
				{
					if ((key == tile[i + 1].keyName && key == tile[i - w].keyName && key == tile[i - w2].keyName &&
						key == tile[i - w3].keyName && key == tile[i - w4].keyName && key == tile[i - w5].keyName) == false)
					{
						continue;
					}

					img = IMAGEMANAGER->findImage(key);
					img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
				}

				if (key == "shopWall1")
				{
					//예외 처리
					if ((key == tile[i + 1].keyName && key == tile[i + 1 - w].keyName && key == tile[i + 1 - w2].keyName &&
						key == tile[i + 1 - w3].keyName && key == tile[i + 1 - w4].keyName && key == tile[i + 1 - w5].keyName) == false)
					{
						continue;
					}


					img = IMAGEMANAGER->findImage(key);
					img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
				}
			}

			else if (key == "shopWall2" || key == "shopWall3")
			{
				if ((key == tile[i + 1].keyName &&
					key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
					key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName) == false)
				{
					continue;
				}
				height = 2 * _tileSize;

				img = IMAGEMANAGER->findImage(key);
				img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}

			else
			{
				height = 3 * _tileSize;

				img = IMAGEMANAGER->findImage(key);
				img->frameRender(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, tile[i].frame.x, tile[i].frame.y, tile[i].rc, TILESIZE);
			}

			break;
		case TERRAIN::OBJECT:
			// 2X2인 오브젝트만 처리

			//i object, i+1 object, i+w object, i+w+1 object
			if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
				tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

			key = tile[i].keyName;

			if (!(tile[i + 1].keyName == key &&
				tile[i + MAXTILE_WIDTH].keyName == key &&
				tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

			if (tile[i].keyName == "flowerbed1")
			{
				key = "tree0";
				width = 2 * _tileSize;
				height = 4 * _tileSize;

				img = IMAGEMANAGER->findImage(key);

				img->renderResize(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}
			else if (tile[i].keyName == "flowerbed2")
			{
				key = "tree1";
				width = 2 * _tileSize;
				height = 5 * _tileSize;

				img = IMAGEMANAGER->findImage(key);

				img->renderResize(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}
			else if (tile[i].keyName == "pillar1")
			{
				key = "pillar1";

				height = 3 * _tileSize;

				img = IMAGEMANAGER->findImage(key);
				img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
			}
			break;
		}
	}

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].kind != TERRAIN::OBJECT) continue;

		// 1x? 이거나 다른 특수한 오브젝트 처리

		string key;
		int height;
		if (tile[i].keyName == "pillar0") // 가로 : 1
		{
			key = "pillar0";

			height = 3 * _tileSize;
			image* img = IMAGEMANAGER->findImage(key);
			img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
		}
		else if (tile[i].keyName == "bossDoor")
		{
			if (tile[i + 5].keyName != "bossDoor")continue;

			key = "bossDoor";

			height = 7 * _tileSize;
			image* img = IMAGEMANAGER->findImage(key);
			img->renderResize(getMemDC(), tile[i].rc.left, tile[i].rc.top - height, img->getWidth(), img->getHeight(), tile[i].rc, TILESIZE);
		}
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

void mapToolScene::imageStretchRender(string keyName, POINT pt, int frameX, int frameY, RECT scale)
{
	IMAGEMANAGER->findImage(keyName)->frameRender(getMemDC(), pt.x, pt.y, frameX, frameY, scale);
}

void mapToolScene::controller()
{
	if (!isMiniMap) {
		if (INPUT->GetKey('W'))
		{
			cam.pt.y += camSpeed;
			moveRect();

			if (dragButton.isCol && isLeft)drag.start.y += 5;
		}
		if (INPUT->GetKey('A'))
		{
			cam.pt.x += camSpeed;
			moveRect();

			if (dragButton.isCol && isLeft)drag.start.x += 5;
		}
		if ((INPUT->GetKey('S')))
		{
			cam.pt.y -= camSpeed;
			moveRect();

			if (dragButton.isCol && isLeft)drag.start.y -= 5;
		}
		if ((INPUT->GetKey('D')))
		{
			cam.pt.x -= camSpeed;
			moveRect();

			if (dragButton.isCol && isLeft)drag.start.x -= 5;
		}
	}

	//Zoom in / out
	if (_mouseWheel == 1 && _tileSize < 50)
	{
		_tileSize += 2;
		_imageSize += 2;
		camSpeed++;
		currentX = tile[0].rc.left;
		currentY = tile[0].rc.top;
		for (int i = 0; i < MAXTILE; i++)
		{
			tile[i].rc = RectMake(currentX + (i % MAXTILE_WIDTH * _tileSize), currentY + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
			obTile[i].rc = RectMake(currentX + (i % MAXTILE_WIDTH * _tileSize), currentY + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
		}
		_mouseWheel = 0;
	}
	else if (_mouseWheel == -1 && _tileSize > 10)
	{
		_tileSize -= 2;
		_imageSize -= 2;
		if (camSpeed > 4) {
			camSpeed--;
		}
		currentX = tile[0].rc.left;
		currentY = tile[0].rc.top;
		for (int i = 0; i < MAXTILE; i++)
		{
			tile[i].rc = RectMake(currentX + (i % MAXTILE_WIDTH * _tileSize), currentY + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
			obTile[i].rc = RectMake(currentX + (i % MAXTILE_WIDTH * _tileSize), currentY + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
		}
		_mouseWheel = 0;
	}


	if (INPUT->GetKeyDown(VK_TAB))
	{
		if (!isMiniMap)
		{
			curTileSize = _tileSize;
			_tileSize = 6;
			_mouseWheel = 0;
			isMiniMap = true;
			for (int i = 0; i < MAXTILE; i++)
			{
				tile[i].rc = RectMake(150 + (i % MAXTILE_WIDTH * _tileSize), 60 + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
				obTile[i].rc = RectMake(150 + (i % MAXTILE_WIDTH * _tileSize), 60 + (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
			}
		}
		else
		{
			_tileSize = curTileSize;
			isMiniMap = false;
			for (int i = 0; i < MAXTILE; i++)
			{
				tile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize), (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
				obTile[i].rc = RectMake((i % MAXTILE_WIDTH * _tileSize), (i / MAXTILE_HEIGHT) * _tileSize, _tileSize, _tileSize);
			}
		}
	}

	//none drag draw
	if (isLeftDown) // 타일의 정보를 가져오는 기능만 수행
	{
		switch (option)
		{
		case OPTION::WALL_MENU:
			//get wall
			for (int i = 0; i < 18; i++) // fix 10 - > 18
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

						if (user.KeyName == "topWall") // fix
						{
							//14

							//vertical wall
							for (int q = 0; q <= 1; q++)
							{
								for (int k = 4; k <= 8; k++)
								{
									tile[i + (k * MAXTILE_WIDTH) + q].keyName = user.KeyName;
									tile[i + (k * MAXTILE_WIDTH) + q].kind = user.kind;
								}
							}

							//horizontal wall
							for (int q = 4; q <= 5; q++)
							{
								for (int k = 1; k <= 5; k++)
								{
									tile[i + (q * MAXTILE_WIDTH) + k].keyName = user.KeyName;
									tile[i + (q * MAXTILE_WIDTH) + k].kind = user.kind;
								}
							}

							//vertical wall
							for (int q = 0; q <= 1; q++)
							{
								for (int k = 4; k <= 8; k++)
								{
									tile[(i + 14) + (k * MAXTILE_WIDTH) + q].keyName = user.KeyName;
									tile[(i + 14) + (k * MAXTILE_WIDTH) + q].kind = user.kind;
								}
							}

							//horizontal wall
							for (int q = 4; q <= 5; q++)
							{
								for (int k = 1; k <= 5; k++)
								{
									tile[(i + 14) + (q * MAXTILE_WIDTH) - (k - 1)].keyName = user.KeyName;
									tile[(i + 14) + (q * MAXTILE_WIDTH) - (k - 1)].kind = user.kind;
								}
							}
						}

						else if (user.KeyName == "bottomWall")
						{
							//vertical
							for (int q = 0; q <= 1; q++)
							{
								for (int k = 2; k <= 8; k++)
								{
									tile[i + (k * MAXTILE_WIDTH) + q].keyName = user.KeyName;
									tile[i + (k * MAXTILE_WIDTH) + q].kind = user.kind;
								}
							}

							//horizontal
							for (int q = 7; q <= 8; q++)
							{
								for (int k = 2; k <= 4; k++)
								{
									tile[i + (q * MAXTILE_WIDTH) + k].keyName = user.KeyName;
									tile[i + (q * MAXTILE_WIDTH) + k].kind = user.kind;
								}
							}

							//vertical
							for (int q = 14; q <= 15; q++)
							{
								for (int k = 2; k <= 8; k++)
								{
									tile[i + (k * MAXTILE_WIDTH) + q].keyName = user.KeyName;
									tile[i + (k * MAXTILE_WIDTH) + q].kind = user.kind;
								}
							}

							//horizontal
							for (int q = 7; q <= 8; q++)
							{
								for (int k = 11; k <= 13; k++)
								{
									tile[i + (q * MAXTILE_WIDTH) + k].keyName = user.KeyName;
									tile[i + (q * MAXTILE_WIDTH) + k].kind = user.kind;
								}
							}
						}

						else if (user.KeyName == "leftWall" || user.KeyName == "rightWall" || user.KeyName == "wallTile" ||
							user.KeyName == "wallTile5" || user.KeyName == "wallTile6" || user.KeyName == "wallTile7" ||
							user.KeyName == "wallTile8" || user.KeyName == "wallTile9" || user.KeyName == "wallTile10")
						{
							tile[i].keyName = user.KeyName;
							tile[i].kind = user.kind;
						}

						else if (user.KeyName == "shopWall0")
						{
							//i, i + w, i + 2w, i + 3w, i + 4w, i + 5w, i + 5w + 1
							for (int q = 0; q < 6; q++)
							{
								tile[i + (MAXTILE_WIDTH * q)].keyName = user.KeyName;
								tile[i + (MAXTILE_WIDTH * q)].kind = user.kind;

								if (q == 5)
								{
									tile[i + (MAXTILE_WIDTH * q + 1)].keyName = user.KeyName;
									tile[i + (MAXTILE_WIDTH * q + 1)].kind = user.kind;
								}
							}
						}

						else if (user.KeyName == "shopWall1")
						{
							for (int q = 0; q < 6; q++)
							{
								tile[(i + 1) + (MAXTILE_WIDTH * q)].keyName = user.KeyName;
								tile[(i + 1) + (MAXTILE_WIDTH * q)].kind = user.kind;

								if (q == 5)
								{
									tile[(i + 1) + (MAXTILE_WIDTH * q) - 1].keyName = user.KeyName;
									tile[(i + 1) + (MAXTILE_WIDTH * q) - 1].kind = user.kind;
								}
							}
						}

						else if (user.KeyName == "shopWall2" || user.KeyName == "shopWall3")
						{
							for (int q = 0; q < 3; q++)
							{
								for (int k = 0; k < 2; k++)
								{
									tile[i + MAXTILE_WIDTH * q + k].keyName = user.KeyName;
									tile[i + MAXTILE_WIDTH * q + k].kind = user.kind;
								}
							}
						}

						else
						{
							tile[i].keyName = user.KeyName;
							tile[i].kind = user.kind;
							tile[i].frame.x = RANDOM->range(5);
							tile[i].frame.y = 0;
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
			for (int i = 0; i < 6; i++)
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
							for (int j = 0; j < 3; j++)
							{
								for (int k = 0; k < 3; k++)
								{
									//생성하려는 지형의 타입이 IMG이거나, WALL일 때 생성되지 않도록 CONTINUE
									if (tile[i + (MAXTILE_WIDTH * j) + k].kind == TERRAIN::IMG ||
										tile[i + (MAXTILE_WIDTH * j) + k].kind == TERRAIN::WALL ||
										tile[i + (MAXTILE_WIDTH * j) + k].kind == TERRAIN::OBJECT)continue;

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
							if (maptool.isCol || tile[i].kind != TERRAIN::TILE)continue;

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
			switch (pageNum)
			{
			case 0: //page1
				for (int i = 0; i < 5; i++)
				{
					if (PtInRect(&object[i].rc, _ptMouse) && user.delay == 10)
					{
						if (pageNum != 0)break;

						user.delay = 0;

						user.KeyName = object[i].keyName;
						user.kind = object[i].kind;

						if (user.KeyName == "pillar0" || user.KeyName == "pillar1")
						{
							user.transY = -(TILESIZE * 3);
						}
						else if (user.KeyName == "bossDoor")
						{
							user.transY = -(TILESIZE * 7);
						}
						else user.transY = 0;
						// 브러쉬 아이콘에 check 표시
						tool = TOOL::DRAW;
					}
				}
				break;
			case 1: //page2
				for (int i = 0; i < 9; i++)
				{
					if (PtInRect(&deco[i].rc, _ptMouse) && user.delay == 10)
					{
						user.delay = 0;
						user.KeyName = deco[i].keyName;
						user.kind = deco[i].kind;
						tool = TOOL::DRAW;
					}
				}
				break;
			case 2:
				break;
			}

			switch (tool)
			{
			case TOOL::DRAW:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (maptool.isCol)continue;

					if (user.kind == TERRAIN::DECO)
					{
						if (PtInRect(&obTile[i].rc, _ptMouse))
						{
							obTile[i].kind = user.kind;
							obTile[i].keyName = user.KeyName;
						}
					}
					else // user.kind == TERRAIN::OBJECT
					{
						if (PtInRect(&tile[i].rc, _ptMouse))
						{

							if (user.KeyName == "pillar0") //가로가 1
							{
								tile[i].kind = user.kind;
								tile[i].keyName = user.KeyName;
							}
							else if (user.KeyName == "bossDoor") // i,i+5
							{
								tile[i].kind = user.kind;
								tile[i + 5].kind = user.kind;

								tile[i].keyName = user.KeyName;
								tile[i + 5].keyName = user.KeyName;
							}
							else
							{
								//2,2 일 때
								for (int j = 0; j < 2; j++)
								{
									for (int k = 0; k < 2; k++)
									{
										tile[i + (MAXTILE_WIDTH * j) + k].kind = user.kind;
										tile[i + (MAXTILE_WIDTH * j) + k].keyName = user.KeyName;
										tile[i + (MAXTILE_WIDTH * j) + k].frame = { k,j };
									}
								}
							}
						}
					}
				}
				break;
			case TOOL::ERASE:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (PtInRect(&obTile[i].rc, _ptMouse))
					{
						obTile[i].keyName = "";
						obTile[i].kind = TERRAIN::NONE;
						obTile[i].uKind = UNIT_KIND::NONE;
					}
				}
				break;
			}
		case OPTION::OTHER_MENU:
			//get tile
			for (int i = 0; i < 6; i++)
			{
				if (PtInRect(&other[i].rc, _ptMouse) && user.delay == 10)
				{
					user.delay = 0;

					user.KeyName = other[i].keyName;
					user.kind = TERRAIN::UNIT;

					switch (i)
					{
					case 0:
						user.uKind = UNIT_KIND::KNIGHT;
						break;
					case 1:
						user.uKind = UNIT_KIND::MAGE;
						break;
					case 2:
						user.uKind = UNIT_KIND::GOLEM;
						break;
					case 3:
						user.uKind = UNIT_KIND::GHOUL;
						break;
					}

					tool = TOOL::DRAW;
				}
			}
			
			//set
			switch (tool)
			{
			case TOOL::DRAW:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (maptool.isCol)continue;

					if (PtInRect(&obTile[i].rc, _ptMouse))
					{
						obTile[i].keyName = user.KeyName;
						obTile[i].kind = user.kind;
						obTile[i].uKind = user.uKind;
					}
				}
				break;
			case TOOL::ERASE:
				for (int i = 0; i < MAXTILE; i++)
				{
					if (obTile[i].uKind != UNIT_KIND::NONE)continue;

					if (PtInRect(&obTile[i].rc, _ptMouse))
					{
						obTile[i].keyName = "";
						obTile[i].kind = TERRAIN::NONE;
						obTile[i].uKind = UNIT_KIND::NONE;
					}
				}
				break;
			}
			break;
		}
	}

	if (isLeft && option != OPTION::SELECT_MENU)
	{
		switch (tool)
		{
		case TOOL::DRAW:
			if (user.kind != TERRAIN::WALL)break;

			if (user.KeyName == "bottomWall" || user.KeyName == "topWall")break; // fix

			if (user.KeyName != "wall0" && user.KeyName != "wall1" && user.KeyName != "wall2" &&
				user.KeyName != "wallTile5" && user.KeyName != "wallTile6" && user.KeyName != "wallTile7" &&
				user.KeyName != "wallTile8" && user.KeyName != "wallTile9" && user.KeyName != "wallTile10") break;//wallTile 5 - 10

			if (!dragButton.isCol)
			{
				for (int i = 0; i < MAXTILE; i++)
				{
					if (maptool.isCol)continue;

					if (PtInRect(&tile[i].rc, _ptMouse))
					{
						//if (tile[i].kind == TERRAIN::WALL)continue;

						tile[i].keyName = user.KeyName;
						tile[i].kind = user.kind;
						tile[i].frame.x = RANDOM->range(5);
						tile[i].frame.y = 0;
					}
				}
			}
			break;
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
					if (user.kind == TERRAIN::WALL)continue;

					if (tile[i].kind == TERRAIN::OBJECT || tile[i].kind == TERRAIN::WALL)continue;

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
					int height = abs((cul.back() / MAXTILE_HEIGHT) - (cul.front() / MAXTILE_HEIGHT)) + 1; // 가로 x칸
					int width = (cul.back() - ((height - 1) * MAXTILE_WIDTH)) - cul.front() + 1; // 세로 y칸

					if (tile[cul.front()].kind != TERRAIN::OBJECT)
					{
						tile[cul.front()].frame = { 0,0 }; // 왼쪽 위
					}

					for (int i = 1; i < width - 1; i++)
					{
						if (tile[cul.front() + i].kind == TERRAIN::OBJECT)continue;

						tile[cul.front() + i].frame = { 1,0 }; // 위
					}

					if (tile[cul.front() + (width - 1)].kind != TERRAIN::OBJECT)
					{
						tile[cul.front() + (width - 1)].frame = { 2,0 }; // 오른쪽 위
					}

					for (int i = 1; i < height - 1; i++)
					{
						if (tile[cul.back() - (MAXTILE_HEIGHT * i)].kind == TERRAIN::OBJECT)continue;

						tile[cul.back() - (MAXTILE_HEIGHT * i)].frame = { 2,1 }; // 오른쪽
					}

					if (tile[cul.back()].kind != TERRAIN::OBJECT)
					{
						tile[cul.back()].frame = { 2,2 }; // 오른쪽 아래
					}

					for (int i = 1; i < width - 1; i++)
					{
						if (tile[cul.back() - i].kind == TERRAIN::OBJECT)continue;

						tile[cul.back() - i].frame = { 1,2 }; // 아래
					}

					tile[cul.back() - (width - 1)].frame = { 0,2 }; // 왼쪽 아래

					for (int i = 1; i < height - 1; i++)
					{
						if (tile[cul.front() + MAXTILE_HEIGHT * i].kind == TERRAIN::OBJECT)continue;

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