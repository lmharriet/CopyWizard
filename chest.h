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

	int time;
	bool isDie;
public:
	HRESULT init(string keyName , POINT _pos, int _hp);

	void update();
	//void render();


	RECT getRc() { return rc; }
	void damaged(int value) { hp -= value; }

};