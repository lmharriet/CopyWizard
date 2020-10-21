#pragma once
#include "gameNode.h"
#include "player.h"
#include "boss.h"
#include "chest.h"
#define MAXWIDTH 1500
#define MAXHEIGHT 989
class bossScene : public gameNode
{
public:
	bossScene();
private:
	RECT rc;
	RECT area;

	bool isBattle;
	int cutTime;

	player* _player;
	boss* _boss;

	chest* _chest;
	//»ç¿îµå

	bool isBossBGM;
	float fadeIn;

	//
	int warpDelay;
	bool isWarp;
	bool gameOver;
public:
	HRESULT init();
	void release();
	void update();
	void warp();
	void bossCutScene();
	void render();

	void soundInit();
	void attackBoss();
	void attackChest();
};