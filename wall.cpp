#include "stdafx.h"
#include "wall.h"
#include "player.h"

HRESULT wall::init()
{
	isLoad = false;
	loadMap(0);

	//culPt
	culPt = { 0,0 };
	POINT ptZero = subTile[0].pos;
	POINT ptCam = { 0,0 };

	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(*cam, subTile[i].rc) == false)continue;
		ptCam = subTile[i].pos;
		break;
	}

	culPt = { ptCam.x - ptZero.x, ptCam.y - ptZero.y };

	PLAYERDATA->setTile(tile);

	int count = 0;
	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].kind != TERRAIN::WALL)continue;

		if (tile[i].keyName != "wallFrame0" && tile[i].keyName != "wallFrame1" &&
			tile[i].keyName != "wallFrame2" && tile[i].keyName != "wallFrame3" &&
			tile[i].keyName != "wall0" && tile[i].keyName != "wall1" &&
			tile[i].keyName != "wall2")
			continue;

		UNITRENDER->addUnit(i, tile[i].keyName, "wall", tile[i].frame);
		count++;
	}

	num = 0;
	size = 0;
	isFrameRect = false;

	return S_OK;
}

void wall::loadMap(int index)
{
	if (isLoad) return;
	char str[50], str1[50];

	sprintf(str, "mapData/map%d.map", index);
	sprintf(str1, "mapData/object%d.map", index);

	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);

	file = CreateFile(str1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, subTile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
	isLoad = true;
}

void wall::update()
{
	this->collisionTile();
}

void wall::render()
{
	string key;
	image* img;
	int width, height;
	for (int f = 0; f < vTile.size(); f++)
	{
		int i = vTile[f];

		if (tile[i].kind != TERRAIN::WALL && tile[i].kind != TERRAIN::OBJECT)continue;

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

			if (key == "wallFrame0" || key == "wallFrame1" || key == "wallFrame2" || key == "wallFrame3")
			{
				image* img = IMAGEMANAGER->findImage(tile[i].keyName);

				CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
			}

			break;
		//case TERRAIN::OBJECT:
		//	// 2X2인 오브젝트만 처리

		//	//i object, i+1 object, i+w object, i+w+1 object
		//	if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
		//		tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

		//	key = tile[i].keyName;

		//	if (!(tile[i + 1].keyName == key &&
		//		tile[i + MAXTILE_WIDTH].keyName == key &&
		//		tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

		//	if (tile[i].keyName == "flowerbed1")
		//	{
		//		key = "tree0";
		//		width = 2 * TILESIZE;
		//		height = 4 * TILESIZE;

		//		img = IMAGEMANAGER->findImage(key);

		//		CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
		//	}
		//	else if (tile[i].keyName == "flowerbed2")
		//	{
		//		key = "tree1";
		//		width = 2 * TILESIZE;
		//		height = 5 * TILESIZE;

		//		img = IMAGEMANAGER->findImage(key);

		//		CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left - width, tile[i].rc.top - height);
		//	}
		//	else if (tile[i].keyName == "pillar1")
		//	{
		//		key = "pillar1";

		//		height = 3 * TILESIZE;

		//		img = IMAGEMANAGER->findImage(key);

		//		CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
		//	}
		//	break;
		}
	}

	//cout << count << '\n';
}

void wall::render2()
{
	size = vTile.size();

	for (int i = 0; i < size; i++)
	{
		num = vTile[i];

		if (tile[num].kind == TERRAIN::NONE || tile[num].kind != TERRAIN::TILE)continue;

		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(tile[num].keyName), tile[num].rc.left, tile[num].rc.top, tile[num].frame.x, tile[num].frame.y);
		
		if (isFrameRect)
		{
			RECT tmp = tile[num].rc;

			tmp.left = CAMERAMANAGER->GetRelativeX(tmp.left);
			tmp.right = CAMERAMANAGER->GetRelativeX(tmp.right);
			tmp.top = CAMERAMANAGER->GetRelativeY(tmp.top);
			tmp.bottom = CAMERAMANAGER->GetRelativeY(tmp.bottom);

			FrameRect(getMemDC(), tmp, RGB(255, 0, 0));
		}
	}

	//object image render
	for (int i = 0; i < size; i++)
	{
		num = vTile[i];

		if (subTile[num].kind != TERRAIN::DECO) continue;
		
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage(subTile[num].keyName), tile[num].rc.left, tile[num].rc.top);
	}
}

void wall::collisionTile()
{
	vTile.clear();
	vTile.shrink_to_fit();
	vWall.clear();
	vWall.shrink_to_fit();
	
	size = MAXTILE;
	int i;
	for (i = 0; i < size; i++)
	{
		if (colCheck(*cam, tile[i].rc))
		{
			vTile.push_back(i);

			if (tile[i].kind == TERRAIN::WALL)vWall.push_back(i);
		}
	}
	
	_player->setTileAd0(vTile);
	PLAYERDATA->setTile(vTile);
	_player->setTileAd1(vWall);
	PLAYERDATA->setWall(vWall);
}