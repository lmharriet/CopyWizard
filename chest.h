#pragma once
#include "gameNode.h"
class chest : public gameNode
{
private:
	int coin;
	string arcanaName;
	int hp;

	string keyName;
	int frameX;
	POINT pos;
	RECT rc;

	int hitCount;

	int time;
	bool isDie;
public:
	HRESULT init(string keyName , POINT _pos, int _hp);

	void update();

	void damaged(POINT pt, int value, int skillNum, bool criCheck);
	//void render();

	POINT getPos() { return pos; }
	RECT getRc() { return rc; }
	int getHp() { return hp; }

};