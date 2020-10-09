#pragma once
#include "gameNode.h"
#include "unitNode.h"
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
	float x, y;
	float angle;
	RECT rc;
	float speed;
	int atkPower;

	//���� ����
	float endY;
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
	void fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind, bool image = false);
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


//=============================================================
//	## bomb ## (��źó�� �ѹ߾� �߻��ϰ� �����ϰ� �ڵ�����) 
//=============================================================
class bomb : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	float _range;			//�Ѿ� ��Ÿ�
	int _bulletMax;			//�Ѿ� �ִ밹��
	int count, index;
public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float speed,float angle, float radius);
	//�Ѿ˹���
	void move();

	//��ź����
	void removeBomb(int index);

	//�Ѿ˺��� ��������
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
	RECT rc;
	float x, y;
	float angle;
	float radius;

	int lifeTime;
	bool isCol;

};
class meteor :public gameNode
{
private:
	vector<tagMeteor> vMeteor;
	vector<tagCircle> vCircle;

	int count, index;
	int circleCount, CircleIndex;

	bool isCoolTime;	// ��ų ��Ÿ���� �����ִ��� �ƴ���

	int coolTime;		// ���ʰ� ��ų ����� ������
	int currentCoolTime;	// ��Ÿ���� �� ���� isCoolTime�� false
public:
	HRESULT init();
	void release();
	void update();
	void render();
	
	void makeCircle(float x, float y);
	void creatMeteor(float x, float y,float angle);

	
	void move();
	bool getCool() { return isCoolTime; }
	int getSkillNum() { return 1; }

	vector<tagMeteor> getMeteorVec() { return vMeteor; }
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

	float distance;
	int index, count;
	bool isFire;
	bool isActive;
	bool gauging;

	int time;

	bool isCoolTime;	// ��ų ��Ÿ���� �����ִ��� �ƴ���

	int coolTime;		// ���ʰ� ��ų ����� ������
	int currentCoolTime;	// ��Ÿ���� �� ���� isCoolTime�� false

public:
	HRESULT init();
	void release();
	void update(int *gaugeTime);
	void render();
	void fire(float x, float y,float angle, int *gaugeTime);
	void move( int gaugeTime);
	bool CheckCollision(RECT enemy);
	int getSkillNum() { return 3; }

	//getter , setter
	bool getFire() { return isFire; }
	bool getCol() { return inferno.Collision; }
	bool getGauging() { return gauging; }
	bool getActive() { return isActive; }
	bool getCool() { return isCoolTime; }

	tagArcana getInf() { return inferno; }
	void setX(float X) { inferno.x = X; }
	void setY(float Y) { inferno.y = Y; }
	void setActive(bool temp) { isActive = temp; }
};


//�̿ϼ�
//=============================================================
//	## homingFlares ## 
//=============================================================
class cleaver : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagArcana> cleaver;
	vector<tagArcana> _vRange;
	//	vector<tagBullet>::iterator _viBullet;

private:
	float _range;			//�Ѿ� ��Ÿ�
	int _bulletMax;			//�Ѿ� �ִ밹��

public:
	HRESULT init(float range);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float angle);
	//�Ѿ˹���
	void move();

};