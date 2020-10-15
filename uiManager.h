#pragma once
#include "singletonBase.h"
struct tagSkill
{
	string keyName;
	int coolTime;

	bool available;
	int maxCoolTime;
};
class uiManager : public singletonBase<uiManager>
{
private:
	tagSkill skillSlot[4];

	int coin;
	int hp;
	float skillGauge;

public:
	HRESULT init();

	void update();

	void render(HDC hdc, int destX, int destY);

	void infoRender(HDC hdc, int destX, int destY);

	void coinRender(HDC hdc);

	void skillRender(HDC hdc);

	void setCoin(int num) { coin = num; }
	void setHp(int _hp) { hp = _hp; }
	void setSkillGauge(float gauge) { skillGauge = gauge; }

	void setSkillSlot(string keyName, int sec);
	void addCoolTime(int index) { skillSlot[index].available = false; }
	void addCoolTime(string keyName);

	//del

	tagSkill getSkillSlot(int index) { return skillSlot[index]; }

};