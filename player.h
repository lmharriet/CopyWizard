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

enum class MOVE
{
	LEFT,
	LEFT_TOP,
	RIGHT,
	RIGHT_TOP,
	UP,
	DOWN,
	LEFT_DOWN,
	RIGHT_DOWN,
};
enum class STATE
{
	IDLE,
	RUN,
	DASH
};

class player : public gameNode
{
private:
	tagTile* tile;
private:
	MOVE move;
	STATE state;
	RECT rc;
	tagCollider tileCheck[8];

	float posX, posY;
	int speed;

	int count, index;
	int dashCount,dashIndex;

	bool dashLeft, dashRight, dashUp, dashDown;

	// 신호 중복 방지
	bool isLeft, isRight, isUp, isDown;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void controller();
	void dashFunction();
	void animation();
	//collision detection
	void tileCol();
	void makeCol(int index, int destX, int destY, int rcSize = 7);

	void resetKey();

	void changeState();
	void buttonDown();

	void viewText();

	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }

	//tile address
	void setTileAd(tagTile* _tile) { tile = _tile; }


	player() {}
	~player() {}
};