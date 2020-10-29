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
	PORTAL->init();
	addImage();
	EFFECT->init();

	_shop = new shop;
	_shop->addImage();

	_player = new player;
	_player->init();
	_player->setDungeonPotal(false);
	//리스폰위치
	_player->setX(2266);
	_player->setY(2936);

	image* img = IMAGEMANAGER->findImage("lobbyRoom");

	CAMERAMANAGER->init(_player->getX(), _player->getY(), img->getWidth() * 2, img->getHeight() * 2,
		-img->getWidth(), -img->getHeight(), WINSIZEX / 2, WINSIZEY / 2);

	//포탈위치
	rc = RectMakeCenter(2266, 222, 100, 100);

	//아이템 저장용
	string itemName[MAXSTARTITEM] = { "힘의 조각",
		"힘법사의 도끼",
		"오거파워건틀릿",
		"철갑옷",
		"뼈갑옷",
		"황금갑옷",
		"낡은 단검",
		"마법사의 단검",
		"황금 단검",
		"저격수의 고글",
		"톱니바퀴",
		"집중의 로브",
		"활력의 심장",
		"미다스의 손",
		"에메랄드",
		"보험증서",
		"절대자의 로브",
		"마력의 근원" };


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

	UI->fadeIn();

	EFFECT->setPortalEffect({ (LONG)_player->getX(), (LONG)_player->getY() });

	//setting
	sceneWarp = false;
	sceneCount = 0;
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
		SCENEMANAGER->loadScene("로딩화면");
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

	//sceneWarpStone
	image* img = IMAGEMANAGER->findImage("sceneWarpStone");
	CAMERAMANAGER->Render(getMemDC(), img,
		2266 - img->getWidth() / 2,
		240 - img->getHeight() / 2);
	//

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
	EFFECT->portalRender(getMemDC());
	UI->render(getMemDC(), 50, 50);

	itemInfo();

	_player->invenRender();

	//viewText();
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
	if (colCheck(_player->getRect(), rc) && INPUT->GetKeyDown('F') && !sceneWarp)
	{
		sceneWarp = true;

		image* img = IMAGEMANAGER->findImage("sceneWarpStone");

		EFFECT->setPortalEffect({ 2266, 217 });
	}

	if (sceneWarp)
	{
		sceneCount++;

		if (sceneCount == 17)
		{
			SCENEMANAGER->loadScene("로딩화면");
			sceneCount = 0;
		}
	}
}

void lobbyScene::itemInfo()
{
	image* back = IMAGEMANAGER->findImage("itemBackBoardFrame");
	RECT rt;
	int frameX = 0;
	for (int j = 0; j < MAXSTARTITEM; j++)
	{
		//정보 출력
		if (colCheck(startingItem[j].rc, _player->getRect()) && shopActive[j / 3])
		{
			int centerX = getRcCenterX(startingItem[j].rc);
			int centerY = getRcCenterY(startingItem[j].rc);

			rt = RectMakeCenter(
				CAMERAMANAGER->GetRelativeX(centerX),
				CAMERAMANAGER->GetRelativeY(centerY),
				280, 70);

			int exLength = startingItem[j].item.Explanation.size();

			//==========================================================
			if (exLength > 69) frameX = 2;

			else if (exLength > 34) frameX = 1;

			CAMERAMANAGER->AlphaFrameRender(getMemDC(), back,
				centerX - back->getFrameWidth() / 2,
				centerY - 10 - back->getFrameHeight() / 2,
				frameX, 0, 130);
			//==========================================================

			UI->setActiveButton2(true);
			UI->FbuttonRender2(getMemDC(),
				{ centerX,
				centerY + 13 });

			TextOut(getMemDC(), CAMERAMANAGER->GetRelativeX(centerX) - 40,
				CAMERAMANAGER->GetRelativeY(centerY) - 70,
				startingItem[j].item.keyName.c_str(), startingItem[j].item.keyName.length());

			const char* ch = startingItem[j].item.Explanation.c_str();

			DrawText(getMemDC(), ch, -1, &rt, DT_CENTER | DT_WORDBREAK);
		}

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

			//if (startingItem[i].isPurchased == true) cout << "이미 구매한 아이템 이라 무료입니다 !" << '\n';
			//else cout << "처음 보는 아이템 입니다 !" << '\n';

			//
			//buy item

			//1. 여기에 player stat을 조정
			PLAYERDATA->setStat(startingItem[i].item.keyName);

			//2. vector<string> vInven에 push_Back

			tagItem _item = ITEM->getItem(startingItem[i].item.keyName);
			PLAYERDATA->pushInven(_item);

			//3. 아이템의 가격만큼 보석을 잃음
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