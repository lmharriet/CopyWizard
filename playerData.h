#pragma once
#include "singletonBase.h"

class playerData : public singletonBase <playerData>
{
private:
	int hp;
	int coin;
	float _x, _y;

public:
	HRESULT init();
	void release();

	int getHp() { return hp; }
	int getCoin() { return coin; }
	float getX() { return _x; }
	float getY() { return _y; }

	void setHp(int _hp) { hp = _hp; }
	void setCoin(int _coin) { coin = _coin; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }

	playerData() {}
	~playerData() {}
};

