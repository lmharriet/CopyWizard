#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ ������ϸ� �̰��� �߰��Ѵ�*/
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
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void volumeControl();
	void soundInit();

};