#pragma once
#include "gameNode.h"
#include "bullet.h"

enum FINALBOSSSTATE
{
	FINALBOSSSPONE,
	FINALBOSSIDLE,
	FINALBOSSDASH,
	FINALBOSSSKILL,
	FINALBOSSDAMAGED,
	FINALBOSSDIE
};
enum BOSSSKILLKIND
{
	SKILL1,
	SKILL2,
	SKILL3,
	SKILL4,
	SKILL5
};
struct tagFinaleBoss
{
	RECT rc;
	POINT center;
	FINALBOSSSTATE bossState;
	BOSSSKILLKIND bossSkill;
	int bossHp;
	int dashDintance;
	float angle;
	bool isHit;
	bool isCol;
};
struct skillBlock
{
	RECT rc;
	POINT center;
	float angle;
	int type;
	int blockCount;
	bool isFire;
	bool isHit;
};
class player;
class finalBoss : public gameNode
{
private:
	player* _player;
public:
	void getPlayerInfo(player* _p) {
		_player = _p;
	}
private:
	tagFinaleBoss boss;

	RECT dashRc;
	vector<skillBlock*> wallBlock;
	vector<skillBlock*> blazeBlock;
	vector<skillBlock*> thunderBlock;
	vector<skillBlock*> windBlock;
	vector<skillBlock*> iceBlock;

	int timer;
	int count;
	int frameX;
	int frameY;
	int dashX;
	int dashY;
	int dashCount;
	int posDashRc;
	int posPlayer;
	int skillNum;

	float dashAngle;

	bool changStage;
	bool leftCheck;
	bool setDashrc;

	bool patternStart;
	bool wallPattern;
	bool blazePattern;
	bool thunderPattern;
	bool windPattern;
	bool icePattern;
public:
	HRESULT init(int _posX, int _posY);
	void release();
	void update();
	void render();

	void animation();
	void updateDashRect();
	void dashPattern();
	void dashColPlayer();
	void getPosDashRc();
	void useSkill();
	void posPlayerAngle();

	void wall();
	void blaze();
	void thunder();
	void wind();
	void ice();
	void die();
	void colCheck();

	void finalBossHpInfo(HDC hdc, int destX, int destY);

	tagFinaleBoss getFinalBoss() {
		return boss;
	}
};

