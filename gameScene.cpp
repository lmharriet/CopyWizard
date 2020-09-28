#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{

	_player = new player;
	_player->init();

	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	playerImg = IMAGEMANAGER->findImage("playerFrame");

	loadMap("mapData/map2.map");

	cam = RectMakeCenter(0, 0, WINSIZEX, WINSIZEY);
	CAMERAMANAGER->init(_player->getX(), _player->getY(), MAXTILE, MAXTILE, -MAXTILE, -MAXTILE, WINSIZEX / 2, WINSIZEY / 2);

	pixelCheck[0].rc = RectMakeCenter(_player->getX() - 30, _player->getY() - 60, 10, 10);
	pixelCheck[1].rc = RectMakeCenter(_player->getX() + 30, _player->getY() - 60, 10, 10);
	pixelCheck[2].rc = RectMakeCenter(_player->getX() - 30, _player->getY() + 60, 10, 10);
	pixelCheck[3].rc = RectMakeCenter(_player->getX() + 30, _player->getY() + 60, 10, 10);

	for (int i = 0; i < 4; i++)
	{
		pixelCheck[i].isCol = false;
	}

	return S_OK;
}

void gameScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void gameScene::update()
{
	_player->update();
	cam = RectMakeCenter(_player->getX(), _player->getY(), WINSIZEX, WINSIZEY);

	//for (int i = 0; i < MAXTILE; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		if (colCheck(tile[i].rc, pixelCheck[j].rc))
	//		{
	//			cout << i << "번 째 타일 이름 : " << tile[i].keyName << "." << '\n';
	//			if (tile[i].keyName != "") continue;

	//			pixelCheck[j].isCol = true;

	//			continue;
	//		}
	//	}
	//}

	for (int i = 0; i < 4; i++)
	{
		pixelCheck[i].isCol = false;
		for (int j = 0; j < MAXTILE; j++)
		{
			if (tile[j].keyName != "") continue;

			if (colCheck(pixelCheck[i].rc, tile[j].rc))
			{
				pixelCheck[i].isCol = true;
				break;
			}
		}
	}

	pixelCheck[0].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 60), 10, 10);
	pixelCheck[1].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 - 60), 10, 10);
	pixelCheck[2].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 - 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 60), 10, 10);
	pixelCheck[3].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2 + 30), CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2 + 60), 10, 10);
}

void gameScene::render()
{
	CAMERAMANAGER->Rectangle(getMemDC(), cam);
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false) continue;

		if (tile[i].keyName != "")
		{
			image* img = IMAGEMANAGER->findImage(tile[i].keyName);

			CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
		}
	}

	//CAMERAMANAGER->Rectangle(getMemDC(), _player->getRect());
	uiImg->render(getMemDC());

	_player->render();

	for (int i = 0; i < 4; i++)
	{
		CAMERAMANAGER->Rectangle(getMemDC(), pixelCheck[i].rc);
		//Rectangle(getMemDC(), pixelCheck[i].rc);
	}

	viewText();

}

void gameScene::loadMap(const char* mapFileName)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(mapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}

void gameScene::viewText()
{
	char text[126];

	for (int i = 0; i < 4; i++)
	{
		wsprintf(text, "pixelRc %d ,isCol =%d, pixelRc.left %d, pixelRc.top %d", i, pixelCheck[i].isCol, pixelCheck[i].rc.left, pixelCheck[i].rc.top);
		textOut(getMemDC(), 80, 50 + i * 20, text, BLACK);
	}
}