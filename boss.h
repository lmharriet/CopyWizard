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
	int pattern;
	int patternCount;

	bool jumpMotion;
	bool leftCheck;
	bool punching[3];
	bool startNiddle;
	bool patternStart;

	bool jumpPattern;
	bool drillPattern;
	bool punchPattern;
	bool niddlePattern;
	bool wallPattern;

	float niddleAngle;
public:
	HRESULT init(int _posX, int _posY);
	void release();
	void update();
	void render();

	void animation();
	void bossPlayerAngle();
	void jump(int patternType);
	void drill(int patternType);
	void punch(int patternType);
	void niddle(int patternType);
	void middleWood();
	void wall(int patternType);
	void bossPattern();

	RECT getBossRect() {
		return boss.rc;
	}
};



