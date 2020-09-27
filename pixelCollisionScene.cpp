#include "stdafx.h"
#include "pixelCollisionScene.h"

//
//HRESULT pixelCollisionScene::init()
//{
//	//백그라운드, 공이미지 초기화
//	//bgImg = IMAGEMANAGER->addImage("mapBlock", "map/mapBlock.bmp", 5543, 4624);
//	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
//
//	_ball = IMAGEMANAGER->addImage("ball", "Images/player.bmp", 32, 64, true, RGB(255, 0, 255));
//
//	/*_mountain = IMAGEMANAGER->findImage("mountain");
//	_ball = IMAGEMANAGER->findImage("ball");*/
//	//bgImg->getX() - bgImg->getWidth()/2,bgImg->getY() - bgImg->getHeight()/2
//	//bgImg->setX(-bgImg->getWidth() / 2 + 670);
//	//bgImg->setY(-bgImg->getHeight() / 2 + 790);
//
//	//공 위치 초기화
//	_x = WINSIZEX / 2;
//	_y = WINSIZEY / 2;
//	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());
//
//	//Y축으로 탐지(공의 하단에서 검사함)
//	_probeY = _y + _ball->getHeight() / 2;
//
//
//	//타일_로드 맵
//	//char str[50];
//	//sprintf(str, "mapData/map%d.map", 3);
//	loadMap("mapData/map2.map");
//
//	//없애도됨
//	cam = RectMake(0, 0, WINSIZEX, WINSIZEY);
//	isBottomCol = isTopCol = false;
//
//	CAMERAMANAGER->init(_x, _y, 150*150, 150*150, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
//	return S_OK;
//}
//
//void pixelCollisionScene::release()
//{
//}
//
//void pixelCollisionScene::update()
//{
//	if (INPUT->GetKey(VK_LEFT))
//	{
//		_x -= 3.0f;
//		//bgImg->setX(bgImg->getX() + 5);
//	}
//	if (INPUT->GetKey(VK_RIGHT))
//	{
//		_x += 3.0f;
//		//bgImg->setX(bgImg->getX() - 5);
//	}
//	if (INPUT->GetKey(VK_DOWN))
//	{
//		_y += 3.0f;
//		//bgImg->setY(bgImg->getY() - 5);
//	}
//	if (INPUT->GetKey(VK_UP))
//	{
//		_y -= 3.0f;
//		//bgImg->setY(bgImg->getY() + 5);
//	}
//	//공의 렉트 움직이기
//	_rc = RectMakeCenter(_x, _y, 60, 60);
//
//	if (CAMERAMANAGER->getRect().left > CAMERAMANAGER->getMinX() &&
//		CAMERAMANAGER->getRect().top > CAMERAMANAGER->getMinY())
//	{
//		cam = RectMakeCenter(_x, _y, WINSIZEX, WINSIZEY);
//	}
//
//	//if (_x - WINSIZEX/2 <= CAMERAMANAGER->getMinX() && _y - WINSIZEY/2 <= CAMERAMANAGER->getMinY()) 
//	//CAMERAMANAGER->RectangleMake(getMemDC(), cam.left, cam.right, WINSIZEX, WINSIZEY);
//	//CAMERAMANAGER->RectangleMake(getMemDC(),_x, _y, 60, 60);
//
//
//	CAMERAMANAGER->MovePivot(_x, _y);
//}
//
//void pixelCollisionScene::render()
//{
//	//sRender.push_back()
//	//백그라운드 렌더
//	//bgImg->render(getMemDC(), bgImg->getX(), bgImg->getY());
//	//Rectangle(getMemDC(), CAMERAMANAGER->getRect());
//
//	//CAMERAMANAGER->Rectangle(getMemDC(), CAMERAMANAGER->getRect());
//	//로드된 타일 렌더
//	for (int i = 0; i < MAXTILE; i++)
//	{
//		if (colCheck(cam, tile[i].rc) == false || tile[i].keyName == "") continue;
//
//		image* img = IMAGEMANAGER->findImage(tile[i].keyName);
//		
//		//if (img == nullptr)cout << "null" << '\n';
//		//else cout << "Not Null" << '\n';
//
//		CAMERAMANAGER->FrameRender(getMemDC(), img, tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
//		//IMAGEMANAGER->frameRender(tile[i].keyName, getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
//	}
//
//	uiImg->render(getMemDC());
//
//	////공 이미지 렌더
//	//_ball->render(getMemDC(), _rc.left + 15, _rc.top);
//
//	CAMERAMANAGER->Render(getMemDC(), _ball, _rc.left, _rc.top);
//
//	//CAMERAMANAGER->Rectangle(getMemDC(), cam);
//
//	//디버깅용
//	if (INPUT->GetToggleKey('A'))
//	{
//		//충돌용 작은 사각형
//		RECT rc = RectMakeCenter(_x, _y + 15, 10, 10);
//		Ellipse(getMemDC(), rc);
//
//		for (int i = 0; i < 4; i++) Rectangle(getMemDC(), pixelCheck[i].rc);
//	}
//
//	textOut(getMemDC(), 10, 10, "Ingame");
//
//	textOut(getMemDC(), 10, 20, "", RGB(0, 0, 255));
//	char tmp[126];
//	sprintf(tmp, "x : %.0f, y : %.0f", _x, _y);
//	TextOut(getMemDC(), 10, 40, tmp, strlen(tmp));
//}
//
//void pixelCollisionScene::loadMap(const char* mapFileName)
//{
//	HANDLE file;
//	DWORD read;
//
//	file = CreateFile(mapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
//	CloseHandle(file);
//}
