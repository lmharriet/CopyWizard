#pragma once
#include "singletonBase.h"

//����, ����

/*
* �ǹ��� �ּ� ����
* ���� �ǹ� 5���� ����
*/

//������ �ּ� 3�ǹ� (�ּ� 3) �ִ� 1��� 1�ǹ� (�ִ� 6)
//������ġ�� posX +- 20, posY +- 10
//�ǹ� ���� frameY = 0, ��� ���� frameY = 1
//���� �̹����� 10x10
//������ ��Ʈ�� �־�� colCheck �Լ� ��� ����

struct tagCoin
{
	POINT pt;
	RECT rc;
	int money;
	
	int currentFrameX;
	int frameY;
};

struct tagHealBall
{
	POINT pt;
	RECT rc;
	int heal;

	int currentFrameX;
};

struct infoTransfer
{
	int gCoin;
	int sCoin;
	int bCoin;
	POINT pt;
};

class dropManager : public singletonBase<dropManager>
{
private:
	vector<tagHealBall> vHealBall;

	vector<tagCoin> vCoin;
	vector<infoTransfer> vTransfer;

	int dtime;
public:
	HRESULT init();
	void update();
	void render(HDC hdc);
	void dropPoint(POINT pt, int minCoin, int maxCoin, float healBallpercentage = 0.f);
	bool checkPercentage(int persentage);
	void coinGenerator();
	
	vector<tagCoin> getCoinVec() { return vCoin; }
	RECT getCoinRect(int index) { return vCoin[index].rc; }
	void delCoin(int index) { vCoin.erase(vCoin.begin() + index); }

	vector<tagHealBall> getBallVec() { return vHealBall; }
	RECT getHealRect(int index) { return vHealBall[index].rc; }
	void delHeal(int index) { vHealBall.erase(vHealBall.begin() + index); }
};