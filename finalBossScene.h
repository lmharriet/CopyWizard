#pragma once
#include "gameNode.h"
#include "finalBoss.h"
#include "player.h"
#define MAXWIDTH 2048
#define MAXHEIGHT 2048
class finalBossScene : public gameNode
{
public:
	//finalBossScene();
private:
	RECT rc;
	RECT area;

	bool isBattle;
	bool mapChange;
	int cutTime;
	int mapCount;

	player* _player;
	finalBoss* _finalBoss;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void bossCutScene();
};

