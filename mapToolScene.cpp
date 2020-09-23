#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	initTile();

	initSelectTerrain();

	initMaptool();

	initButton();

	initDrag();

	initUser();

	initSaveAndLoad();


	//option = OPTION::SELECT_MENU;
	option = OPTION::OBJECT_MENU;
	isLeftDown = isLeft = isLeftUp = false;

	pageNum = 0;


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

	//SELECT MENU���� ������(��, ���찳 ���..)�� �����ϴ�
	if (option != OPTION::SELECT_MENU) iconCheck();

	//delay�� �־� �޴��� �ٲ� �� �ٷ� ���� �Ǵ� ���� ����
	if (user.delay < 10) user.delay++;

	//���콺�� ����ui�� ������ draw,erase �� �۾��� ��Ȱ��ȭ
	if (PtInRect(&maptool.rc, _ptMouse))
	{
		maptool.isCol = true;

		//user�� ui�ȿ� ������ �巡�� ��Ȱ��ȭ
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

	// ���̺� 
	if (isSave)
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&fileWin[i], _ptMouse) && isLeftDown)
			{
				mapSave(i);
				isSave = false;
			}
		}

	// �ε�
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
	//Ÿ�� �̹��� �׸���
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(tile[i].rc, cam.rc) || tile[i].kind == TERRAIN::NONE)continue;

		if (tile[i].keyName != "") imageFrameRender(tile[i].keyName, { tile[i].rc.left,tile[i].rc.top }, tile[i].frame.x, tile[i].frame.y);
	}
	objectImgRender();
	//Ÿ�� �׸���
	tileRender();
	//�ɼ��� ���¿� ���� UI�̹��� ����
	UIRender();

	rcRender();

	checkRender();

	//�޴� ȭ�� �ƴϰ� user�� ������ ������ �귯�� ��Ȱ��ȭ
	if (option != OPTION::SELECT_MENU && user.KeyName == "" && user.kind == TERRAIN::NONE)
		imageAlphaRender("active", { icon[1].left,icon[1].top + 5 });

	//userRender
	if (user.kind != TERRAIN::NONE)
	{
		imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 }, 200);
	}

	textRender();

	FrameRect(getMemDC(), drag.rc, WHITE);

	//���̺� & �ε� ���� & image ����
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
	//����� �޴� �̵�, ���̺�, �ε� �� ó��

	// 0 :  wall  //
	// 1 :  tile  //
	// 2 : object //

	//option�� MENU �� ��

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
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("����ȭ��");
		this->saveCheck();
		this->loadCheck();
		break;
	case OPTION::WALL_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			//������ �������ִ� ������ �ʱ�ȭ
			user.KeyName = "";
			user.kind = TERRAIN::NONE;
			option = OPTION::SELECT_MENU;

			//�ٽ� ������ �� icon�� �̸� Ȱ��ȭ �Ǿ������� ����
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

	//����� ������ �Է¸� ó��

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
		wall[i].rc = RectMake(929 + (i % 2) * 175, 140 + (i / 2) * 162, 165, 117);
		wall[i].kind = TERRAIN::WALL;
		wall[i].keyName = wallName[i];
	}

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
	string decoName[9] = { "grass4","grass3","grass2","tomb0","tomb1","flower","window0","window1", "flag0" };
	for (int i = 0; i < 9; i++)
	{
		deco[i].rc = RectMake(941 + (i % 3)* 120, 170 + (i / 3) * 105, 90, 75);
		deco[i].keyName = decoName[i];
		deco[i].kind = TERRAIN::IMG;
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
	IMAGEMANAGER->addFrameImage("wall0", "maptool/wall/wall0.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall1", "maptool/wall/wall1.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wall2", "maptool/wall/wall2.bmp", 160, 128, 5, 4, false);
	IMAGEMANAGER->addFrameImage("wallTile", "maptool/wall/wallTile.bmp", 32, 32, 1, 1, false);

	//TILE//
	IMAGEMANAGER->addFrameImage("grass0", "maptool/tile/grass0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("grass1", "maptool/tile/grass1.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("grass2", "maptool/tile/grass2.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("ground0", "maptool/tile/ground0.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("ground1", "maptool/tile/ground1.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);
	IMAGEMANAGER->addFrameImage("ground2", "maptool/tile/ground2.bmp", TILESIZE * 3, TILESIZE * 3, 3, 3, false);

	//OBJECT//
	IMAGEMANAGER->addFrameImage("flowerbed1", "maptool/object/flowerbed1.bmp", TILESIZE * 2, TILESIZE * 2, 2, 2, false);
	IMAGEMANAGER->addFrameImage("flowerbed2", "maptool/object/flowerbed1.bmp", TILESIZE * 2, TILESIZE * 2, 2, 2, false);
	IMAGEMANAGER->addImage("tree0", "maptool/object/tree0.bmp", TILESIZE * 6, TILESIZE * 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tree1", "maptool/object/tree1.bmp", TILESIZE * 6, TILESIZE * 7, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bossDoor", "maptool/object/bossDoor-col.bmp", TILESIZE * 6, TILESIZE * 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("pillar0", "maptool/object/pillar0.bmp", TILESIZE * 1, TILESIZE * 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("pillar1", "maptool/object/pillar1.bmp", TILESIZE * 2, TILESIZE * 5, true, RGB(255, 0, 255));


	//DECO//
	IMAGEMANAGER->addImage("grass4", "maptool/deco/grass4.bmp", 50, 26, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("grass3", "maptool/deco/grass3.bmp", 56, 44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("grass2", "maptool/deco/grass2.bmp", 64, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tomb0", "maptool/deco/tomb0.bmp", 116, 84, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("tomb1", "maptool/deco/tomb1.bmp", 116, 80, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("flower", "maptool/deco/flower.bmp", 70, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("window0", "maptool/deco/window0.bmp", 82, 74, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("window1", "maptool/deco/window1.bmp", 82, 74, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("flag0", "maptool/deco/flag0.bmp", 82, 74, true, RGB(255, 0, 255));


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
			//Ÿ�Ͽ� ������ ���� �� �ʷϻ� ���� ���
			if (tile[i].kind == TERRAIN::NONE)
			{
				FrameRect(getMemDC(), tile[i].rc, DARKGREEN);
				//char num[10];
				//textOut(getMemDC(), tile[i].rc.left, tile[i].rc.top, itoa(i, num, 10), GREEN);
			}

			//if (tile[i].keyName != "") imageFrameRender(tile[i].keyName, { tile[i].rc.left,tile[i].rc.top }, tile[i].frame.x, tile[i].frame.y);

			if (tile[i].kind != TERRAIN::NONE)
			{
				switch (tile[i].kind)
				{
				case TERRAIN::TILE:
					FrameRect(getMemDC(), tile[i].rc, CLOUDYBLUE);
					break;
				case TERRAIN::WALL:
					FrameRect(getMemDC(), tile[i].rc, YELLOW);
					break;
				case TERRAIN::IMG:
					FrameRect(getMemDC(), tile[i].rc, ORANGE);
					break;
				case TERRAIN::OBJECT:
					FrameRect(getMemDC(), tile[i].rc, PINK);
					break;
				}
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

		//obeject page �ѱ��
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

	if (option != OPTION::SELECT_MENU)
	{
		for (int i = 0; i < 2; i++)Rectangle(getMemDC(), pageButton[i]);
	}
}

void mapToolScene::rcRender()
{
	//button
	if (INPUT->GetToggleKey(VK_TAB)) // ��Ʈ�� ���� �ִ� �� �� ����
	{
		buttonRender();
		FrameRect(getMemDC(), maptool.rc, WHITE);

		switch (option)
		{
		case OPTION::SELECT_MENU:
			for (int i = 0; i < 3; i++) Rectangle(getMemDC(), mapOption[i]);
			break;
		case OPTION::WALL_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);
			Rectangle(getMemDC(), dragButton.rc);
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), wall[i].rc);
			break;
		case OPTION::TILE_MENU:
			for (int i = 0; i < 4; i++)	Rectangle(getMemDC(), icon[i]);
			for (int i = 0; i < 6; i++)Rectangle(getMemDC(), bigTile[i].rc);

			Rectangle(getMemDC(), dragButton.rc);
			break;
		case OPTION::OBJECT_MENU:
			for (int i = 0; i < 4; i++) Rectangle(getMemDC(), icon[i]);

			for (int i = 0; i < 5; i++) Rectangle(getMemDC(), object[i].rc);

			for (int i = 0; i < 9; i++) Rectangle(getMemDC(), deco[i].rc);
			Rectangle(getMemDC(), dragButton.rc);
			break;
		}
	}
}

void mapToolScene::checkRender()
{
	//�޴����� �ɼǹ�ư�� ����
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
		if (!colCheck(tile[i].rc, cam.rc) || tile[i].kind != TERRAIN::OBJECT)continue;

		// 2X2�� ������Ʈ�� ó��

		//i object, i+1 object, i+w object, i+w+1 object
		if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
			tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

		string key = tile[i].keyName;

		if (!(tile[i + 1].keyName == key &&
			tile[i + MAXTILE_WIDTH].keyName == key &&
			tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

		int width, height;
		if (tile[i].keyName == "flowerbed1")
		{
			key = "tree0";
			width = 2 * TILESIZE;
			height = 4 * TILESIZE;

			IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height);
		}
		else if (tile[i].keyName == "flowerbed2")
		{
			key = "tree1";
			width = 2 * TILESIZE;
			height = 5 * TILESIZE;

			IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height);
		}
		else if (tile[i].keyName == "pillar1")
		{
			key = "pillar1";

			height = 3 * TILESIZE;

			IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
		}
	}

	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].kind != TERRAIN::OBJECT) continue;

		// 1x? �̰ų� �ٸ� Ư���� ������Ʈ ó��

		string key;
		int height;
		if (tile[i].keyName == "pillar0") // ���� : 1
		{
			key = "pillar0";

			height = 3 * TILESIZE;

			IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
		}
		else if (tile[i].keyName == "bossDoor")
		{

			if (tile[i + 5].keyName != "bossDoor")continue;

			key = "bossDoor";

			height = 7 * TILESIZE;

			IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
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
	if (isLeftDown) // Ÿ���� ������ �������� ��ɸ� ����
	{
		switch (option)
		{
		case OPTION::WALL_MENU:
			//get wall
			for (int i = 0; i < 4; i++)
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
					//user�� ui �ȿ� �ְų�, Ÿ�� ������ ������ �����̵� ��� ��Ȱ��ȭ
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
					// �귯�� �����ܿ� check ǥ��
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
									//�����Ϸ��� ������ Ÿ���� IMG�̰ų�, WALL�� �� �������� �ʵ��� CONTINUE
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
				case TOOL::ERASE: // �Ʒ��� isLeft �� �� ó��
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
						// �귯�� �����ܿ� check ǥ��
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

					if (PtInRect(&tile[i].rc, _ptMouse))
					{

						if (user.KeyName == "pillar0") //���ΰ� 1
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
							//2,2 �� ��
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
				break;
			}
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

						//ù��ǥ ��ġ tile[i], drag.start
						//��������ǥ ��ġ tile[i], drag.end

						//cout << "�浹�� �κ� : " << i << '\n';
						cul.push_back(i);

						tile[i].frame = { 1,1 };
					}
				}
				if (cul.size() > 4)
				{
					//����, ���� ���ϱ�
					int height = abs((cul.back() / 100) - (cul.front() / 100)) + 1; // ���� xĭ
					int width = (cul.back() - ((height - 1) * 100)) - cul.front() + 1; // ���� yĭ

					if (tile[cul.front()].kind != TERRAIN::OBJECT)
					{
						tile[cul.front()].frame = { 0,0 }; // ���� ��
					}

					for (int i = 1; i < width - 1; i++)
					{
						if (tile[cul.front() + i].kind == TERRAIN::OBJECT)continue;

						tile[cul.front() + i].frame = { 1,0 }; // ��
					}

					if (tile[cul.front() + (width - 1)].kind != TERRAIN::OBJECT)
					{
						tile[cul.front() + (width - 1)].frame = { 2,0 }; // ������ ��
					}

					for (int i = 1; i < height - 1; i++)
					{
						if (tile[cul.back() - (MAXTILE_HEIGHT * i)].kind == TERRAIN::OBJECT)continue;

						tile[cul.back() - (MAXTILE_HEIGHT * i)].frame = { 2,1 }; // ������
					}

					if (tile[cul.back()].kind != TERRAIN::OBJECT)
					{
						tile[cul.back()].frame = { 2,2 }; // ������ �Ʒ�
					}

					for (int i = 1; i < width - 1; i++)
					{
						if (tile[cul.back() - i].kind == TERRAIN::OBJECT)continue;

						tile[cul.back() - i].frame = { 1,2 }; // �Ʒ�
					}

					tile[cul.back() - (width - 1)].frame = { 0,2 }; // ���� �Ʒ�

					for (int i = 1; i < height - 1; i++)
					{
						if (tile[cul.front() + MAXTILE_HEIGHT * i].kind == TERRAIN::OBJECT)continue;

						tile[cul.front() + (MAXTILE_HEIGHT * i)].frame = { 0,1 }; // ����
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