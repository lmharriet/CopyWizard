#pragma once
#include "singletonBase.h"

struct tagItem
{
	string keyName;			//아이템 이름
	float damage;			//데미지
	float criDamage;		//크리티컬 데미지
	float criChance;		//크리티컬 확률
	int maxHp;				//최대 체력
	int speed;				//속도

	float potionDropChance;	//포션 드랍율
	float defenceChance;	//공격 방어율
	float burnChance;		//불태울 확률

	bool vampireBlood;		//흡혈기능

	int price;				//가격
	string description;		//아이템 설명
	POINT frame;			//프레임 번호
};

class itemManager : public singletonBase <itemManager>
{
private:
	vector<tagItem> vItem;

public:
	HRESULT init();
	void addImage();
	void addItem(string keyName, float damage, float criDamage, float criChance, int maxHp, int speed, 
		float potionDropChance, float defenceChance, float burnChance, bool vampireBlood, int price, string description, POINT frame);

	tagItem getItem(string keyName);
};