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

	void setCoin(int num) { coin = num; }
	void setHp(int _hp) { hp = _hp; }


};