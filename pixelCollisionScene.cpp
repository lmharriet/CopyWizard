#include "stdafx.h"
#include "pixelCollisionScene.h"


HRESULT pixelCollisionScene::init()
{
	//백그라운드, 공이미지 초기화
	//_mountain = IMAGEMANAGER->addImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY,true,RGB(255,0,255));
	_ball = IMAGEMANAGER->addImage("ball", "Images/ball.bmp", 60, 60,true, RGB(255, 0, 255));

	/*_mountain = IMAGEMANAGER->findImage("mountain");
	_ball = IMAGEMANAGER->findImage("ball");*/


	//공 위치 초기화
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//Y축으로 탐지(공의 하단에서 검사함)
	_probeY = _y + _ball->getHeight() / 2;


	//타일_로드 맵
	char str[50];
	sprintf(str, "mapData/map%d.map", RANDOM->range(3));
	loadMap(str);

	return S_OK;
}

void pixelCollisionScene::release()
{
}

void pixelCollisionScene::update()
{
	if (INPUT->GetKey(VK_LEFT))
	{
		_x -= 3.0f;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		_x += 3.0f;
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		_y += 3.0f;
	}
	if (INPUT->GetKey(VK_UP))
	{
		_y -= 3.0f;
	}
	//공의 렉트 움직이기
	_rc = RectMakeCenter(_x, _y, 60, 60);
	_probeY = _y + _ball->getHeight() / 2;

	/*이부분이 픽셀충돌의 핵심*/
	//for (int i = _probeY - 50; i < _probeY + 50; i++)
	//{
	//	COLORREF color = GetPixel(_mountain->getMemDC(), _x, i);
	//	int r = GetRValue(color);
	//	int g = GetGValue(color);
	//	int b = GetBValue(color);

	//	//if (r == 255 && g == 0 && b == 255)
	//	//{
	//	//
	//	//}

	//	if (!(r == 255 && g == 0 && b == 255))
	//	{
	//		_y = i - _ball->getHeight() / 2;
	//		break;
	//	}
	//}


}

void pixelCollisionScene::render()
{
	//sRender.push_back()
	//백그라운드 렌더
	//_mountain->render(getMemDC());
	
	//공 이미지 렌더
	_ball->render(getMemDC(), _rc.left, _rc.top);

	//로드된 타일 렌더
	for (int i = 0; i < MAXTILE; i++)
	{
		IMAGEMANAGER->frameRender(tile[i].keyName, getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	}

	//objcet img render
	for (int i = 0; i < MAXTILE; i++)
	{
		if (tile[i].kind != TERRAIN::OBJECT)continue;

		//i object, i+1 object, i+w object, i+w+1 object
		if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
			tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

		string key = tile[i].keyName;

		if (!(tile[i + 1].keyName == key &&
			tile[i + MAXTILE_WIDTH].keyName == key &&
			tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

		IMAGEMANAGER->findImage("tree0")->render(getMemDC(), tile[i].rc.left - (2 * TILESIZE), tile[i].rc.top - (4 * TILESIZE));
	}
	

	//디버깅용
	if (INPUT->GetToggleKey('A'))
	{
		//충돌용 작은 사각형
		RECT rc = RectMakeCenter(_x, _probeY, 10, 10);
		Rectangle(getMemDC(), rc);
	}

	textOut(getMemDC(), 10, 10, "Ingame");
}

void pixelCollisionScene::loadMap(const char* mapFileName)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(mapFileName,GENERIC_READ,0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}
