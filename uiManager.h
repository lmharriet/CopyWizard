#pragma once
#include "singletonBase.h"
#define SKILLSLOT_MAX 6
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

struct tagButtonAnimation
{
	string keyName;
	int frameX;
	int delay;
	POINT pos;
	bool isActive;
};

class uiManager : public singletonBase<uiManager>
{
private:
	tagSkill skillSlot[SKILLSLOT_MAX];

	tagSkillGaugeBlink gaugeBlink;

	int coin;
	int hp;
	float skillGauge;

	int blackOpacity;
	int blackOpacity2;
	int time;
	//
	tagButtonAnimation Fbutton;
	tagButtonAnimation itemFbutton;
public:
	HRESULT init();

	void update();

	void FbuttonRender(HDC hdc);

	void FbuttonRender2(HDC hdc, POINT pt);

	void setActiveButton2(bool temp) { itemFbutton.isActive = temp; }

	void setPoint(POINT pt);

	void render(HDC hdc, int destX, int destY);

	void infoRender(HDC hdc, int destX, int destY);

	void coinRender(HDC hdc);

	void GamRender(HDC hdc);

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

	void setActiveButton(bool temp) { Fbutton.isActive = temp; }

	//warp screen
	void fadeIn();
	void fadeOut();
};