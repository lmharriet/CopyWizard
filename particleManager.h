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

	int angleNum;		//��ƼŬ n������ ����Ŵ
	int maxAngle;		//��ƼŬ�� ������ �� ���?
	float radius;		//��ǥ �߾ӿ��� ��ŭ �־������� ����?

	float particleSpeed;//��ƼŬ�� angle�� ���󰡴� �ӵ�
	int frameDelay;		//��ƼŬ ������ �̹��� ������

	bool isFrameDel;	//��ƼŬ�� ������ ���̵Ǹ� �����Ǵ���? -> false�� �ð��� ������ ��ŭ ����ִ´�.
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
	vector<tagParticlePoint> vParticlePoint; // ��ƼŬ ������
	vector<tagParticlePoint> vExplosion;	  // ���� ��ƼŬ ������
	vector<tagParticle> vParticle;			  // ��ƼŬ
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