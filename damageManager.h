#pragma once
#include "singletonBase.h"
struct tagDamage
{
	float x, y;
	int damage;

	int upForce;
	int gravity;

	float size;

	bool isUp;
	bool isLeft;
	int lifeTime;
};
class damageManager : public singletonBase<damageManager>
{
	vector<tagDamage> vDamage;
	int time;
public:
	HRESULT init();
	void render(HDC hdc);

	void generator(POINT pt, int damage = 0, bool isLeft = true);

	
};