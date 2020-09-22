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

	//BGM_Sound
	SOUNDMANAGER->addSound("titleBGM", "Sound/TitleScreen.mp3", true, true);
	volume = 0.5f;
	SOUNDMANAGER->play("titleBGM",volume);


	/*씬추가*/
	SCENEMANAGER->addScene("시작화면", new startScene);
	SCENEMANAGER->addScene("INI테스트", new iniTestScene);
	//SCENEMANAGER->addScene("사운드", new soundTestScene);
	SCENEMANAGER->addScene("픽셀충돌", new pixelCollisionScene);
	SCENEMANAGER->addScene("로딩화면", new loadingScene);

	mapToolScene* maptool = new mapToolScene;
	SCENEMANAGER->addScene("맵툴제작", maptool);
	maptool->addImage();
	/*현재씬*/
	SCENEMANAGER->loadScene("시작화면");

	ShowCursor(false);
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
	gameNode::update();
	
	//씬매니져 업데이트
	SCENEMANAGER->update();

	//사운드매니져 업데이트 (이게 없으면 사운드매니져 제대로 동작하지 않는다!!!)
	SOUNDMANAGER->update();

	if (INPUT->GetKeyDown(VK_ADD))
	{
		if (volume < 1.f)
		{
			volume += .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
		}
	}
	if (INPUT->GetKeyDown(VK_SUBTRACT))
	{
		if (volume > .0f)
		{
			volume -= .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
		}
	}
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
	IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 32, _ptMouse.y - 32);

	
//=============================================================
	//백버퍼의 내용을 화면DC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}
