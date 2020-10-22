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
	FINALBOSSWOOD,
	FINALBOSSBLAZE,
	FINALBOSSTHUNDER,
	FINALBOSSWIND,
	FINALBOSSICE
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

struct tagDamageType
{
	int skillNum;
	int currentTime;
	int endTime;
};

class player;
class finalBoss : public gameNode
{
private:
	player* _player;

	//damage calculate
	vector<tagDamageType> vDamage;

public:
	void getPlayerInfo(player* _p) {
		_player = _p;
	}
private:
	tagFinaleBoss boss;

	RECT dashRc;
	RECT iceSpearRc[5];
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
	int hitTimer;
	int patternCount;
	int posX;
	int posY;
	int iceSpearRcX[5];
	int iceSpearRcY[5];

	float dashAngle;
	float iceSpearAngle[5];

	bool changStage;
	bool leftCheck;
	bool setDashrc;
	bool isHit;
	bool bossRespone;

	bool patternStart;
	bool wallPattern;
	bool blazePattern;
	bool thunderPattern;
	bool windPattern;
	bool icePattern;
	bool isDamaged;

	bool gameOver;
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
	void grogi();

	void finalBossHpInfo(HDC hdc, int destX, int destY);

	bool damageCheck(int skillNum);

	void damage(int damage, float _hitAngle, int skillNum, bool isCritical);

	void damageCul();

	tagFinaleBoss getFinalBoss() {
		return boss;
	}

	void setRect(int x, int y) { boss.rc = RectMakeCenter(x, y, 150, 150); }
	void setCenter(POINT pt) { boss.center = { pt.x,pt.y }; }
	void setBossHit(bool bossHit) { boss.isHit = bossHit; }
};

