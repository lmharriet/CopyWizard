#pragma once
#include "singletonBase.h"

struct tagParticle
{
	string keyName;

	float x, y;
	float angle;
	float speed;

	int frameX;
	int frameY;
	int maxFrameX;

	int delay;

	int time;
	bool isFrameDel;
	int lifeTime;

	bool isBack;
	int backTime;

	//collect
	bool isCollect;
	float endX, endY;
};

struct tagParticlePoint
{
	string keyName;

	float x, y;
	int delay;

	int angleNum;		//파티클 n방향을 가리킴
	int maxAngle;		//파티클의 방향이 총 몇가지?
	float radius;		//좌표 중앙에서 얼마큼 멀어진곳에 생성?

	float particleSpeed;//파티클이 angle로 날라가는 속도
	int frameDelay;		//파티클 프레임 이미지 딜레이

	bool isFrameDel;	//파티클이 프레임 끝이되면 삭제되는지? -> false면 시간을 정해준 만큼 살아있는다.
	int particleEndTime;

	int currentTime;
	int lifeTime;
	int createDelay;

	bool isBack;
	bool isCollect;
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

	void pointGenerate(string keyName, float x, float y, int CreateDelay, int lifeTime, int maxAngle, float radius, float particleSpeed, int frameDelay);

	void explosionGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, bool isFrameDel, int particleEndTime = 50,bool isBack = false);

	void collectingGenerate(string keyName, float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay, int lifeTime, int createDelay);

	void pointActive();
	void explosionActive();

	void generate(string keyName, float x, float y, float angle, int delay, float speed, bool isFrameDel, int particleEndTime = 50, bool isBack = false, int backTime = 0);
	void generate2(string keyName, float x, float y, float angle, int delay, float speed, float endX, float endY, bool isCollect = true);

	void potionParticlePlay(float x, float y);
	void explosionParticlePlay(float x, float y);
	void explosionParticle2Play(float x, float y);
	void bossJumpParticlePlay(float x, float y);
	void smokeParticlePlay(float x, float y);
};