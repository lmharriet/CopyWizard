#pragma once
#include "gameNode.h"
#include "unitNode.h"
#include "tileNode.h"

struct tagInfo
{
	string keyName;
	string explanation;
	int coolTime;
};
//총알 구조체
struct tagBullet
{
	image* bulletImage;
	RECT rc;
	MONSTERKIND kind;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float gravity;
	float radius;
	float distance;
	int count;
	int atkPower;

	bool fire;
	bool collision;
	int FrameX;
	int FrameY;
};

struct tagArcana
{
	image* img;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float radius;
	float range;

	int lifeTime;
	int atkPower;

	int frameX;
	int frameY;

	bool Collision;
	MOVE dir;
};

struct tagMeteor
{
	image* img;
	RECT rc;

	float x, y;
	float angle;
	float speed;
	int atkPower;
	int lifeTime;

	//삭제 조건
	float endY;


};

struct tagDragon
{
	RECT rc;
	image* img;
	float x, y;
	float speed;
	float angle;
	float saveAngle;

	int frameY;
	int index;
	int lifeTime;
	int currentTime;
	int atkPower;

	float persent;

	bool isFire;
};

//=============================================================
//    ## attack ## (공용총알)
//=============================================================
class attack : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagBullet> _vAttack;
	vector<tagBullet>::iterator _viAttack;
private:
	const char* _imageName;    //총알 이미지 이름
	float _range;            //총알 사거리
	float _ghoulLargeRange;
	int _bulletMax;            //총알 최대갯수
	bool _isFrameImg;        //프레임 이미지냐?


public:
	HRESULT init(const char* imageName, int bulletMax, float range, float ghoulLargeRange, bool isFrameImg = false);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind);
	//총알무브
	void move();
	//총알 비어있는지 확인
	bool attackEmpty() { if (_vAttack.empty())return true; else return false; }

	//총알삭제
	void removeAttack(int index);

	//공용총알 벡터 가져오기
	vector<tagBullet> getAttack() { return _vAttack; }
	//공용총알 렉트 가져오기
	RECT getRect(int index) { return _vAttack[index].rc; }

};

struct tagBomb
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;

	float range;	// 최대 거리
	float distance; // firePos 부터 curPos 까지 거리

	int atkPower;

	bool collision;
};
//=============================================================
//	## bomb ## (폭탄처럼 한발씩 발사하고 생성하고 자동삭제) 
//=============================================================
class bomb : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagBomb> _vBullet;
	tagBomb tmpBomb;

	tagInfo info;

	int bombCount;
	int bTime;
private:
	int _bulletMax;			//총알 최대갯수
	int count, index;

	bool isCoolTime;	// 스킬 쿨타임이 돌고있는지 아닌지
						// 몇초간 스킬 사용을 금할지 -> info cooltime
	int currentCoolTime;	// 쿨타임이 다 돌면 isCoolTime을 false

	bool bossScene;

public:

	HRESULT init(int maxBullet);
	void release();
	void update();
	void render();

	float getRange(float angle, float x, float y);

	//총알발사
	void fire(float x, float y, float speed, float angle);
	void bombActive();
	//총알무브
	void move();

	//폭탄삭제
	void removeBomb(int index);

	//총알벡터 가져오기
	vector<tagBomb> getBullet() { return _vBullet; }
	tagInfo getInfo() { return info; }

	bool getCool() { return isCoolTime; }
	bool getCol(int index) { return _vBullet[index].collision; }
	int getSize() { return _vBullet.size(); }

	void setCol(int index, bool temp) { _vBullet[index].collision = temp; }
	void setBossScene(bool temp) { bossScene = temp; }
};

//=============================================================
//	## METEOR ## 
//=============================================================
struct tagCircle
{
	RECT rc;
	float x, y;
	float angle;
	float radius;

	int lifeTime;
	bool isCol;


	int count;
	int index;
};
class meteor :public gameNode
{
private:
	vector<tagMeteor> vMeteor;
	vector<tagCircle> vCircle;
	vector<tagMeteor> vDamage;

	tagInfo info;

private:

	bool isCoolTime;	// 스킬 쿨타임이 돌고있는지 아닌지
						// 몇초간 스킬 사용을 금할지 -> infoCoolTime	
	int currentCoolTime;	// 쿨타임이 다 돌면 isCoolTime을 false


	int count, index;


	//upgrade용
	POINT save;
	int ranCount;
	int meteorCount;
	bool upgrade;
	bool isReady;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void makeCircle(float x, float y);
	void creatMeteor(float x, float y, float angle);



	void move();
	void coolTimeReduction();


	tagInfo getInfo() { return info; }

	bool getCool() { return isCoolTime; }
	int getSkillNum() { return (int)PLAYERSKILL::METEOR; }
	//생성된 메테오용
	vector<tagMeteor> getMeteorVec() { return vMeteor; }
	//충돌 + 데미지용
	int  getColSize() { return vDamage.size(); }
	int getAtkPower(int index) { return vDamage[index].atkPower; }
	float getAngle(int index) { return vDamage[index].angle; }
	RECT getColRect(int index) { return vDamage[index].rc; }

	//upgrade 스킬용
	void setUpgrade(bool _upgrade) { upgrade = _upgrade; }

};


//===================
//	## dashFire ## 
//===================
class dashFire :public gameNode
{
private:
	vector<tagArcana> vDash;
	tagInfo info;


	int currentCoolTime;

	bool isCoolTime;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	void singleRender(int index);

	void fire(float x, float y);

	void coolTimeReduction();

	//getter, setter
	tagInfo getInfo() { return info; }

	RECT getRect(int index) { return vDash[index].rc; }
	int getSize() { return vDash.size(); }
	int getAtk(int index) { return vDash[index].atkPower; }
	int getSkillNum() { return (int)PLAYERSKILL::DASHFIRE; }
	float getY(int index) { return vDash[index].y; }

	bool getIsCoolTime() { return isCoolTime; }

	void setIsCoolTime(bool temp) { isCoolTime = temp; }

};

//===================
//	## RANGING INFERNO ## 
//===================
struct tagTail
{
	int currentTime;
	float minAngle;
	int lifeTime;
	float speed;
	float angle;

	float x;
	float y;
};

class RagingInferno :public gameNode
{
private:
	tagArcana inferno;

	vector<tagTail> vTail;

	tagInfo info;

	float distance;
	int index, count;
	bool isFire;
	bool isActive;
	bool gauging;

	int gaugeTime;

	int time;



	//cool Time
	bool isCoolTime;	// 스킬 쿨타임이 돌고있는지 아닌지
						// 몇초간 스킬 사용을 금할지->info cooltime
	int currentCoolTime;	// 쿨타임이 다 돌면 isCoolTime을 false

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void fire(float x, float y, float angle);
	void move();

	void coolTimeReduction();
	bool CheckCollision(RECT enemy);

	//getter , setter

	tagInfo getInfo() { return info; }

	bool getFire() { return isFire; }
	bool getCol() { return inferno.Collision; }
	bool getGauging() { return gauging; }
	bool getActive() { return isActive; }
	bool getCool() { return isCoolTime; }


	int getSkillNum() { return (int)PLAYERSKILL::INFERNO;}
	tagArcana getInf() { return inferno; }


	void setX(float X) { inferno.x = X; }
	void setY(float Y) { inferno.y = Y; }
	void setActive(bool temp) { isActive = temp; }
};


//미완성 -> 업그레이드 스킬 충돌용 렉트 넣기
//=============================================================
//	##dragonArc## 
//=============================================================
struct tagDragonCol
{
	RECT rc;
	float x, y;
	float angle;

};
class dragonArc : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagDragon> vDragon;

	vector<tagDragon> vWings;
	tagDragon dragonHead;


	tagInfo info;
private:

	vector<tagDragonCol> vColRound;

	int pattern;

	int currentCoolTime;
	bool isCoolTime;
	bool upgrade;

	int count, index;

public:
	HRESULT init();
	void release();
	void update();
	void render();


	void fire(float x, float y, float angle);
	void move();

	//upgrade skill
	void phoenixFire(float x, float y, float angle);
	void phoenixMove();

	void coolTimeReduction();


	//getter ,setter

	tagInfo getInfo() { return info; }

	//non-Upgrade
	RECT getDragonRC(int index) { return vDragon[index].rc; }
	float getDragonAngle(int index) { return vDragon[index].angle; }
	int getAtkPower(int index) { return vDragon[index].atkPower; }
	int getSkillNum() { return (int)PLAYERSKILL::DRAGONARC;}
	int getSize() { return vDragon.size(); }

	//upgrade
	RECT getColRc(int index) { return vColRound[index].rc; }
	float getHeadAngle(int index) { return vColRound[index].angle; }
	int getcolSize() { return vColRound.size(); }
	int getUpgradeAtkPower(int index) { return vWings[index].atkPower; }

	bool getUpgrade() { return upgrade; }
	bool getCool() { return isCoolTime; }

	void setUpgrade(bool temp) { upgrade = temp; }

};



//ICE SPEAR 
struct tagSpear
{
	RECT rc;
	float x, y;
	float fireX, fireY;
	float angle;

	float distance; // firePos 부터 curPos 까지 거리

	float range;  // 최대 거리
	float speed;
	int atkPower;
	int lifeTime;

	bool collision;
	bool isBig;
};
struct tagStaySpear
{
	float x, y;
	float strY;
	float endY;
	float angle;
	float speed;
	float range;

	float fireX, fireY;
};
class iceSpear : public gameNode
{
private:
	
	tagInfo info;
	tagSpear saveSpear;

	vector<tagSpear> vSpear;
	
	vector<tagSpear> vUltSpear;
	vector<tagStaySpear> vStay;

	float saveRange;
	
	int spearCount;
	int time;
	int pattern;

	int currentCoolTime;
	bool upgrade;
	bool isCoolTime;
	bool upgread;
	
	bool active;
	int imgRadius;

	//upgrade
	float posX, posY;

	int delay;
	int sTime;
	int eTime;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	void chargeSpear();
	float rangeCul(float maxRange, float x, float y, float angle);
	void fire(float x, float y, float angle);
	void move();

	void upgradefire(float x, float y, float angle);
	void fireCount();
	void upgradeMove();

	void coolTimeReduction();


	//getter , setter

	tagInfo getInfo() { return info; }
	vector<tagSpear> getSpear() { return vSpear; }
	int getSize() { return vSpear.size(); }
	RECT getSpearRc(int index) { return vSpear[index].rc; }
	int getAtkPower(int index) { return vSpear[index].atkPower; }
	float getSpearAngle(int index) { return vSpear[index].angle; }
	int getSkillNum() { return (int)PLAYERSKILL::ICESPEAR;}
	
	void setCol(int index, bool col) { vSpear[index].collision = col; }



	//upgrade
	RECT getUpgradeRC(int index) { return vUltSpear[index].rc; }
	int getUpgradeSize() { return vUltSpear.size(); }
	int getUpgradeAtk(int index) { return vUltSpear[index].atkPower; }
	float getUpgradeAngle(int index) { return vUltSpear[index].angle; }
	bool getIsBig(int index) { return vUltSpear[index].isBig; }
	bool getCool() { return isCoolTime; }
	void setUpgradeCol(int index, bool col) { vUltSpear[index].collision = col; }

};
