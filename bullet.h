#pragma once
#include "gameNode.h"
#include "unitNode.h"
//총알 구조체
struct tagBullet
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float gravity;
	float radius;
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

//=============================================================
//	## bullet ## (공용총알)
//=============================================================
class bullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
private:
	const char* _imageName;	//총알 이미지 이름
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수
	bool _isFrameImg;		//프레임 이미지냐?

	
public:
	HRESULT init(const char* imageName, int bulletMax, float range, bool isFrameImg = false);
	void release();
	void update();
	void render();
	
	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알무브
	void move();
	//총알 비어있는지 확인
	bool bulletEmpty() { if (_vBullet.empty())return true; else return false; }

	//총알삭제
	void removeBullet(int index);

	//공용총알 벡터 가져오기
	vector<tagBullet> getBullet() { return _vBullet; }
	//공용총알 렉트 가져오기
	RECT getRect(int index) { return _vBullet[index].rc; }
	
};


//=============================================================
//	## bomb ## (폭탄처럼 한발씩 발사하고 생성하고 자동삭제) 
//=============================================================
class bomb : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수
	int count, index;
public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float speed,float angle, float radius);
	//총알무브
	void move();

	//폭탄삭제
	void removeBomb(int index);

	//총알벡터 가져오기
	vector<tagBullet> getBullet() { return _vBullet; }

	bool getCol(int index) { return _vBullet[index].collision; }
	int getSize() { return _vBullet.size(); }

	void setCol(int index, bool temp) { _vBullet[index].collision = temp; }
};

//=============================================================
//	## METEOR ## 
//=============================================================
struct tagCircle
{
	float x, y;
	float angle;
	RECT rc;
};
class meteor :public gameNode
{
private:
	vector<tagArcana> vMeteor;
	vector<tagCircle> vCircle;


	float angleRange;
	float _range;


	//ult
	int meteorCount;
	bool isUlt;
	tagArcana ult;

	int ranCount;

	int count, index;
	int timer;

	bool isCol;
public:
	HRESULT init(float range);
	void release();
	void update();
	void render();

	void meteorFire(float x, float y, float speed, MOVE dir, float range);
	void meteorUlt();
	void meteorUltFire(float x, float y, float speed, MOVE dir, float range);
	void move();
	int getSkillNum() { return 1; }

	bool getCol() { return isCol; }

	vector<tagArcana> getMeteorVec() { return vMeteor; }
};


//===================
//	## dashFire ## 
//===================
class dashFire :public gameNode
{
private:
	vector<tagArcana> _vDash;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void singleRender(int index);

	void fire(float x, float y);

	//getter, setter
	
	RECT getRect(int index) { return _vDash[index].rc; }
	int getSize() { return _vDash.size(); }
	int getAtk(int index) { return _vDash[index].atkPower; }
	int getSkillNum() { return 2; }
	float getY(int index) { return _vDash[index].y; }

	
};

//===================
//	## RANGING INFERNO ## 
//===================
class RagingInferno :public gameNode
{
private:
	tagArcana inferno;

	float range;
	float distance;
	int index, count;
	bool isFire;
public:
	HRESULT init();
	void release();
	void update(float range, int gaugeTime);
	void render();
	void fire(float x, float y,float angle);
	void move(float range, int gaugeTime);
	int getSkillNum() { return 3; }

	//getter , setter
	bool getFire() { return isFire; }
	bool getCol() { return inferno.Collision; }

};


//미완성
//=============================================================
//	## homingFlares ## 
//=============================================================
class homingFlares : public gameNode
{
private:
	//총알 구조체를 담을 벡터선언
	vector<tagArcana> _vFlares;
	vector<tagArcana> _vRange;
	//	vector<tagBullet>::iterator _viBullet;

private:
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수

public:
	HRESULT init(float range);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float angle);
	//총알무브
	void move();

};