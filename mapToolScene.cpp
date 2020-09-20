#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	addImage();

	initTile();
	initSelecTile();

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

	//SELECT MENU���� ������(��, ���찳 ���..)�� �����ϴ�
	if (option != OPTION::SELECT_MENU) iconCheck();
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
	if (user.kind != TERRAIN::NONE) imageAlphaRender(user.KeyName, { _ptMouse.x - TILESIZE / 2,_ptMouse.y - TILESIZE / 2 }, 200);

	textRender();
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
				option = (OPTION)i;
			}
		}
		if (PtInRect(&BACK, _ptMouse) && isLeftDown) SCENEMANAGER->loadScene("����ȭ��");
		break;
	case OPTION::WALL_MENU:
		if (PtInRect(&BACK, _ptMouse) && isLeftDown)
		{
			option = OPTION::SELECT_MENU;
			//�ٽ� ������ �� icon�� �̸� Ȱ��ȭ �Ǿ������� ����
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

	//����� ������ �Է¸� ó��

	switch (option)
	{
	case OPTION::WALL_MENU:
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
	}
}

void mapToolScene::initTile()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		tile[i].rc = RectMake((i % MAXTILE_WIDTH * TILESIZE), (i / MAXTILE_HEIGHT) * TILESIZE, TILESIZE, TILESIZE);
		tile[i].kind = TERRAIN::NONE;
		tile[i].keyName = "";
	}
}

void mapToolScene::initSelecTile()
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
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("wallMenu", "maptool/ui/wallmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("tileMenu", "maptool/ui/tilemenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("objectMenu", "maptool/ui/objectmenu.bmp", 360, 720);
	IMAGEMANAGER->addImage("checkIcon", "maptool/ui/check.bmp", 36, 36, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("grass0", "maptool/tile/grass0.bmp", TILESIZE * 3, TILESIZE * 3);
	IMAGEMANAGER->addImage("ground0", "maptool/tile/ground0.bmp", TILESIZE * 3, TILESIZE * 3);
}

void mapToolScene::tileRender()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(tile[i].rc, cam.rc))
		{
			FrameRect(getMemDC(), tile[i].rc, DARKGREEN);
		}
	}

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
	if (INPUT->GetToggleKey(VK_TAB)) // ��Ʈ�� ���� �ִ� �� �� ����
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
	//�޴����� �ɼǹ�ư�� ����
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

	if (isLeftDown)
	{
		for (int i = 0; i < 2; i++)
		{
			if (PtInRect(&bigTile[i].rc, _ptMouse))
			{
				user.KeyName = bigTile[i].keyName;
				user.kind = bigTile[i].kind;
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