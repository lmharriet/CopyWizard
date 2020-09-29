#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"
struct tagCollider
{
	RECT rc;
	POINT pos;
	bool isCol;
};

enum class STATE
{
	IDLE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	RUN,
	DASH_LEFT,
	DASH_RIGHT,
	DASH_UP,
	DASH_DOWN,
	SKILL
};

class player : public gameNode
{
private:
	tagTile* tile;
private:
	STATE pState;
	RECT rc;
	tagCollider tileCheck[8];

	float posX, posY;
	int speed;
	bool isLeft, isUp, isDash;


	int count, index;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void controller();
	void dashFunction();
	void tileCol();
	void makeCol(int index, int destX, int destY, int rcSize = 7);
	bool checkDirectionX()
	{
		if (isLeft || !isLeft) return true;
		else return false;
	}


	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }

	//tile address
	void setTileAd(tagTile* _tile) { tile = _tile; }


	player() {}
	~player() {}
};