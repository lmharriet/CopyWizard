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

struct infoTransfer
{
	int gCoin;
	int sCoin;
	POINT pt;
};

class dropManager : public singletonBase<dropManager>
{
private:
	vector<tagCoin> vCoin;
	vector<infoTransfer> vTransfer;

	int dtime;
public:
	HRESULT init();
	void update();
	void render(HDC hdc);
	void dropPoint(POINT pt, int minCoin, int maxCoin);
	void coinGenerator();

	RECT getCoinRect(int index) { return vCoin[index].rc; }
	void delCoin(int index) { vCoin.erase(vCoin.begin() + index); }
};