#pragma once
#include "gameNode.h"
/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
/*씬들의 헤더파일만 이곳에 추가한다*/
#include "startScene.h"
#include "gameScene.h"
#include "mapToolScene.h"
#include "bossScene.h"
#include "loadingScene.h"

class mainGame : public gameNode
{
private:
	float volume;
	float sfxVolume;
	bool isMute;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void volumeControl();
	void soundInit();

};