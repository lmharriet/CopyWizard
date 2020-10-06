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
	bool fire;
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
	void fire(float x, float y,float angle);
	//�Ѿ˹���
	void move();

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
};

//=============================================================
//	## METEOR ## 
//=============================================================
class meteor :public gameNode
{
private:
	vector<tagArcana> vMeteor;
	
	float angleRange;
	float _range;


	//ult
	int meteorCount;
	bool isUlt;
	tagArcana ult;

	int ranCount;

	int count, index;
	int timer;
public:
	HRESULT init(float range);
	void release();
	void update();
	void render();

	void meteorFire(float x, float y, float speed, MOVE dir, float range);
	void meteorUlt();
	void meteorUltFire(float x, float y, float speed, MOVE dir, float range);
	void move();

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
	float getY(int index) { return _vDash[index].y; }
	int getSize() { return _vDash.size(); }
	RECT getRect(int index) { return _vDash[index].rc; }
	
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
};