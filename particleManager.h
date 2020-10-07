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

	int currentTime;
	int lifeTime;
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

	void explosionGenerate(float x, float y, int maxAngle, float radius, float particleSpeed, int frameDelay);

	void pointActive();
	void explosionActive();
	void generate(string keyName, float x, float y, float angle, int delay, float speed);
};