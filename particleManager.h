#pragma once
#include "singletonBase.h"
struct tagParticle
{
	tagParticle(int _x, int _y, int _size, int _gravity, float _angle, int _fixedDelay, int _delay, int _speed)
	: x(_x), y(_y), size(_size), gravity(_gravity), angle(_angle), fixedDelay(_fixedDelay), delay(_delay), speed(_speed) {}

	int x, y;
	int size;
	int gravity;
	float angle;
	int fixedDelay;
	int delay;

	int speed;
};
struct tagParticlePoint
{
	tagParticlePoint(int _x, int _y, int _number, float _minAngle, float _maxAngle, int _CreateDelay, int _delay, int _time)
	: x(_x), y(_y), number(_number), minAngle(_minAngle), maxAngle(_maxAngle), CreateDelay(_CreateDelay), delay(_delay), time(_time) {}

	int x, y;
	int number;
	float minAngle, maxAngle;
	int CreateDelay;
	int delay;
	int time;
};

struct tagParticle2
{
	float x, y;
	float angle;
	float speed;

	int frameX;
	int frameY;
	int maxFrameX;

	int delay;

	int time;
};

struct tagParticlePoint2
{
	float x, y;
	int delay;

	int angleNum;		//��ƼŬ n������ ����Ŵ
	int maxAngle;		//��ƼŬ�� ������ �� ���?
	float radius;		//��ǥ �߾ӿ��� ��ŭ �־������� ����?

	float particleSpeed;//��ƼŬ�� angle�� ���󰡴� �ӵ�
	int frameDelay;		//��ƼŬ ������ �̹��� ������

	int currentTime;
	int lifeTime;
};
class particleManager : public singletonBase <particleManager>
{
private:
	vector<tagParticlePoint> particlePoint;

	vector<tagParticlePoint2> particlePoint2;
	

	vector<tagParticle*> vParticle;
	vector<tagParticle*> vParticlePuller;

public:
	HRESULT init();

	tagParticle* getParticle();

	void rtnParticle(tagParticle* ptc);

	HRESULT generate(int x, int y, int size, int gravity, float angle, int delay, int speed); // create rect
	HRESULT pointGenerate(int x, int y, int number, float minAngle, float maxAngle, int CreateDelay, int time); // create rects

	void pointGenerate2(POINT pt, int CreateDelay, int time);

	void resetPoint() { particlePoint.clear(); }

	void render(HDC hdc, RECT cam);

	void tRender(HDC hdc);

	void pointActive();
};

class particleManager0 : public singletonBase<particleManager0>
{
private:
	vector<tagParticlePoint2> vParticlePoint; // ��ƼŬ ������
	vector<tagParticle2> vParticle;			  // ��ƼŬ
public:
	HRESULT init();
	void render(HDC hdc);

	void pointGenerate(float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay);

	void pointActive();
	void generate(float x, float y, float angle, int delay, float speed);
};