#pragma once
#include "gameNode.h"
class chest : public gameNode
{
private:
	int coin;
	string arcanaKeyName;
	string arcanaName;
	int arcanaCoolTime;

	int hp;

	string keyName;
	int frameX;
	POINT pos;
	RECT rc;

	int hitCount;

	int time;
	int opacity;
	bool isDie;
	bool isBoxAppear;
public:
	HRESULT init(string keyName , POINT _pos, int _hp);

	void update();

	void bossScene_update();

	void damaged(POINT pt, int value, int skillNum, bool criCheck);
	void render(HDC hdc);
	//void render();

	POINT getPos() { return pos; }
	RECT getRc() { return rc; }
	int getHp() { return hp; }

};