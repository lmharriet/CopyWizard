#pragma once
#include "gameNode.h"
enum BOSSSTATE
{
	RESPONE,
	BOSSIDLE,
	JUMP,
	DRILL,
	NIDDLE,
	PUNCH,
	WALL
};
struct tagBoss
{
	RECT rc;
	POINT center;
	BOSSSTATE bossState;
	float angle;
};
struct tagBlock
{
	RECT rc;
	POINT center;
	float angle;
	int type;
	int blockCount;
	bool isNear;
};
class player;
class boss : public gameNode
{
private:
	player* _player;
public:
	void getPlayerInfo(player* _p) {
		_player = _p;
	}
private:
	tagBoss boss;
	tagBlock block[3];
	tagBlock drillBlcok;
	tagBlock niddleBlock[5];
	vector<tagBlock> wallBlock;

	int count, timer;
	int posX, posY;
	int posPlayer;
	int punchCount;
	int woodTimer;
	int frameX, frameY;

	bool jumpMotion;
	bool leftCheck;
	bool punching[3];
	bool startNiddle;

	float niddleAngle;
public:
	HRESULT init(int _posX, int _posY);
	void release();
	void update();
	void render();

	void animation();
	void bossPlayerAngle();
	void jump();
	void drill();
	void punch();
	void niddle();
	void middleWood();
	void wall();

	RECT getBossRect() {
		return boss.rc;
	}
};



