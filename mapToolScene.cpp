#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	initTile();
	initSelectTerrain();

	maptool.rc = RectMake(920, 0, WINSIZEX - 920, WINSIZEY);
	maptool.isCol = false;
		
	for (int i = 0; i < 3; i++)
	{
		mapOption[i] = RectMake(1020, 197 + (i * 83), 160, 60);
	}
	SAVE = RectMake(955, 563, 100, 45);
	LOAD = RectMake(955, 613, 100, 45);
	BACK = RectMake(955, 663, 100, 45);

	for (int i = 0; i < 4; i++)
	{
		icon[i] = RectMake(928, 138 + (i * 50), 50, 48);
	}

	//drag
	drag.start = drag.end = { 0,0 };

	dragButton.rc = RectMake(928, 87, 50, 48);
	dragButton.isCol = false;

	//user init
	user.KeyName = "";
	user.kind = TERRAIN::NONE;
	user.delay = 0;
	tool = TOOL::NONE;

	option = OPTION::SELECT_MENU;



	//���� & �ҷ����� â 
	for (int i = 0; i < 3; i++)
	{
		fileWin[i] = RectMake(125 + i * 250, 228, 200, 300);
	}

	isSave = isLoad = false;



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

	//SELECT MENU���� ������(��, ���찳 ���..)�� �����ϴ�
	if (option != OPTION::SELECT_MENU) iconCheck();

	//delay�� �־� �޴��� �ٲ� �� �ٷ� ���� �Ǵ� ���� ����
	if (user.delay < 10) user.delay++;

	//���콺�� ����ui�� ������ draw,erase �� �۾��� ��Ȱ��ȭ
	if (PtInRect(&maptool.rc, _ptMouse)) maptool.isCol = true;
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
	//Ÿ�� �׸���
	tileRender();

	//�ɼ��� ���¿� ���� UI�̹��� ����
	UIRender();

	rcRender();

	checkRender();


	//userRender
	if (user.kind != TERRAIN::NONE)
	{
		imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 }, 200);
	}

	textRender();

	FrameRect(getMemDC(), drag.rc, WHITE);


	//���̺� & �ε� ����
	if (isSave || isLoad)
	{
		for (int i = 0; i < 3; i++)
		{
			Rectangle(getMemDC(), fileWin[i]);
			textOut(getMemDC(), fileWin[i].left, fileWin[i].top, "����ī��", RGB(0, 0, 0));
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
					//tool = TOOL::DRAW;
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
					//tool = TOOL::DRAW;
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
					//tool = TOOL::DRAW;
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

	//WALL//
	IMAGEMANAGER->addFrameImage("wall0", "maptool/wall/wall0.bmp", 160, 128, 5, 4,false);
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
			//Ÿ�Ͽ� ������ ���� �� �ʷϻ� ���� ���
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
	if (INPUT->GetToggleKey(VK_TAB)) // ��Ʈ�� ���� �ִ� �� �� ����
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

	if (isLeftDown) // Ÿ���� ������ �������� ��ɸ� ����
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
							//tile[i].keyName = user.KeyName;
							//tile[i].kind = user.kind;

							//tile[i+1].keyName = user.keyName;
							//tile[i+1].kind = user.kind;....
							//���⼭ tile[i+1..i+maxtile_width * 2 + 2] ���� �ʱ�ȭ�� ������Ѵ�.

							////ù��° ��
							//tile[i].frame = { 0,0 };
							//tile[i + 1].frame = { 1,0 };
							//tile[i + 2].frame = { 2,0 };
							////�ι�° ��
							//tile[i + MAXTILE_WIDTH].frame = { 0,1 };
							//tile[i + MAXTILE_WIDTH + 1].frame = { 1,1 };
							//tile[i + MAXTILE_WIDTH + 2].frame = { 2,1 };
							////����° ��
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
				case TOOL::ERASE: // �Ʒ��� isLeft �� �� ó��
					break;
				case TOOL::SPOID:
					if (!dragButton.isCol && !maptool.isCol)
					{
						for (int i = 0; i < MAXTILE; i++)
						{
							if (tile[i].kind == TERRAIN::NONE)continue;

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
						tile[i].frame = { 0,0 };
					}
				}
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