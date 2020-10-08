#pragma once
#include "singletonBase.h"

class playerData : public singletonBase <playerData>
{
private:
	int hp;
	int coin;
	float _x, _y;
	int maxHp;

	int _gaugeTime;
public:
	HRESULT init();
	void release();

	int getMaxHp() { return maxHp; }
	int getHp() { return hp; }
	int getCoin() { return coin; }
	float getX() { return _x; }
	float getY() { return _y; }
	int getGaugeTime() { return _gaugeTime; }

	void setHp(int _hp) { hp = _hp; }
	void setCoin(int _coin) { coin = _coin; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setGaugeTime(int time) { _gaugeTime = time; }

	playerData() {}
	~playerData() {}
};

