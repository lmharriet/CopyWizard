#include "stdafx.h"
#include "gameScene.h"

HRESULT gameScene::init()
{
	
	_player = new player;
	_player->init();

	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	playerImg = IMAGEMANAGER->findImage("playerFrame");
	//playerImg = IMAGEMANAGER->addImage("ball", "Images/player.bmp", 32, 64, true, RGB(255, 0, 255));


	_rc = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY(), playerImg->getWidth(), playerImg->getHeight());

	//Y축으로 탐지(공의 하단에서 검사함)
	//_probeY = _y + playerImg->getHeight() / 2;


	//타일_로드 맵
	//char str[50];
	//sprintf(str, "mapData/map%d.map", 3);
	loadMap("mapData/map2.map");

	//없애도됨
	cam = RectMake(0, 0, WINSIZEX, WINSIZEY);
	isBottomCol = isTopCol = false;

	CAMERAMANAGER->init(_x, _y, 150*150, 150*150, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
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

	if (CAMERAMANAGER->getRect().left > CAMERAMANAGER->getMinX() &&
		CAMERAMANAGER->getRect().top > CAMERAMANAGER->getMinY())
	{
		cam = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY(), WINSIZEX, WINSIZEY);
	}

	//if (_x - WINSIZEX/2 <= CAMERAMANAGER->getMinX() && _y - WINSIZEY/2 <= CAMERAMANAGER->getMinY()) 
	//CAMERAMANAGER->RectangleMake(getMemDC(), cam.left, cam.right, WINSIZEX, WINSIZEY);
	//CAMERAMANAGER->RectangleMake(getMemDC(),_x, _y, 60, 60);

	// camera가 따라가는 대상
	CAMERAMANAGER->MovePivot(_player->getPlayerX(), _player->getPlayerY());
}

void gameScene::render()
{
	for (int i = 0; i < MAXTILE; i++)
	{
		if (colCheck(cam, tile[i].rc) == false || tile[i].keyName == "") continue;

		image* img = IMAGEMANAGER->findImage(tile[i].keyName);

		//if (img == nullptr)cout << "null" << '\n';
		//else cout << "Not Null" << '\n';

		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
		//IMAGEMANAGER->frameRender(tile[i].keyName, getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	}

	//_player->render();

	uiImg->render(getMemDC());

	//CAMERAMANAGER->Render(getMemDC(),playerImg, _player->getPlayerRect().left, _player->getPlayerRect().top);
	CAMERAMANAGER->FrameRender(getMemDC(), playerImg, _player->getPlayerRect().left, _player->getPlayerRect().top, 0, 0);


	/*
	//디버깅용
	if (INPUT->GetToggleKey('A'))
	{
		//충돌용 작은 사각형
		RECT rc = RectMakeCenter(_x, _y + 15, 10, 10);
		Ellipse(getMemDC(), rc);

		for (int i = 0; i < 4; i++) Rectangle(getMemDC(), pixelCheck[i].rc);
	}

	textOut(getMemDC(), 10, 10, "Ingame");

	textOut(getMemDC(), 10, 20, "", RGB(0, 0, 255));
	char tmp[126];
	sprintf(tmp, "x : %.0f, y : %.0f", _x, _y);
	TextOut(getMemDC(), 10, 40, tmp, strlen(tmp));
	*/

		
}

void gameScene::loadMap(const char* mapFileName)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(mapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}
