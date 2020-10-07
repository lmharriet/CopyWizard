#pragma once
#include "gameNode.h"
#include "unitNode.h"
//�Ѿ� ����ü
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

	int skillType;
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
	int skillType;

	int frameX;
	int frameY;

	bool Collision;
	MOVE dir;
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
	void fire(float x, float y, float angle, float speed);
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
	float getY(int index) { return _vDash[index].y; }
	bool getCol(int index) { return _vDash[index].Collision; }

	void setCol(int index, bool temp) { _vDash[index].Collision = temp; }
	
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
	int gaugeTime;
	int index, count;
	bool isFire;
public:
	HRESULT init();
	void release();
	void update(float range);
	void render();
	void fire(float x, float y,float angle);
	void move(float range);


	//getter , setter
	bool getFire() { return isFire; }
	bool getCol() { return inferno.Collision; }
	int getGaugeTime() { return gaugeTime; }
	
};


//�̿ϼ�
//=============================================================
//	## homingFlares ## 
//=============================================================
class homingFlares : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ���ͼ���
	vector<tagArcana> _vFlares;
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