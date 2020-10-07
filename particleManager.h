#pragma once
#include "singletonBase.h"

struct tagParticle
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

struct tagParticlePoint
{
	float x, y;
	int delay;

	int angleNum;		//파티클 n방향을 가리킴
	int maxAngle;		//파티클의 방향이 총 몇가지?
	float radius;		//좌표 중앙에서 얼마큼 멀어진곳에 생성?

	float particleSpeed;//파티클이 angle로 날라가는 속도
	int frameDelay;		//파티클 프레임 이미지 딜레이

	int currentTime;
	int lifeTime;
};

class particleManager : public singletonBase<particleManager>
{
private:
	vector<tagParticlePoint> vParticlePoint; // 파티클 생성자
	vector<tagParticlePoint> vExplosion;	  // 폭팔 파티클 생성자
	vector<tagParticle> vParticle;			  // 파티클
public:
	HRESULT init();
	void render(HDC hdc);

	void pointGenerate(float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay);

	void explosionGenerate(float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay);

	void pointActive();
	void explosionActive();
	void generate(string keyName, float x, float y, float angle, int delay, float speed);
};