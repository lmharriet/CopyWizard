#include "stdafx.h"
#include "pixelCollisionScene.h"


HRESULT pixelCollisionScene::init()
{
	//��׶���, ���̹��� �ʱ�ȭ
	//_mountain = IMAGEMANAGER->addImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY,true,RGB(255,0,255));
	_ball = IMAGEMANAGER->addImage("ball", "Images/ball.bmp", 60, 60,true, RGB(255, 0, 255));

	/*_mountain = IMAGEMANAGER->findImage("mountain");
	_ball = IMAGEMANAGER->findImage("ball");*/


	//�� ��ġ �ʱ�ȭ
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//Y������ Ž��(���� �ϴܿ��� �˻���)
	_probeY = _y + _ball->getHeight() / 2;


	//Ÿ��_�ε� ��
	char str[50];
	sprintf(str, "mapData/map%d.map", RANDOM->range(3));
	loadMap(str);

	//���ֵ���
	cam = RectMake(0, 0, WINSIZEX, WINSIZEY);

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
	//���� ��Ʈ �����̱�
	_rc = RectMakeCenter(_x, _y, 60, 60);
	_probeY = _y + _ball->getHeight() / 2;

	/*�̺κ��� �ȼ��浹�� �ٽ�*/
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
	//��׶��� ����
	//_mountain->render(getMemDC());

	//�ε�� Ÿ�� ����
	for (int i = 0; i < MAXTILE; i++)
	{
		IMAGEMANAGER->frameRender(tile[i].keyName, getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	}

	//�� �̹��� ����
	_ball->render(getMemDC(), _rc.left, _rc.top);

	//object image render
	for (int i = 0; i < MAXTILE; i++)
	{
		if (!colCheck(tile[i].rc, cam) || tile[i].kind != TERRAIN::OBJECT)continue;

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
	

	//������
	if (INPUT->GetToggleKey('A'))
	{
		//�浹�� ���� �簢��
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
