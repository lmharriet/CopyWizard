#pragma once
#include "gameNode.h"
#include "unitNode.h"
#include "tileNode.h"

struct tagInfo
{
	string keyName;
	string explanation;
};
//�Ѿ� ����ü
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

	//���� ����
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
//	## bullet ## (�����Ѿ�)
//=============================================================
class bullet : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
private:
	const char* _imageName;	//�Ѿ� �̹��� �̸�
	float _range;			//�Ѿ� ��Ÿ�
	int _bulletMax;			//�Ѿ� �ִ밹��
	bool _isFrameImg;		//������ �̹�����?


public:
	HRESULT init(const char* imageName, int bulletMax, float range, bool isFrameImg = false);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind);
	//�Ѿ˹���
	void move();
	//�Ѿ� ����ִ��� Ȯ��
	bool bulletEmpty() { if (_vBullet.empty())return true; else return false; }

	//�Ѿ˻���
	void removeBullet(int index);

	//�����Ѿ� ���� ��������
	vector<tagBullet> getBullet() { return _vBullet; }
	//�����Ѿ� ��Ʈ ��������
	RECT getRect(int index) { return _vBullet[index].rc; }
	
};

struct tagBomb
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;

	float range;	// �ִ� �Ÿ�
	float distance; // firePos ���� curPos ���� �Ÿ�

	int atkPower;

	bool collision;
};
//=============================================================
//	## bomb ## (��źó�� �ѹ߾� �߻��ϰ� �����ϰ� �ڵ�����) 
//=============================================================
class bomb : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagBomb> _vBullet;
	tagBomb tmpBomb;

	tagInfo info;

	int bombCount;
	int bTime;
private:
	int _bulletMax;			//�Ѿ� �ִ밹��
	int count, index;

	bool isCoolTime;	// ��ų ��Ÿ���� �����ִ��� �ƴ���

	int coolTime;		// ���ʰ� ��ų ����� ������
	int currentCoolTime;	// ��Ÿ���� �� ���� isCoolTime�� false

	bool bossScene;

public:

	HRESULT init(int maxBullet);
	void release();
	void update();
	void render();

	float getRange(float angle, float x, float y);

	//�Ѿ˹߻�
	void fire(float x, float y, float speed, float angle);
	void bombActive();
	//�Ѿ˹���
	void move();

	//��ź����
	void removeBomb(int index);

	//�Ѿ˺��� ��������
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

	bool isCoolTime;	// ��ų ��Ÿ���� �����ִ��� �ƴ���

	int coolTime;		// ���ʰ� ��ų ����� ������
	int currentCoolTime;	// ��Ÿ���� �� ���� isCoolTime�� false


	int count, index;


	//upgrade��
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
	int getSkillNum() { return 1; }
	//������ ���׿���
	vector<tagMeteor> getMeteorVec() { return vMeteor; }
	//�浹 + ��������
	int  getColSize() { return vDamage.size(); }
	int getAtkPower(int index) { return vDamage[index].atkPower; }
	float getAngle(int index) { return vDamage[index].angle; }
	RECT getColRect(int index) { return vDamage[index].rc; }

	//upgrade ��ų��
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

	int coolTime;
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
	int getSkillNum() { return 2; }
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
	bool isCoolTime;	// ��ų ��Ÿ���� �����ִ��� �ƴ���

	int coolTime;		// ���ʰ� ��ų ����� ������
	int currentCoolTime;	// ��Ÿ���� �� ���� isCoolTime�� false

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


	int getSkillNum() { return 3; }
	tagArcana getInf() { return inferno; }


	void setX(float X) { inferno.x = X; }
	void setY(float Y) { inferno.y = Y; }
	void setActive(bool temp) { isActive = temp; }
};


//�̿ϼ� -> ���׷��̵� ��ų �浹�� ��Ʈ �ֱ�
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
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagDragon> vDragon;

	vector<tagDragon> vWings;
	tagDragon dragonHead;


	tagInfo info;
private:

	vector<tagDragonCol> vColRound;

	int pattern;

	int coolTime;
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
	int getSkillNum() { return 4; }
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

	float distance; // firePos ���� curPos ���� �Ÿ�

	float range;  // �ִ� �Ÿ�
	float speed;
	int atkPower;
	
	bool collision;
};
class iceSpear : public gameNode
{
private:
	
	tagInfo info;

	vector<tagSpear> vSpear;

	float gauge;

	int coolTime;
	int currentCoolTime;
	bool upgrade;
	bool isCoolTime;

	int imgRadius;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void chargeSpear();

	float rangeCul(float maxRange, float x, float y, float angle);

	void fire(float x, float y, float angle);
	void move();


};