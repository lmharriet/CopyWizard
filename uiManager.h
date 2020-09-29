#pragma once
#include "singletonBase.h"
struct tagSkill
{
	string keyName;
	int coolTime;
};
class uiManager : public singletonBase<uiManager>
{
private:
	int coin;
	int hp;

public:
	HRESULT init();

	void infoRender(HDC hdc, int destX, int destY);

	void coinRender(HDC hdc);

	void setCoin(int num) { coin = num; }
	void setHp(int _hp) { hp = _hp; }
};