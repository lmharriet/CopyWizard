#pragma once
#include "gameNode.h"
#include "finalBoss.h"
#include "player.h"
#include "chest.h"
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
	chest* _chest;

	RECT boxRc;
	int boxFrameX;
	int endCutTime;
	bool isEnd;

	POINT topBox;
	POINT bottomBox;
	int saveY;

	//player
	POINT pFrame;

	//sound
	bool isBossBGM;
	float fadeIn;
public:
	HRESULT init();
	void release();
	void update();
	void endCutScene();
	void render();
	void bossCutScene();
	void attackSura();
};

