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
	bool fire;
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

	//�Ѿ˻���
	void removeBullet(int index);

	//�����Ѿ� ���� ��������
	vector<tagBullet> getBullet() { return _vBullet; }
	//�����Ѿ� ��Ʈ ��������
	RECT getRect(int index) { return _vBullet[index].rc; }
};

//�̿ϼ�
//=============================================================
//	## homingFlares ## (homingFlares[0] -> �迭ó�� �̸� �����صΰ� �Ѿ˹߻�)
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
	vector<tagArcana> _vMeteor;
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
};