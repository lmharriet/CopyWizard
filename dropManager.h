#pragma once
#include "singletonBase.h"

//코인, 포션

/*
* 실버는 최소 단위
* 골드는 실버 5개와 같음
*/

//코인은 최소 3실버 (최소 3) 최대 1골드 1실버 (최대 6)
//생성위치는 posX +- 20, posY +- 10
//실버 코인 frameY = 0, 골드 코인 frameY = 1
//코인 이미지는 10x10
//코인은 렉트가 있어야 colCheck 함수 사용 가능

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