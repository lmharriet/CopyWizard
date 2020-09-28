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
	tagParticlePoint(int _x, int _y, int _number, float _minAngle, float _maxAngle, int _CreateDelay, int _delay)
	: x(_x), y(_y), number(_number), minAngle(_minAngle), maxAngle(_maxAngle), CreateDelay(_CreateDelay), delay(_delay) {}

	int x, y;
	int number;
	float minAngle, maxAngle;
	int CreateDelay;
	int delay;
};
class particleManager : public singletonBase <particleManager>
{
private:
	vector<tagParticlePoint> particlePoint;
	vector<tagParticle*> vParticle;
	vector<tagParticle*> vParticlePuller;

public:
	HRESULT init();

	tagParticle* getParticle();

	void rtnParticle(tagParticle* ptc);

	HRESULT generate(int x, int y, int size, int gravity, float angle, int delay, int speed); // create rect
	HRESULT pointInit(int x, int y, int number, float minAngle, float maxAngle, int CreateDelay); // create rects

	void resetPoint() { particlePoint.clear(); }

	void active(HDC hdc, RECT cam);

	void pointActive();
};