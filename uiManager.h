#pragma once
#include "singletonBase.h"
struct tagSkill
{
	string keyName;
	int coolTime;

	bool available;
	int maxCoolTime;
};

struct tagSkillGaugeBlink
{
	int opacity;

	bool isUp;
	bool isActive;
};

class uiManager : public singletonBase<uiManager>
{
private:
	tagSkill skillSlot[4];

	tagSkillGaugeBlink gaugeBlink;

	int coin;
	int hp;
	float skillGauge;

	int blackOpacity;
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
	void setSkillSlotIndex(int  index, string keyName, int sec);
	void fixCoolTime(string keyName, int time);
	void addCoolTime(int index) { skillSlot[index].available = false; }
	void addCoolTime(string keyName);

	tagSkill getSkillSlot(int index) { return skillSlot[index]; }


	//warp screen
	void fadeIn();
};