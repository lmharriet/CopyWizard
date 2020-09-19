#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	IMAGEMANAGER->addFrameImage("tile", "Images/tile.bmp", 144, 144, 3, 3);
	IMAGEMANAGER->addImage("tileSet", "Images/tile.bmp", 144, 144, true, RGB(255, 0, 255));

	//제작할곳
	for (int i = 0; i < MAXTILE_HEIGHT; i++)
	{
		for (int j = 0; j < MAXTILE_WIDTH; j++)
		{
			int order = MAXTILE_WIDTH * i + j;

			tile[order].rc = RectMake(200 + ((j % MAXTILE_WIDTH) * TILESIZE), 130 + ((i % MAXTILE_HEIGHT) * TILESIZE), TILESIZE, TILESIZE);
			tile[order].active = false;
			tile[order].isCol = false;
			tile[order].isCam = false;

			tile[order].frame = { 0,0 };
		}
	}
	
	//가져올곳 <단일>
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int order = 3 * i + j;

			sTile[order].frame = { j,i };

			sTile[order].rc = RectMake(1100 + (j * 55), 380+ (i * 55), TILESIZE, TILESIZE);
		}
	}

	//가져올곳 <세트>
	sTile_set.rc = RectMake(1107, 180, 144, 144);

	//mouse
	state = ERASE;
	memset(&mouse, 0, sizeof(mouse));

	//button down
	isLeftDown = isLeftkey = isLeftUp = false;

	//cam
	camX = camY = 0;
	camRC = RectMake(0, 0, WINSIZEX, WINSIZEY);

	//button
	dragChangeButton = RectMake(1102, 330, 150, 40);
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
	//cam
	if (INPUT->GetKey(VK_LEFT))camX += 3;
	if (INPUT->GetKey(VK_RIGHT))camX -= 3;
	if (INPUT->GetKey(VK_UP))camY += 3;
	if (INPUT->GetKey(VK_DOWN))camY -= 3;

	//제작할곳
	for (int i = 0; i < MAXTILE_HEIGHT; i++)
	{
		for (int j = 0; j < MAXTILE_WIDTH; j++)
		{
			int order = MAXTILE_WIDTH * i + j;

			tile[order].rc = moveRC(200 + ((j % MAXTILE_WIDTH) * TILESIZE), 130 + ((i % MAXTILE_HEIGHT) * TILESIZE), TILESIZE, TILESIZE);
		}
	}

	//tile set
	if(mouse.dragMode) dragTile();
	else singleTile();

	//tile get < SINGLE >
	if (isLeftDown)
	{
		for (int i = 0; i < MAXTILE_HEIGHT; i++)
		{
			if (PtInRect(&sTile[i].rc, _ptMouse))
			{
				mouse.frame = sTile[i].frame;
				//mouse.frame.x = sTile[i].frame.x;
				//mouse.frame.y = sTile[i].frame.y;
				state = DRAW;
				mouse.active = true;
			}
		}
	}
	//tile get < SET >
	if (isLeftDown)
	{
		if (PtInRect(&sTile_set.rc, _ptMouse))
		{
			state = DRAW_SET;
			mouse.active = true;
		}
	}

	//MOUSE.active = draw나 draw_set 상태일 때(true), erase 상태일 때(false)
	/*
	state로 draw,draw_set,erase 구분
	*/

	
	//MODE

	//erace mode
	if (INPUT->GetKeyDown('E'))
	{
		mouse.active = false;
	}

	//brush mode
	if (INPUT->GetKeyDown('B'))
	{
		mouse.active = true;
	}

	if (!mouse.active) state = ERASE;

	if (INPUT->GetKeyDown(VK_SPACE))init();

	if (INPUT->GetKeyDown('S')) save();
	if (INPUT->GetKeyDown('L')) load();

	//button
	if (INPUT->GetKeyDown(VK_LBUTTON)) isLeftDown = true;
	else isLeftDown = false;
	if (INPUT->GetKey(VK_LBUTTON))isLeftkey = true;
	else isLeftkey = false;
	if (INPUT->GetKeyUp(VK_LBUTTON))isLeftUp = true;
	else isLeftUp = false;

	//ui button
	if (isLeftDown && PtInRect(&dragChangeButton, _ptMouse))
	{
		if(mouse.dragMode) mouse.dragMode = false;
		else mouse.dragMode = true;
	}

	//cam col
	RECT tp;
	for (int i = 0; i < MAXTILE; i++)
	{
		if (IntersectRect(&tp, &tile[i].rc, &camRC))tile[i].isCam = true;
		else tile[i].isCam = false;
	}
}

void mapToolScene::render()
{
	//render
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!tile[i].active)
		{
			if (!tile[i].isCol)
			{
				if (tile[i].isCam) Rectangle(getMemDC(), tile[i].rc);
			}
			else
			{
				HPEN hPen, oPen;
				hPen = CreatePen(PS_SOLID, 2, GREEN);
				oPen = (HPEN)SelectObject(getMemDC(), hPen);
				if (tile[i].isCam) Rectangle(getMemDC(), tile[i].rc);
				
				SelectObject(getMemDC(), oPen);
				DeleteObject(hPen);
			}
		}
		else
		{
			if (!tile[i].isCol && tile[i].isCam) Rectangle(getMemDC(), tile[i].rc);

			if (tile[i].active && i < MAXTILE - MAXTILE_WIDTH && tile[i + MAXTILE_WIDTH].active)
			{
				if (checkPt(tile[i + MAXTILE_WIDTH].frame, { 0,1 }) && checkPt(tile[i].frame, { 1,0 }))
				{
					tile[i].frame = { 0,0 };
				}
				else if (checkPt(tile[i + MAXTILE_WIDTH].frame, { 2,1 }) && checkPt(tile[i].frame, { 1,0 }))
				{
					tile[i].frame = { 2,0 };
				}
				else IMAGEMANAGER->frameRender("tile", getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
			}
			if (tile[i].active && i > MAXTILE_WIDTH - 1 && tile[i - MAXTILE_WIDTH].active)
			{
				if (checkPt(tile[i - MAXTILE_WIDTH].frame, { 0,1 }) && checkPt(tile[i].frame, { 1,2 }))
				{
					tile[i].frame = { 0,2 };
				}
				else if (checkPt(tile[i - MAXTILE_WIDTH].frame, { 2,1 }) && checkPt(tile[i].frame, { 1,2 }))
				{
					tile[i].frame = { 2,2 };
				}
				else  IMAGEMANAGER->frameRender("tile", getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
			}
			else IMAGEMANAGER->frameRender("tile", getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);

			if (tile[i].isCol)
			{
				HBRUSH hBrush, oBrush;
				HPEN hPen, oPen;

				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				oBrush = (HBRUSH)SelectObject(getMemDC(), hBrush);
				SetBkMode(getMemDC(), TRANSPARENT);

				hPen = CreatePen(PS_SOLID, 4, RED);
				oPen = (HPEN)SelectObject(getMemDC(), hPen);
				if (tile[i].isCam) Rectangle(getMemDC(), tile[i].rc);

				SelectObject(getMemDC(), oPen);
				SelectObject(getMemDC(), oBrush);

				DeleteObject(hPen);
				DeleteObject(hBrush);
			}
		}
	}

	//mouse img
	if (mouse.active)
	{
		if(state == DRAW_SET) IMAGEMANAGER->render("tileSet", getMemDC(), _ptMouse.x, _ptMouse.y);
		else IMAGEMANAGER->frameRender("tile", getMemDC(), _ptMouse.x - 24, _ptMouse.y - 24, mouse.frame.x, mouse.frame.y);
	}

	//fremerender
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int order = 3 * i + j;

			//Rectangle(getMemDC(), sTile[order].rc);
			IMAGEMANAGER->frameRender("tile", getMemDC(), sTile[order].rc.left, sTile[order].rc.top,
				sTile[order].frame.x, sTile[order].frame.y);
		}
	}

	IMAGEMANAGER->render("tileSet", getMemDC(), sTile_set.rc.left, sTile_set.rc.top);

	//button
	Rectangle(getMemDC(), dragChangeButton);


	if (isLeftkey)
	{
		switch (state)
		{
		case DRAW:
			FrameRect(getMemDC(), mouse.rc, BLACK);
			break;
		case DRAW_SET:
			FrameRect(getMemDC(), mouse.rc, BLACK);
			break;
		}
	}	

	textRender(); //모드 확인용
}

void mapToolScene::save()
{
	//HANDLE file;
	//DWORD write;

	//file = CreateFile("mapData/save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//WriteFile(file, tile, sizeof(tagMap) * MAXTILE, &write, NULL);
	//CloseHandle(file);
}

void mapToolScene::load()
{
	//HANDLE file;
	//DWORD read;
	//
	//file = CreateFile("mapData/save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//ReadFile(file, tile, sizeof(tagMap) * MAXTILE, &read, NULL);
	//CloseHandle(file);
}

void mapToolScene::dragTile()
{
	if (isLeftDown)
	{
		mouse.start = _ptMouse;
	}
	if (isLeftkey)
	{
		mouse.end = _ptMouse;
	}

	if (mouse.start.y < mouse.end.y)
	{
		if (mouse.start.x < mouse.end.x) mouse.rc = RectMake(mouse.start.x, mouse.start.y, mouse.end.x - mouse.start.x, mouse.end.y - mouse.start.y);
		else mouse.rc = RectMake(mouse.end.x, mouse.start.y, mouse.start.x - mouse.end.x, mouse.end.y - mouse.start.y);
	}
	else
	{
		if (mouse.start.x > mouse.end.x) mouse.rc = RectMake(mouse.end.x, mouse.end.y, mouse.start.x - mouse.end.x, mouse.start.y - mouse.end.y);
		else mouse.rc = RectMake(mouse.start.x, mouse.end.y, mouse.end.x - mouse.start.x, mouse.start.y - mouse.end.y);
	}

	//VK_LBUTTON을 안눌렀을 때
	if(!isLeftkey) mouse.start = mouse.end = { 0,0 };

	//COL
	RECT temp;
	for (int i = 0; i < MAXTILE; i++)
	{
		if (IntersectRect(&temp, &mouse.rc, &tile[i].rc))
		{
			tile[i].isCol = true;
		}
		else tile[i].isCol = false;
	}
	if (isLeftUp)
	{
		for (int i = 0; i < MAXTILE; i++)
		{
			if (IntersectRect(&temp, &mouse.rc, &tile[i].rc))
			{
				switch (state)
				{
				case DRAW:
					tile[i].active = true;
					tile[i].frame = mouse.frame;
					break;
				case DRAW_SET:
					tile[i].active = true;

					if (!tile[i + 1].isCol)tile[i].frame = RIGHT_TILE;
					if (!tile[i - 1].isCol)tile[i].frame = LEFT_TILE;
					if (!tile[i - MAXTILE_WIDTH].isCol)tile[i].frame = TOP_TILE;
					if (!tile[i + MAXTILE_WIDTH].isCol)tile[i].frame = BOTTOM_TILE;

					if (tile[i + 1].isCol && tile[i - 1].isCol &&
						tile[i - MAXTILE_WIDTH].isCol && tile[i + MAXTILE_WIDTH].isCol)tile[i].frame = CENTER_TILE;

					break;
				case ERASE:
					if (mouse.active)continue;

					tile[i].active = false;
					break;
				}
			}
		}
	}
}

void mapToolScene::singleTile()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		if (PtInRect(&tile[i].rc, _ptMouse))
		{
			switch (state)
			{
			case DRAW:
				if (!isLeftkey)continue;

				tile[i].active = true;
				tile[i].frame = mouse.frame;
				break;
			case DRAW_SET:
				if (!isLeftDown)continue;

				tile[i].active = tile[i + 1].active = tile[i + 2].active = true;
				tile[i + MAXTILE_WIDTH].active = tile[i + MAXTILE_WIDTH+1].active = tile[i + MAXTILE_WIDTH+2].active = true;
				tile[i + MAXTILE_WIDTH*2].active = tile[i + MAXTILE_WIDTH * 2+1].active = tile[i + MAXTILE_WIDTH * 2+2].active = true;

				tile[i].frame = tile[i + 1].frame = tile[i + 2].frame = TOP_TILE;
				tile[i + MAXTILE_WIDTH].frame = LEFT_TILE;
				tile[i + MAXTILE_WIDTH + 1].frame = CENTER_TILE;
				tile[i + MAXTILE_WIDTH + 2].frame = RIGHT_TILE;
				tile[i + MAXTILE_WIDTH * 2].frame = tile[i + MAXTILE_WIDTH * 2 + 1].frame = tile[i + MAXTILE_WIDTH * 2 + 2].frame = BOTTOM_TILE;
				break;
			case ERASE:
				if (!isLeftkey || mouse.active)continue;

				tile[i].active = false;
				break;
			}
			break;
		}
	}
}

bool mapToolScene::checkPt(POINT pt, POINT _pt)
{
	if (pt.x == _pt.x && pt.y == _pt.y)return true;

	return false;
}

void mapToolScene::textRender()
{
	string str;

	switch (state)
	{
	case DRAW:
		str = "DRAW MODE";
		break;
	case DRAW_SET:
		str = "SET DRAW MODE";
		break;
	case ERASE:
		str = "ERASE MODE";
		break;
	}

	TextOut(getMemDC(), 10, 10, str.c_str(), strlen(str.c_str()));

	//cout << mouse.dragMode << '\n';
}