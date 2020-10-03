#pragma once
#include "gameNode.h"
#include "player.h"
#define MAXWIDTH 1500
#define MAXHEIGHT 989
class bossScene : public gameNode
{
	RECT rc;
	RECT area;

	bool isBattle;
	int cutTime;

	player* _player;
public:
	HRESULT init();
	void release();
	void update();
	void bossCutScene();
	void render();
};