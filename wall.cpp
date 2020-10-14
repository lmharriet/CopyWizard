#include "stdafx.h"
#include "wall.h"
#include "player.h"

HRESULT wall::init()
{
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
	cout << count << '\n';

	return S_OK;
}

void wall::loadMap(int index)
{
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
	for (int i = 0; i < vTile.size(); i++)
	{
		int num = vTile[i];

		if (tile[num].keyName == "" || tile[num].kind != TERRAIN::TILE)continue;

		image* img = IMAGEMANAGER->findImage(tile[num].keyName);

		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[num].rc.left, tile[num].rc.top, tile[num].frame.x, tile[num].frame.y);
	}

	//string key;
	//image* img;
	//int width, height;
	//for (int f = 0; f < vTile.size(); f++)
	//{
	//	int i = vTile[f];

	//	if (tile[i].kind != TERRAIN::WALL && tile[i].kind != TERRAIN::OBJECT)continue;

	//	int w = MAXTILE_WIDTH;
	//	int w2 = MAXTILE_WIDTH * 2;
	//	int w3 = MAXTILE_WIDTH * 3;
	//	int w4 = MAXTILE_WIDTH * 4;
	//	int w5 = MAXTILE_WIDTH * 5;
	//	int w6 = MAXTILE_WIDTH * 6;

	//	switch (tile[i].kind)
	//	{
	//	case TERRAIN::WALL:
	//		key = tile[i].keyName;

	//		if (key == "topWall") // fix
	//		{
	//			if ((key == tile[i + 1].keyName &&
	//				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
	//				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
	//				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
	//				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&

	//				key == tile[i - w3 + 2].keyName && key == tile[i - w3 + 3].keyName &&
	//				key == tile[i - w3 + 4].keyName && key == tile[i - w3 + 5].keyName &&
	//				key == tile[i - w4 + 2].keyName && key == tile[i - w4 + 3].keyName &&
	//				key == tile[i - w4 + 4].keyName && key == tile[i - w4 + 5].keyName &&

	//				key == tile[i - w4 + 10].keyName && key == tile[i - w4 + 11].keyName &&
	//				key == tile[i - w4 + 12].keyName && key == tile[i - w4 + 13].keyName &&
	//				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&

	//				key == tile[i - w3 + 10].keyName && key == tile[i - w3 + 11].keyName &&
	//				key == tile[i - w3 + 12].keyName && key == tile[i - w3 + 13].keyName &&
	//				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&

	//				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
	//				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
	//				key == tile[i + 14].keyName && key == tile[i + 15].keyName)
	//				== false)
	//			{
	//				continue;
	//			}

	//			height = 8 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
	//		}

	//		else if (key == "bottomWall") // fix
	//		{
	//			if ((key == tile[i + 1].keyName &&
	//				key == tile[i - w].keyName && key == tile[i - w + 1].keyName &&
	//				key == tile[i - w2].keyName && key == tile[i - w2 + 1].keyName &&
	//				key == tile[i - w3].keyName && key == tile[i - w3 + 1].keyName &&
	//				key == tile[i - w4].keyName && key == tile[i - w4 + 1].keyName &&
	//				key == tile[i - w5].keyName && key == tile[i - w5 + 1].keyName &&
	//				key == tile[i - w6].keyName && key == tile[i - w6 + 1].keyName &&

	//				key == tile[i - w + 2].keyName && key == tile[i - w + 3].keyName &&
	//				key == tile[i - w + 4].keyName &&
	//				key == tile[i + 2].keyName && key == tile[i + 3].keyName &&
	//				key == tile[i + 4].keyName &&


	//				key == tile[i - w + 11].keyName && key == tile[i - w + 12].keyName &&
	//				key == tile[i - w + 13].keyName &&
	//				key == tile[i + 11].keyName && key == tile[i + 12].keyName &&
	//				key == tile[i + 13].keyName &&

	//				key == tile[i + 14].keyName && key == tile[i + 15].keyName &&
	//				key == tile[i - w + 14].keyName && key == tile[i - w + 15].keyName &&
	//				key == tile[i - w2 + 14].keyName && key == tile[i - w2 + 15].keyName &&
	//				key == tile[i - w3 + 14].keyName && key == tile[i - w3 + 15].keyName &&
	//				key == tile[i - w4 + 14].keyName && key == tile[i - w4 + 15].keyName &&
	//				key == tile[i - w5 + 14].keyName && key == tile[i - w5 + 15].keyName &&
	//				key == tile[i - w6 + 14].keyName && key == tile[i - w6 + 15].keyName
	//				) == false)
	//			{
	//				continue;
	//			}
	//			height = 8 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->Render(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height);
	//		}

	//		//if (key == "wallFrame0" || key == "wallFrame1" || key == "wallFrame2" || key == "wallFrame3")
	//		//{
	//		//	image* img = IMAGEMANAGER->findImage(tile[i].keyName);

	//		//	CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	//		//}

	//		else if (key == "wall0" || key == "wall1" || key == "wall2")
	//		{
	//			height = 3 * TILESIZE;

	//			img = IMAGEMANAGER->findImage(key);

	//			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top - height, tile[i].frame.x, tile[i].frame.y);
	//		}

	//		break;
	//	}
	//}

	//cout << vWall.size() << '\n';

	//index, keyName 
}

void wall::collisionTile()
{
	vTile.clear();
	vWall.clear();
	for (int i = 0; i < MAXTILE; i++)
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