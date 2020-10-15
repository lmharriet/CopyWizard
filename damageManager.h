#pragma once
#include "singletonBase.h"
struct tagHit
{
	string keyName;

	float x, y;
	int damage;

	int upForce;
	int gravity;

	bool isCritical;

	float size;

	bool isUp;
	bool isLeft;
	int lifeTime;
};
class damageManager : public singletonBase<damageManager>
{
	vector<tagHit> vDamage;
	int time;
public:
	HRESULT init();
	void render(HDC hdc);

	void generator(POINT pt, string keyName, int damage = 0, bool isLeft = true, bool isCritical = false);

	
};