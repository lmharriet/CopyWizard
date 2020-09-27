#include "stdafx.h"
#include "pixelCollisionScene.h"


HRESULT pixelCollisionScene::init()
{
	//��׶���, ���̹��� �ʱ�ȭ
	//bgImg = IMAGEMANAGER->addImage("mapBlock", "map/mapBlock.bmp", 5543, 4624);
	uiImg = IMAGEMANAGER->addImage("UI", "Images/gameUI.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_ball = IMAGEMANAGER->addImage("ball", "Images/player.bmp", 32, 64, true, RGB(255, 0, 255));

	/*_mountain = IMAGEMANAGER->findImage("mountain");
	_ball = IMAGEMANAGER->findImage("ball");*/
	//bgImg->getX() - bgImg->getWidth()/2,bgImg->getY() - bgImg->getHeight()/2
	//bgImg->setX(-bgImg->getWidth() / 2 + 670);
	//bgImg->setY(-bgImg->getHeight() / 2 + 790);

	//�� ��ġ �ʱ�ȭ
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//Y������ Ž��(���� �ϴܿ��� �˻���)
	_probeY = _y + _ball->getHeight() / 2;


	//Ÿ��_�ε� ��
	//char str[50];
	//sprintf(str, "mapData/map%d.map", 3);
	loadMap("mapData/map2.map");

	//���ֵ���
	cam = RectMake(0, 0, WINSIZEX, WINSIZEY);
	isBottomCol = isTopCol = false;


	POINT temp[4] = { {-10,0},{10,0},{10,30},{-10,30} };
	for (int i = 0; i < 4; i++)
	{
		pixelCheck[i].pos = temp[i];
		pixelCheck[i].rc = RectMakeCenter(_x + temp[i].x, _y + temp[i].y, 10, 10);
		pixelCheck[i].isCol = false;
	}

	return S_OK;
}

void pixelCollisionScene::release()
{
}

void pixelCollisionScene::update()
{
	if (INPUT->GetKey(VK_LEFT) &&
		(!pixelCheck[0].isCol || !pixelCheck[3].isCol))
	{
		//_x -= 3.0f;
		//bgImg->setX(bgImg->getX() + 5);
	}
	if (INPUT->GetKey(VK_RIGHT) &&
		(!pixelCheck[1].isCol || !pixelCheck[2].isCol))
	{
		//_x += 3.0f;
		//bgImg->setX(bgImg->getX() - 5);
	}
	if (INPUT->GetKey(VK_DOWN) &&
		(!pixelCheck[2].isCol || !pixelCheck[3].isCol))
	{
		//_y += 3.0f;
		//bgImg->setY(bgImg->getY() - 5);
	}
	if (INPUT->GetKey(VK_UP) &&
		(!pixelCheck[0].isCol || !pixelCheck[1].isCol))
	{
		//_y -= 3.0f;
		//bgImg->setY(bgImg->getY() + 5);
	}
	//���� ��Ʈ �����̱�
	_rc = RectMakeCenter(_x, _y, 60, 60);

	//for (int i = 0; i < 4; i++)
	//{
	//	int tempX = abs(bgImg->getX()) + WINSIZEX / 2;
	//	int tempY = abs(bgImg->getY()) + WINSIZEY / 2;

	//	COLORREF color = GetPixel(bgImg->getMemDC(), tempX + pixelCheck[i].pos.x, tempY + pixelCheck[i].pos.y);
	//	int r = GetRValue(color);
	//	int g = GetGValue(color);
	//	int b = GetBValue(color);

	//	if (r == 255 && g == 0 && b == 255)
	//	{
	//		pixelCheck[i].isCol = true;
	//	}
	//	else
	//	{
	//		pixelCheck[i].isCol = false;
	//	}
	//}
}

void pixelCollisionScene::render()
{
	//sRender.push_back()
	//��׶��� ����
	//bgImg->render(getMemDC(), bgImg->getX(), bgImg->getY());

	uiImg->render(getMemDC());
	//�ε�� Ÿ�� ����
	for (int i = 0; i < MAXTILE; i++)
	{
		IMAGEMANAGER->frameRender(tile[i].keyName, getMemDC(), tile[i].rc.left, tile[i].rc.top, tile[i].frame.x, tile[i].frame.y);
	}

	////�� �̹��� ����
	_ball->render(getMemDC(), _rc.left + 15, _rc.top);

	////object image render
	//for (int i = 0; i < MAXTILE; i++)
	//{
	//	if (!colCheck(tile[i].rc, cam) || tile[i].kind != TERRAIN::OBJECT)continue;

	//	// 2X2�� ������Ʈ�� ó��

	//	//i object, i+1 object, i+w object, i+w+1 object
	//	if (!(tile[i + 1].kind == TERRAIN::OBJECT && tile[i + MAXTILE_WIDTH].kind == TERRAIN::OBJECT &&
	//		tile[i + MAXTILE_WIDTH + 1].kind == TERRAIN::OBJECT)) continue;

	//	string key = tile[i].keyName;

	//	if (!(tile[i + 1].keyName == key &&
	//		tile[i + MAXTILE_WIDTH].keyName == key &&
	//		tile[i + MAXTILE_WIDTH + 1].keyName == key)) continue;

	//	int width, height;
	//	if (tile[i].keyName == "flowerbed1")
	//	{
	//		key = "tree0";
	//		width = 2 * TILESIZE;
	//		height = 4 * TILESIZE;

	//		IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height);
	//	}
	//	else if (tile[i].keyName == "flowerbed2")
	//	{
	//		key = "tree1";
	//		width = 2 * TILESIZE;
	//		height = 5 * TILESIZE;

	//		IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left - width, tile[i].rc.top - height);
	//	}
	//	else if (tile[i].keyName == "pillar1")
	//	{
	//		key = "pillar1";

	//		height = 3 * TILESIZE;

	//		IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
	//	}
	//}

	//for (int i = 0; i < MAXTILE; i++)
	//{
	//	if (tile[i].kind != TERRAIN::OBJECT) continue;

	//	// 1x? �̰ų� �ٸ� Ư���� ������Ʈ ó��

	//	string key;
	//	int height;
	//	if (tile[i].keyName == "pillar0") // ���� : 1
	//	{
	//		key = "pillar0";

	//		height = 3 * TILESIZE;

	//		IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
	//	}
	//	else if (tile[i].keyName == "bossDoor")
	//	{

	//		if (tile[i + 5].keyName != "bossDoor")continue;

	//		key = "bossDoor";

	//		height = 7 * TILESIZE;

	//		IMAGEMANAGER->findImage(key)->render(getMemDC(), tile[i].rc.left, tile[i].rc.top - height);
	//	}
	//}
	//

	//������
	if (INPUT->GetToggleKey('A'))
	{
		//�浹�� ���� �簢��
		RECT rc = RectMakeCenter(_x, _y + 15, 10, 10);
		Ellipse(getMemDC(), rc);

		for (int i = 0; i < 4; i++) Rectangle(getMemDC(), pixelCheck[i].rc);
	}

	textOut(getMemDC(), 10, 10, "Ingame");

	textOut(getMemDC(), 10, 20, "", RGB(0, 0, 255));
	char tmp[126];
	for (int i = 0; i < 4; i++)
	{
		pixelCheck[i].isCol;
		wsprintf(tmp, "pixelCheck : %d", pixelCheck[i].isCol);
		TextOut(getMemDC(), 10, 50 + i * 20, tmp, strlen(tmp));
	}
}

void pixelCollisionScene::loadMap(const char* mapFileName)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(mapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile, sizeof(tagTile) * MAXTILE, &read, NULL);
	CloseHandle(file);
}
