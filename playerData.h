#pragma once
#include "singletonBase.h"

class playerData : public singletonBase <playerData>
{
private:
	int _hp;
	float _x, _y;

public:
	HRESULT init();
	void release();

	int getHp() { return _hp; }
	float getX() { return _x; }
	float getY() { return _y; }
	void setHp(int hp) { _hp = hp; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }

	playerData() {}
	~playerData() {}
};

