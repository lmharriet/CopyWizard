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
	WALL,
	BOSSDAMAGED,
	BOSSDIE,
	FINALATTACK
};
struct tagBoss
{
	RECT rc;
	POINT center;
	BOSSSTATE bossState;
	int bossHp;
	float angle;
	bool isHit;
};
struct tagBlock
{
	RECT rc;
	POINT center;
	float angle;
	int type;
	int blockCount;
	bool isFire;
	bool isHit;
};

struct tagHit0
{
	int skillNum;
	int currentTime;
	int endTime;
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
	//tagBlock block[3];
	tagBlock drillBlock;
	//tagBlock niddleBlock[5];
	vector<tagBlock*> niddleBlock;
	vector<tagBlock*> punchBlock;
	vector<tagBlock*> wallBlock;
	vector<tagBlock*> shockBlock;
	
	//damage calculate
	vector<tagHit0> vDamage;

	int count, timer;
	int posX, posY;
	int posPlayer;
	int punchCount;
	int woodTimer;
	int frameX, frameY;
	int pattern;
	int patternCount;
	int patternTimer;
	int samePattern;
	int hitTimer;

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
	bool isHit;
	bool isFinalAttack;
	bool gameOver;
	bool bossDied;

	bool bossAtack;
	bool finalAttack;
	bool tempDown;

	float niddleAngle;

	//sound
	bool isPlayDrillSFX;
	bool isPlayJumpSFX;
	bool isPlayWallSFX;
	bool isPlayNiddleSFX;
	bool isPlayPunchSFX;
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
	void collCheck();
	void punchSet();
	void punchRectMove();
	void bossDamaged();
	void bossDie();
	
	void bossHpInfo(HDC hdc, int destX, int destY);

	bool damageCheck(int skillNum);

	void damage(int damage, float _hitAngle, float _knockBack, int skillNum, bool isCritical);

	void damageCul();

	void bossFinalAttack(int patternType);


	//getter / setter
	tagBoss getBoss() { return boss; }
	RECT getBossRect() {
		return boss.rc;
	}
	bool getBossAtack() {
		return bossAtack;
	}
	
	void setRect(int x, int y) { boss.rc = RectMakeCenter(x, y, 150, 150); }
	void setCenter(POINT pt) { boss.center = { pt.x,pt.y }; }
	void setBossHit(bool bossHit) { boss.isHit = bossHit; }
	//void setBossHp(int bosshp) { boss.bossHp -= bosshp; }
};



