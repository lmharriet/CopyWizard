#pragma once
#include "singletonBase.h"

struct tagCoin
{
	POINT pt;
	RECT rc;
	int money;
	float speed;//follow
	
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

	float minX;
	float maxX;
	float minY;
	float maxY;
};

struct tagDropArcana
{
	string arcanaName;
	RECT rc;
	int coolTime;
	
	float size;
	string keyName;
	POINT pos;
};

struct tagCoinView
{
	POINT pt;

	string keyName;
	int curOpacity;
	float speed;
};

struct tagDropItem
{
	tagItem item;
	RECT rc;
	POINT pos;
};

class dropManager : public singletonBase<dropManager>
{
private:
	vector<tagHealBall> vHealBall;

	vector<tagCoin> vCoin;
	vector<infoTransfer> vTransfer;

	vector<tagCoinView> coinView;
	vector<tagCoinView> saveCoinView;

	vector<tagDropArcana> vArcana;
	vector<tagDropItem> vItem;

	int dtime;
	int cTime;
public:
	HRESULT init();
	void update();
	void render(HDC hdc);
	void cardRender(HDC hdc);
	void coinEffectRender(HDC hdc);
	void dropPoint(POINT pt, int minCoin, int maxCoin, float healBallpercentage = 0.f, POINT rangeX = { -20,20 }, POINT rangeY = { -10,10 });
	void dropPointArcana(string keyName, POINT pt, string arcanaName, int coolTime);
	void dropPointItem(tagDropItem _item);
	bool checkPercentage(int persentage);
	void coinGenerator();

	void getCoinEffect(int money);

	void dropPoint_heal(POINT pt, POINT rangeNum, POINT rangeX, POINT rangeY);

	vector<tagCoin> getCoinVec() { return vCoin; }
	RECT getCoinRect(int index) { return vCoin[index].rc; }
	void delCoin(int index) { vCoin.erase(vCoin.begin() + index); }

	POINT getCoinPt(int index) { return vCoin[index].pt; }
	void setCoinPt(int index, int valueX, int valueY) { vCoin[index].pt.x += valueX, vCoin[index].pt.y += valueY; }
	void setRect(int index) { vCoin[index].rc = RectMakeCenter(vCoin[index].pt.x,vCoin[index].pt.y, 20, 20); }

	vector<tagHealBall> getBallVec() { return vHealBall; }
	RECT getHealRect(int index) { return vHealBall[index].rc; }
	void delHeal(int index) { vHealBall.erase(vHealBall.begin() + index); }

	vector<tagDropArcana> getCardVec() { return vArcana; }
	RECT getCardRect(int index) { return vArcana[index].rc; }
	void delCard(int index) { vArcana.erase(vArcana.begin() + index); }

	vector<tagDropItem> getItemVec() { return vItem; }
	RECT getItemRect(int index) { return vItem[index].rc; }
	void delItem(int index) { vItem.erase(vItem.begin() + index); }
};