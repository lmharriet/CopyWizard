#pragma once
#include "singletonBase.h"
#include "bossSkill.h"
class bossManager : public singletonBase<bossManager>
{
private:
	vector<bossSkill*> _vBossSkill;
public:
	HRESULT init(float x, float y, int deleteTime, int type);
	void release();
	void update();
	void render(HDC hdc);
};

