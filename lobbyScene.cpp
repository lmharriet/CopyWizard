#include "stdafx.h"
#include "lobbyScene.h"

HRESULT lobbyScene::init()
{
	PARTICLE->init();
	UI->init();
	PLAYERDATA->init();
	PLAYERDATA->setHp(PLAYERDATA->getMaxHp());
	ITEM->init();
	DAMAGE->init();

	addImage();
	EFFECT->init();

	_player = new player;
	_player->init();
	_player->setDungeonPotal(false);
	//��������ġ
	_player->setX(2266);
	_player->setY(2936);

	image* img = IMAGEMANAGER->findImage("lobbyRoom");

	CAMERAMANAGER->init(_player->getX(), _player->getY(), img->getWidth() * 2, img->getHeight() * 2,
		-img->getWidth(), -img->getHeight(), WINSIZEX / 2, WINSIZEY / 2);

	//��Ż��ġ
	rc = RectMakeCenter(2266, 222, 100, 100);

	//������ �����
	string itemName[MAXSTARTITEM] = { "���� ����",
		"�������� ����",
		"�����Ŀ���Ʋ��",
		"ö����",
		"������",
		"Ȳ�ݰ���",
		"���� �ܰ�",
		"�������� �ܰ�",
		"Ȳ�� �ܰ�",
		"���ݼ��� ���",
		"��Ϲ���",
		"������ �κ�",
		"Ȱ���� ����",
		"�̴ٽ��� ��",
		"���޶���",
		"��������",
		"�������� �κ�",
		"������ �ٿ�" };


	ZeroMemory(startingItem, sizeof(startingItem));

	for (int i = 0; i < 9; i++)
	{
		int stoneRange = (i / 3) * 148;

		startingItem[i].rc = RectMakeCenter(stoneRange + 429 + (i * 123), 1515, 60, 60);
		startingItem[i].item = ITEM->getItem(itemName[i]);

		startingItem[i + 9].rc = RectMakeCenter(stoneRange + 2790 + (i * 123), 1515, 60, 60);
		startingItem[i + 9].item = ITEM->getItem(itemName[i + 9]);
	}

	//INI DATA LOAD
	char tmp[126];
	for (int i = 0; i < 18; i++)
	{
		char key[126] = "item";

		sprintf(tmp, "%d", i);

		strcat(key, tmp);

		startingItem[i].isPurchased = INIDATA->loadDataInteger("INI/START_ITEM", "Purchased List", key);

		cout << startingItem[i].isPurchased << '\n';
	}

	//
	memset(shopActive, 1, sizeof(shopActive));

	return S_OK;
}

void lobbyScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void lobbyScene::update()
{
	UI->update();
	CAMERAMANAGER->update();
	PLAYERDATA->update();
	_player->other_update();
	if (INPUT->GetKeyDown('J'))
	{
		SCENEMANAGER->loadScene("�ε�ȭ��");
	}

	image* img = IMAGEMANAGER->findImage("lobbyBack");
	_player->colorCheck(img);

	PARTICLE->explosionActive();
	PARTICLE->pointActive();

	buyStartingItem();
	warpToGameScene();
}

void lobbyScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("lobbyBack"), 0, 0);
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("lobbyRoom"), 0, 0);
	CAMERAMANAGER->Rectangle(getMemDC(), rc);

	image* img;
	for (int i = 0; i < MAXSTARTITEM; i++)
	{
		img = IMAGEMANAGER->findImage("itemFrame");

		if (shopActive[i / 3] == false)continue;

		CAMERAMANAGER->FrameRender(getMemDC(), img,
			getRcCenterX(startingItem[i].rc) - 20,
			getRcCenterY(startingItem[i].rc) - 125 - 20,
			startingItem[i].item.frame.x,
			startingItem[i].item.frame.y);

		//CAMERAMANAGER->Rectangle(getMemDC(), startingItem[i].rc);
	}
	EFFECT->dRender(getMemDC());
	EFFECT->pRender(getMemDC());
	_player->attackCircleRender();
	EFFECT->render(getMemDC());
	_player->render(1);
	PARTICLE->render(getMemDC());
	UI->render(getMemDC(), 50, 50);
	_player->invenRender();

	viewText();
}

void lobbyScene::addImage()
{
	IMAGEMANAGER->addImage("lobbyRoom", "resource/lobby/lobbyMap.bmp", 4500, 3150, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("lobbyBack", "resource/lobby/lobbyCol2.bmp", 4500, 3150, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
	IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
	IMAGEMANAGER->addFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
	IMAGEMANAGER->addFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
	IMAGEMANAGER->addFrameImage("playerSpearFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);

}

void lobbyScene::viewText()
{
	char str[126];
	sprintf(str, "%.f, %.f", _player->getX(), _player->getY());
	TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));

}

void lobbyScene::warpToGameScene()
{
	if (colCheck(_player->getRect(), rc) && INPUT->GetKeyDown('F'))
	{
		SCENEMANAGER->loadScene("�ε�ȭ��");
	}
}

void lobbyScene::buyStartingItem()
{
	char iniKeyName[60] = "item";
	char iniKeyNum[60];
	for (int i = 0; i < MAXSTARTITEM; i++)
	{
		//0,1,2
		if (shopActive[i / 3] == false)continue;

		if (colCheck(_player->getRect(), startingItem[i].rc) && INPUT->GetKeyDown('F'))
		{
			shopActive[i / 3] = false;

			//if (startingItem[i].isPurchased == true) cout << "�̹� ������ ������ �̶� �����Դϴ� !" << '\n';
			//else cout << "ó�� ���� ������ �Դϴ� !" << '\n';

			//
			//buy item

			//1. ���⿡ player stat�� ����
			PLAYERDATA->setStat(startingItem[i].item.keyName);

			//2. vector<string> vInven�� push_Back

			tagItem _item = ITEM->getItem(startingItem[i].item.keyName);
			PLAYERDATA->pushInven(_item);

			//3. �������� ���ݸ�ŭ ������ ����
			//

			//

			//item0,item1...

			sprintf(iniKeyNum, "%d", i);
			strcat(iniKeyName, iniKeyNum);

			cout << iniKeyName << '\n';

			INIDATA->addData("Purchased List", iniKeyName, "1");
			INIDATA->saveINI("INI/START_ITEM");
		}
	}
}