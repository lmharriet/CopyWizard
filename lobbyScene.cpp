#include "stdafx.h"
#include "lobbyScene.h"

HRESULT lobbyScene::init()
{
	PARTICLE->init();
	UI->init();
	PLAYERDATA->resetInven();
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

	PLAYERDATA->getHp();

	image* img = IMAGEMANAGER->findImage("lobbyRoom");

	CAMERAMANAGER->init(_player->getX(), _player->getY(), img->getWidth() * 2, img->getHeight() * 2,
		-img->getWidth(), -img->getHeight(), WINSIZEX / 2, WINSIZEY / 2);

	//포탈위치
	rc = RectMakeCenter(2266, 222, 100, 100);

	//아이템 저장용
	string itemName[MAXSTARTITEM] = { "힘의 조각",
		"철갑옷",
		"낡은 단검",
		"힘법사의 도끼",
		"뼈갑옷",
		"마법사의 단검",
		"오거파워건틀릿",
		"황금갑옷",
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

	//사운드
	soundInit();
	return S_OK;
}

void lobbyScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void lobbyScene::update()
{

	//사운드
	if (isLobbyBGM)
	{
		SOUNDMANAGER->fadeIn("lobbyBGM", fadeIn);
		fadeIn += 0.002f;
		if (fadeIn >= SOUNDMANAGER->getVolumeBGM())
			isLobbyBGM = false;
	}

	UI->update();
	CAMERAMANAGER->update();
	PLAYERDATA->update();
	_player->other_update();
	if (INPUT->GetKeyDown('J'))
	{
		SOUNDMANAGER->stop("lobbyBGM");
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
	//CAMERAMANAGER->Rectangle(getMemDC(), rc);

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

	startingItemPrice();

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
	IMAGEMANAGER->addFrameImage("shopNumbers", "Images/npc/shopNumbers.bmp", 60, 10, 10, 1);
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
			SOUNDMANAGER->stop("lobbyBGM");
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
			SOUNDMANAGER->play("npcBuy", false);
			if (startingItem[i].isPurchased == false) {
				if (startingItem[i].item.price > PLAYERDATA->getGem()) {
					return;
				}
				else {
					PLAYERDATA->setGem(PLAYERDATA->getGem() - startingItem[i].item.price);
				}
			}

			shopActive[i / 3] = false;

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

void lobbyScene::startingItemPrice()
{
	for (int i = 0; i < 18; i++) {
		if (shopActive[i / 3]) {
			if (startingItem[i].item.price > 9) {
				int one = startingItem[i].item.price % 10;
				int ten = startingItem[i].item.price / 10;
				CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("shopNumbers"), startingItem[i].rc.left + 25, startingItem[i].rc.top - 32, ten, 0);
				CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("shopNumbers"), startingItem[i].rc.left + 35, startingItem[i].rc.top - 32, one, 0);
			}
			else {
				CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("shopNumbers"), startingItem[i].rc.left + 35, startingItem[i].rc.top - 32, startingItem[i].item.price, 0);
			}
		}
	}
}

void lobbyScene::soundInit()
{

	//플레이어 사운드
	SOUNDMANAGER->addSound("playerHit", "Sound/player/hit.mp3");
	SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/nomalDash.mp3");
	SOUNDMANAGER->addSound("playerfireDash", "Sound/player/fireDash.mp3");
	SOUNDMANAGER->addSound("playerFoot", "Sound/player/footstep.mp3");
	SOUNDMANAGER->addSound("blazeFire", "Sound/player/blaze_fire.mp3");
	SOUNDMANAGER->addSound("blazeExp", "Sound/player/blaze_exp.mp3");
	SOUNDMANAGER->addSound("RagingInfernoExp", "Sound/player/RagingInferno_exp.mp3");
	SOUNDMANAGER->addSound("RagingInfernoExpBoss", "Sound/player/RagingInferno_exp_boss.mp3");
	SOUNDMANAGER->addSound("RagingInfernoFire", "Sound/player/RagingInferno_fire.mp3");
	SOUNDMANAGER->addSound("meteoFire0", "Sound/player/meteo_fire0.mp3");
	SOUNDMANAGER->addSound("meteoFire1", "Sound/player/meteo_fire1.mp3");
	SOUNDMANAGER->addSound("meteoFire2", "Sound/player/meteo_fire2.mp3");
	SOUNDMANAGER->addSound("meteoFire3", "Sound/player/meteo_fire3.mp3");
	SOUNDMANAGER->addSound("meteoExp0", "Sound/player/meteo_full0.mp3");
	SOUNDMANAGER->addSound("meteoExp1", "Sound/player/meteo_full1.mp3");
	SOUNDMANAGER->addSound("meteoExp2", "Sound/player/meteo_full2.mp3");
	SOUNDMANAGER->addSound("meteoExp3", "Sound/player/meteo_full3.mp3");
	SOUNDMANAGER->addSound("fireDragon", "Sound/player/fireDragon.mp3");
	SOUNDMANAGER->addSound("fireDragonUpgrade", "Sound/player/fireDragonUpgrade.mp3");
	SOUNDMANAGER->addSound("iceSpearATK", "Sound/player/iceSpearATK.mp3");
	SOUNDMANAGER->addSound("iceSpearFire", "Sound/player/iceSpearFire.mp3");
	SOUNDMANAGER->addSound("iceSpearBreak", "Sound/player/iceSpearBreak.mp3");
	SOUNDMANAGER->addSound("iceSpearHit", "Sound/player/iceSpearHit.mp3");

	SOUNDMANAGER->addSound("invenOpen", "Sound/UI/inventoryON.mp3");
	SOUNDMANAGER->addSound("invenClose", "Sound/UI/inventoryOFF.mp3");
	SOUNDMANAGER->addSound("portalWarp", "Sound/portalWarp.mp3");
	SOUNDMANAGER->addSound("shieldON", "Sound/UI/shieldON.mp3");
	SOUNDMANAGER->addSound("shieldOFF", "Sound/UI/shieldOFF.mp3");
	SOUNDMANAGER->addSound("npcBuy", "Sound/UI/npcBuy02.mp3");

	isLobbyBGM = true;
	fadeIn = 0;

	SOUNDMANAGER->stop("titleBGM");
	SOUNDMANAGER->play("lobbyBGM",true);

}
