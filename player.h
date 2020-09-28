#pragma once
#include "gameNode.h"
#include "tileNode.h"

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
	float posX, posY;
	int speed;
	bool isLeft;
	bool isUp;

	int count, index;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }

	player() {}
	~player() {}
};