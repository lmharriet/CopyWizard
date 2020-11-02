#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	IMAGEMANAGER->addImage("cursor", "wizard/cursor.bmp", 64, 64, true, RGB(255, 0, 255));

	/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
	/*이곳에서 각각의 씬들을 추가하고 현재씬을 설정한다*/

	soundInit();

	
	/*씬추가*/

	SCENEMANAGER->addScene("로딩화면", make_shared<loadingScene>());
	SCENEMANAGER->addScene("시작화면", make_shared <startScene>());
	SCENEMANAGER->addScene("로비화면", make_shared<lobbyScene>());
	SCENEMANAGER->addScene("인게임", make_shared<gameScene>());
	SCENEMANAGER->addScene("보스방", make_shared<bossScene>());
	SCENEMANAGER->addScene("최종보스방", make_shared<finalBossScene>());

	shared_ptr<mapToolScene> maptool = make_shared <mapToolScene>();
	SCENEMANAGER->addScene("맵툴제작", maptool);
	maptool->init();
	maptool->addImage();
	/*현재씬*/

	//SCENEMANAGER->loadScene("인게임");
	SCENEMANAGER->loadScene("시작화면");
	//SCENEMANAGER->loadScene("로비화면");
	//SCENEMANAGER->loadScene("맵툴제작");
	//SCENEMANAGER->loadScene("보스방");
	//SCENEMANAGER->loadScene("최종보스방");

	ShowCursor(false);
	time = 0;
	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	
	gameNode::release();
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	//gameNode::update();
	time++;
	//if (INPUT->GetKeyDown(VK_SPACE))TIME->setTest(12.f);

	//볼륨 컨트롤 
	this->volumeControl();
	//씬매니져 업데이트
	if (time % (int)TIME->getTest() == 0)
	{
		gameNode::update();
		SCENEMANAGER->update();
	}

	//사운드매니져 업데이트 (이게 없으면 사운드매니져 제대로 동작하지 않는다!!!)
	SOUNDMANAGER->update();

	if(INPUT->GetKeyDown(0x31)) // 1
		SCENEMANAGER->loadScene("시작화면");
	if(INPUT->GetKeyDown(0x32)) // 2
		SCENEMANAGER->loadScene("맵툴제작");
	if(INPUT->GetKeyDown(0x33)) // 3
		SCENEMANAGER->loadScene("인게임");
	if(INPUT->GetKeyDown(0x34)) // 4
		SCENEMANAGER->loadScene("보스방");


	//if (INPUT->GetKey(VK_SPACE) && (INT)TIME->getTest() > 1)
	//{
	//	TIME->setTest(-TIME->Lerp(0, TIME->getTest(), TIME->deltaTime()));
	//	cout << (INT)TIME->getTest() << '\n';
	//	//TIME->Lerp()
	//}
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵 (이것은 렌더에 그냥 두기)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	
	//글자색 배경 없애기
	SetBkMode(getMemDC(), TRANSPARENT);

	//씬매니져 렌더
	SCENEMANAGER->render();
	TIME->render(getMemDC());

	IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 32, _ptMouse.y - 32);

	
//=============================================================
	//백버퍼의 내용을 화면DC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}

void mainGame::volumeControl()
{
	if (INPUT->GetKeyDown(VK_ADD) || INPUT->GetKeyDown(VK_OEM_PLUS))
	{
		if (volume < 1.f)
		{
			volume += .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("lobbyBGM", volume);
		}
	}
	if (INPUT->GetKeyDown(VK_SUBTRACT) || INPUT->GetKeyDown(VK_OEM_MINUS))
	{
		if (volume > .0f)
		{
			volume -= .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("lobbyBGM", volume);
		}
	}

	if (INPUT->GetKeyDown(VK_OEM_4))//down
	{
		if (sfxVolume > -1.0f)
			sfxVolume -= 0.05f;
		SOUNDMANAGER->setVolumeSFX(sfxVolume);
	}
	if(INPUT->GetKeyDown(VK_OEM_6))//up
	{
		if (sfxVolume < 1.0f)
			sfxVolume += 0.05f;
		SOUNDMANAGER->setVolumeSFX(sfxVolume);
	}

	if (INPUT->GetKeyDown(VK_OEM_3)) //mute
	{
		if (!isMute)
		{
			SOUNDMANAGER->setVolumeSFX(-1.0f);
			SOUNDMANAGER->setBackGroundVolume("titleBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("lobbyBGM", -1.f);
			
			isMute = true;
		}
		else
		{
			SOUNDMANAGER->setVolumeSFX(sfxVolume);
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("lobbyBGM", volume);
			
			isMute = false;
		}

	}
	
}

void mainGame::soundInit()
{
	
	//BGM_Sound
	SOUNDMANAGER->addSound("titleBGM", "Sound/TitleScreen.mp3", true, true);
	SOUNDMANAGER->addSound("mapToolBGM", "Sound/MapToolBGM.mp3", true, true);
	SOUNDMANAGER->addSound("ingameBGM", "Sound/IngameBGM.mp3", true, true);
	SOUNDMANAGER->addSound("bossBGM", "Sound/BossBGM.mp3", true, true);
	SOUNDMANAGER->addSound("lobbyBGM", "Sound/PlayerRoom.mp3", true, true);
	
	volume = 0.5f;
	
	
	

	sfxVolume = 0.5f;
	isMute = false;
}
