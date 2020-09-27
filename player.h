#pragma once
#include "gameNode.h"


enum PLAYERSTATE
{
	IDLE,
	RUN,
	DASH,
	SKILL
};
class player : public gameNode
{

private:

private:
	PLAYERSTATE state;
	RECT rc;
	int x, y;
	int speed;
	bool isLeft;
	bool isUp;

	int count, index;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	
	int getPlayerX() { return x; }
	int getPlayerY() { return y; }
	RECT getPlayerRect() { return rc; }

	player() {}
	~player() {}
};

